#pragma once
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Texture.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;

	Vertex(glm::vec3 _position, glm::vec2 _texture, glm::vec3 _normal)
	{
		position = _position;
		texture = _texture;
		normal = _normal;
	}
};

struct MeshEntry
{
	GLuint VB, IB;
	unsigned int NumIndices, MaterialIndex;

	MeshEntry();
	~MeshEntry();
	void Init(const std::vector<Vertex>& Vertices, const std::vector<unsigned int>& Indices);
};

class Mesh
{
private:
	std::vector<MeshEntry> m_Entries;
	std::vector<Texture*> m_Textures;

	bool InitFromScene(const aiScene* pScene, const std::string& Filename);
	void InitMesh(unsigned int Index, const aiMesh* paiMesh);
	bool InitMaterials(const aiScene* pScene, const std::string& Filename);
	void Clear();

public:
	Mesh();
	~Mesh();
	bool LoadMesh(const std::string& Filename);
	void Render();

};