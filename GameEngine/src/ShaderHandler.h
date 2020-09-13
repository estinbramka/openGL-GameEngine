#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class ShaderHandler
{
private:
	std::string vs, fs;
	GLuint ShaderProgram;

public:
	ShaderHandler();
	ShaderHandler(std::string _vs, std::string _fs);
	GLuint CompileShaders();
	static bool ReadFile(std::string fileName, std::string& outFile);
	static void AddShader(GLuint ShaderProgram, std::string shaderText, GLenum ShaderType);

};