#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"

class Camera
{
private:
	glm::vec3 position, target, up;

public:
	Camera();
	void ProcessKeyboard(char key, float deltaTime);
	glm::mat4 GetTransformation();

};