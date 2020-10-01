#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"
#include "Camera.h"
#include <stb_image.h>

class ModelHandler
{
private:
	GLuint ShaderProgram;
	GLuint gWVPLocation, gWorldLocation, gEyeWorldPosLocation;
	Vertex *pVertices;
	unsigned int *pIndices;
	size_t sizeVertices, sizeIndices;
	GLuint VBO, IBO;
	glm::vec3 m_Scale, m_WorldPos, m_Rotate;
	glm::mat4 m_WorldTransformation;
	glm::mat4 m_CameraTransformation;
	glm::mat4 m_Transformation;
	Camera *camera;
	GLuint texture, gSampler;
	void CalcNormals();

public:
	ModelHandler(GLuint _ShaderProgram, Vertex *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices, Camera *camera);
	void Animation(float deltaTime);
	void Draw();
	void Scale(float x, float y, float z);
	void WorldPos(float x, float y, float z);
	void Rotate(float x, float y, float z);
	glm::mat4 GetWorldTransformation();
	void LoadTextures(std::string texturePath, GLenum format);

};