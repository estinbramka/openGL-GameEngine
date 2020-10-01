#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"

const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;

class Camera
{
private:
	glm::vec3 position, front, up, right, worldUp;
	float yaw, pitch;
	glm::mat4 m_ProjectionTransformation;
	void UpdateCameraVectors();

public:
	Camera();
	void ProcessKeyboard(char key, float deltaTime);
	void ProsessMouse(float xoffset, float yoffset);
	glm::mat4 GetTransformation();
	glm::vec3 GetPosition();

};