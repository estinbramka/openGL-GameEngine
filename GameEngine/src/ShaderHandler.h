#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>

class ShaderHandler
{
private:
	std::string vs, fs;
	GLuint ShaderProgram;
	GLuint gScaleLocation;

public:
	ShaderHandler();
	ShaderHandler(std::string _vs, std::string _fs);
	void CompileShaders();
	static bool ReadFile(std::string fileName, std::string& outFile);
	static void AddShader(GLuint ShaderProgram, std::string shaderText, GLenum ShaderType);
	void Scale();

};