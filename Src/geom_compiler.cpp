#include "geom_compiler.h"

#pragma comment(lib, "../../dependencies/assimp/lib/Debug/assimp-vc143-mtd.lib")

int main(int argc, const char* argv[])
{
    xcore::Init("geom_compiler");
    std::filesystem::path target;
    std::filesystem::path desc;
    std::filesystem::path rscpath;

    xcore::log::Output("Welcome to geom compilier.\n");
    //Cmd line parsing
    {
        const int max_input_size = 7;

        if (argc == 2 && strcmp(argv[1], "-help"))
        {
            xcore::log::Output("the command format is as follows: -target <platform> -desc <descriptor filepath> -rscpath <output filepath>\n");
            return 0;
        }

        if (argc != max_input_size)
        {
            xcore::log::Output("ERROR: unknown commands\n");
            return 0;
        }
        //Handling inputs for geom compiler 
        for (int i{}; i < max_input_size; ++i)
        {
            std::string input(argv[i]);
            if (i < max_input_size)
            {
                if (input == "-target")
                {
                    target = std::string(argv[i + 1]).c_str();
                }
                else if (input == "-desc")
                {
                    desc = std::string(argv[i + 1]).c_str();
                }
                else if (input == "-rscpath")
                {
                    rscpath = std::string(argv[i + 1]).c_str();
                }
            }
        }

        //Check input
        if (target != "PC")
        {
            xcore::log::Output("ERROR: Unknown target platform given. Terminating program.\n");
            return 0;
        }
        if (!std::filesystem::exists(desc))
        {
            xcore::log::Output("ERROR: Could not find the descriptor path. Terminating program.\n");
            return 0;
        }

        //The Descriptor file
        Descriptor descriptor;
        {
            std::ifstream ifs(desc);
            if (!ifs)
            {
                xcore::log::Output("ERROR: Could not open descriptor file. Terminating program.\n");
                return 0;
            }

            std::string str;
            while (std::getline(ifs, str))
            {
                size_t pos = str.find_first_of(':');
                std::string key = str.substr(0, pos);
                std::string val = str.substr(pos + 1);
                if (key == "path")
                {
                    descriptor.m_path_to_asset = val;
                }
                else if (key == "scale")
                {
                    std::stringstream ss(val);
                    char tmp;
                    ss >> tmp >> descriptor.m_scale[0] >> tmp >> descriptor.m_scale[1] >> tmp >> descriptor.m_scale[2];
                }
                else if (key == "rotation")
                {
                    std::stringstream ss(val);
                    char tmp;
                    ss >> tmp >> descriptor.m_rotation[0] >> tmp >> descriptor.m_rotation[1] >> tmp >> descriptor.m_rotation[2];
                }
                else if (key == "translation")
                {
                    std::stringstream ss(val);
                    char tmp;
                    ss >> tmp >> descriptor.m_translate[0] >> tmp >> descriptor.m_translate[1] >> tmp >> descriptor.m_translate[2];
                }
                else if (key == "merge_all_meshes")
                {
                    descriptor.m_merge = val != "0";
                }
                else if (key == "rename_mesh")
                {
                    descriptor.m_mesh_name = val;
                }
            }
        }

        //Geom Structure
        Geom geom;
        xcore::log::Output("Loading Geom\n");
        {
            Assimp::Importer assimp;
            uint32_t flag = aiPostProcessSteps::aiProcess_Triangulate |
                aiPostProcessSteps::aiProcess_GenUVCoords |
                aiPostProcessSteps::aiProcess_TransformUVCoords |
                aiPostProcessSteps::aiProcess_FlipUVs |
                aiPostProcessSteps::aiProcess_SortByPType |
                aiPostProcessSteps::aiProcess_JoinIdenticalVertices |
                aiPostProcessSteps::aiProcess_RemoveRedundantMaterials |
                aiPostProcessSteps::aiProcess_FindInvalidData |
                aiPostProcessSteps::aiProcess_CalcTangentSpace;

            if (descriptor.m_merge)
            {
                flag |= aiPostProcessSteps::aiProcess_PreTransformVertices;
            }

            std::filesystem::path path_to_asset = desc.parent_path() / descriptor.m_path_to_asset;
            const aiScene* aiscene = assimp.ReadFile(path_to_asset.string(), flag);

            geom.nMesh = aiscene->mNumMeshes;
            for (uint32_t i{}; i < geom.nIndices < esh; ++i)
            {
                aiMesh* mesh = aiscene->mMeshes[i];

                uint32_t nFace = mesh->mNumFaces;
                geom.nPos += mesh->mNumVertices;
                for (uint32_t j{}; j < nFace; ++j)
                {
                    geom.nIndices += mesh->mFaces[j].mNumIndices;
                }
            }
            geom.nExtras = geom.nPos;
            geom.nLOD = geom.nMesh;
            geom.nSubMesh = geom.nMesh;

            geom.Allocate();

            uint32_t LODIndex = 0;
            uint32_t submeshIndex = 0;
            uint32_t verticesIndex = 0;
            uint32_t indicesIndex = 0;

            for (uint32_t i{}; i < geom.nMesh; ++i)
            {
                aiMesh* aimesh = aiscene->mMeshes[i];
                Mesh& mesh = geom.pMesh[i];

                for (uint32_t j{}; j < aimesh->mNumVertices; ++j)
                {
                    geom.pPos[verticesIndex + j].pos.m_X = aimesh->mVertices[j].x;
                    geom.pPos[verticesIndex + j].pos.m_Y = aimesh->mVertices[j].y;
                    geom.pPos[verticesIndex + j].pos.m_Z = aimesh->mVertices[j].z;
                }

                strcpy_s(&mesh.m_name[0], mesh.m_name.max_size(), aimesh->mName.C_Str());

                mesh.m_nLODS = 1;
                mesh.m_iLODS = LODIndex;
                LODIndex += mesh.m_nLODS;

                for (uint32_t k{ mesh.m_iLODS }; k < (mesh.m_iLODS + mesh.m_nLODS); ++k)
                {
                    LODS& lod = geom.pLOD[k];
                    lod.m_nLODSubmeshes = 1;
                    lod.m_iLODSubmesh = submeshIndex;
                    submeshIndex += lod.m_nLODSubmeshes;

                    for (uint32_t l{ lod.m_iLODSubmesh }; l < (lod.m_iLODSubmesh + lod.m_nLODSubmeshes); ++l)
                    {
                        SubMeshes& SubMesh = geom.pSubMesh[l];
                        SubMesh.m_nFaces = aimesh->mNumfaces;

                        SubMesh.m_nVertices = aimesh->mNumVertices;
                        SubMesh.m_iVertices = SubMesh.m_nVertices;

                        for (uint32_t m{}; m < aimesh.mNumFaces; ++m)
                        {
                            SubMesh.m_nIndices += aimesh->mFaces[m].mNumIndices;
                            for (uint32_t n{}; n < aimesh->mFaces[m].mNumIndices; ++n)
                            {
                                geom.pIndices[indicesIndex + n].m_index = aimesh->mFaces[m].mIndices[n];
                            }
                            indicesIndex += aimesh->mFaces[m].mNumIndices;
                        }

                        SubMesh.m_iMaterial = aimesh->mMaterialIndex;
                    }
                }
            }
            
        }



    }
}
