#include "ShaderHandler.h"

ShaderHandler::ShaderHandler()
{
	vs = "vs.shader";
	fs = "fs.shader";
}

ShaderHandler::ShaderHandler(std::string _vs, std::string _fs)
{
	vs = _vs;
	fs = _fs;
}

void ShaderHandler::CompileShaders()
{
	ShaderProgram = glCreateProgram();

	if (ShaderProgram == 0) {
		fprintf(stderr, "Error creating shader program\n");
	}

	std::string vsContent, fsContent;

	if (!ReadFile(vs, vsContent)) {
		fprintf(stderr, "Error reading Vertex Shader\n");
	}

	if (!ReadFile(fs, fsContent)) {
		fprintf(stderr, "Error reading Fragment Shader\n");
	}

	AddShader(ShaderProgram, vsContent, GL_VERTEX_SHADER);
	AddShader(ShaderProgram, fsContent, GL_FRAGMENT_SHADER);

	GLint Success = 0;
	GLchar ErrorLog[1024] = { 0 };

	glLinkProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(ShaderProgram);
	glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
	if (!Success) {
		glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
	}

	glUseProgram(ShaderProgram);

	gScaleLocation = glGetUniformLocation(ShaderProgram, "gScale");
	if (gScaleLocation == -1)
	{
		fprintf(stderr, "Error with glGetUniformLocation\n");
	}
}

bool ShaderHandler::ReadFile(std::string fileName, std::string & outFile)
{
	fileName = "res/" + fileName;
	std::ifstream f(fileName.c_str());

	if (f.is_open())
	{
		std::string line;
		outFile = "";
		while (std::getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}

		f.close();

		return true;
	}
	else
	{
		fprintf(stderr, "Error in function <ShaderHandler::ReadFile> with filename input: %s\n", fileName.c_str());
	}

	return false;
}

void ShaderHandler::AddShader(GLuint ShaderProgram, std::string shaderText, GLenum ShaderType)
{
	GLuint ShaderObj = glCreateShader(ShaderType);

	if (ShaderObj == 0) {
		fprintf(stderr, "Error creating shader type %d\n", ShaderType);
	}

	const GLchar* p[1];
	p[0] = shaderText.c_str();
	GLint Lengths[1];
	Lengths[0] = strlen(shaderText.c_str());
	glShaderSource(ShaderObj, 1, p, Lengths);
	glCompileShader(ShaderObj);
	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
		fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
	}

	glAttachShader(ShaderProgram, ShaderObj);
}

void ShaderHandler::Scale()
{
	static float scale = 0.0f;

	scale += 0.002f;

	glUniform1f(gScaleLocation, sinf(scale));
}
