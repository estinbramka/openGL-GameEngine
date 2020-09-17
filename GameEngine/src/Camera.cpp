#include "Camera.h"

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, -3.0f);
	target = glm::vec3(0.0f, 0.0f, 2.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	//m_Transformation = glm::lookAt(position, target, up);
}

void Camera::ProcessKeyboard(char key, float deltaTime)
{
	if (key == 'W')
		position += glm::vec3(0.0f, 0.0f, 1.0f)*deltaTime*10.0f;
	if (key == 'S')
		position -= glm::vec3(0.0f, 0.0f, 1.0f)*deltaTime*10.0f;
	if (key == 'A')
		std::cout << "A\n";
	if (key == 'D')
		std::cout << "D\n";
}

glm::mat4 Camera::GetTransformation()
{
	return glm::lookAt(position, target, up);
}
