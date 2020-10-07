#pragma once
#include <GL/glew.h>
#include <string>
#include <stb_image.h>
#include <iostream>

class Texture
{
private:
	std::string filename;
	GLenum textureTarget;
	GLuint textureObject;

public:
	Texture(std::string _filename, GLenum _textureTarget);
	bool Load();
	void Bind(GLenum textureUnit);

};