#include "Camera.h"

void Camera::UpdateCameraVectors()
{
	glm::vec3 tFront;
	tFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	tFront.y = sin(glm::radians(pitch));
	tFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(tFront);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::Camera()
{
	position = glm::vec3(0.0f, 0.0f, -3.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 90.0f;
	pitch = 0;
	UpdateCameraVectors();

	m_ProjectionTransformation = glm::perspective(glm::radians(60.0f), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f);
}

void Camera::ProcessKeyboard(char key, float deltaTime)
{
	float velocity = SPEED * deltaTime;
	if (key == 'W')
		position += front * velocity;
	if (key == 'S')
		position -= front * velocity;
	if (key == 'A')
		position -= right * velocity;
	if (key == 'D')
		position += right * velocity;
}

void Camera::ProsessMouse(float xoffset, float yoffset)
{
	xoffset *= SENSITIVITY;
	yoffset *= SENSITIVITY;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	UpdateCameraVectors();

}

glm::mat4 Camera::GetTransformation()
{
	return m_ProjectionTransformation * glm::lookAt(position, position + front, up);
}
