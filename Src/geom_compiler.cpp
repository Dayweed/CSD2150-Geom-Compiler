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



    }
}
