#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <array>
#include <functional>

#include <xcore/src/xcore.h>

struct Descriptor
{
	std::filesystem::path m_path_to_asset;
	float m_scale[3]{ 1.f, 1.f, 1.f };
	float m_rotation[3]{ 0.f, 0.f, 0.f };
	float m_translate[3]{ 0.f, 0.f, 0.f };
	bool m_merge{ false };
	std::string m_mesh_name{ "mesh" };
};

struct Mesh
{
	std::array<char, 64> m_name;
	uint32_t m_nLODS;
	uint32_t m_iLODS;
};

struct LODS
{
	uint32_t m_nLODSubmeshes = 0;
	uint32_t m_iLODSubmesh = 0;
};

struct SubMeshes
{
	uint32_t m_nFaces = 0;
	uint32_t m_iIndices = 0;
	uint32_t m_iVertices = 0;
	uint32_t m_iMaterial = 0;
	uint32_t m_nIndices = 0;
	uint32_t m_nVertices = 0;
};

struct Indices
{
	uint32_t m_index;
};

struct Pos
{
	xcore::vector3 pos;
};

struct  Geom
{
	Mesh* pMesh = nullptr;
	LODS* pLOD = nullptr;
	SubMeshes* pSubMesh = nullptr;
	Pos* pPos = nullptr;
	Indices* pIndices = nullptr;

	uint32_t nMesh = 0, nLOD = 0, nSubMesh = 0, nPos = 0, nIndices = 0;

	//To allocate memory for the pointers after setting the numbers
	void Allocate()
	{
		pMesh = new Mesh[nMesh];
		pLOD = new LODS[nLOD];
		pSubMesh = new SubMeshes[nSubMesh];
		pPos = new Pos[nPos];
		pIndices = new Indices[nIndices];
	}
	//Deallocate the memory
	~Geom()
	{
		if (pMesh) delete[] pMesh;
		if (pLOD) delete[] pLOD;
		if (pSubMesh) delete[] pSubMesh;
		if (pPos) delete[] pPos;
		if (pIndices) delete[] pIndices;
	}
};
