#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#define M_PI 3.14159265358979323846
#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))

class ModelHandler
{
private:
	GLuint ShaderProgram;
	GLuint gWorldLocation;
	glm::vec3 *pVertices;
	size_t sizeVertices;
	unsigned int *pIndices;
	size_t sizeIndices;
	GLuint VBO;
	GLuint IBO;
	glm::vec3 m_Scale;
	glm::vec3 m_WorldPos;
	glm::vec3 m_Rotate;
	glm::mat4 m_WorldTransformation;

public:
	ModelHandler(GLuint _ShaderProgram, glm::vec3 *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices);
	void Animation();
	void Draw();
	void Scale(float x, float y, float z);
	void WorldPos(float x, float y, float z);
	void Rotate(float x, float y, float z);
	glm::mat4 GetWorldTransformation();

};