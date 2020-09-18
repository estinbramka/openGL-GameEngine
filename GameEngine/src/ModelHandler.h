#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"
#include "Camera.h"

class ModelHandler
{
private:
	GLuint ShaderProgram;
	GLuint gCameraLocation;
	glm::vec3 *pVertices;
	unsigned int *pIndices;
	size_t sizeVertices, sizeIndices;
	GLuint VBO, IBO;
	glm::vec3 m_Scale, m_WorldPos, m_Rotate;
	glm::mat4 m_WorldTransformation;
	glm::mat4 m_CameraTransformation;
	glm::mat4 m_Transformation;
	Camera *camera;

public:
	ModelHandler(GLuint _ShaderProgram, glm::vec3 *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices, Camera *camera);
	void Animation(float deltaTime);
	void Draw();
	void Scale(float x, float y, float z);
	void WorldPos(float x, float y, float z);
	void Rotate(float x, float y, float z);
	glm::mat4 GetWorldTransformation();

};