#pragma once

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texture;
	glm::vec3 normal;

	Vertex(glm::vec3 _position, glm::vec2 _texture)
	{
		position = _position;
		texture = _texture;
		normal = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};