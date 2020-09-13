#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderHandler.h"
#include "ModelHandler.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

GLuint VBO;
GLuint IBO;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}


	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	//----------------------------------------------------------------------------------------------------------------------------
	glm::vec3 Vertices[4];
	Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	Vertices[1] = glm::vec3(0.0f, -1.0f, 1.0f);
	Vertices[2] = glm::vec3(1.0f, -1.0f, 0.0f);
	Vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);

	unsigned int Indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 1, 2 };

	ShaderHandler shaderHandler;
	GLuint shaderProgram = shaderHandler.CompileShaders();
	ModelHandler modelHandler(shaderProgram, Vertices, Indices, sizeof(Vertices), sizeof(Indices));
	//----------------------------------------------------------------------------------------------------------------------------

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelHandler.Animation();
		modelHandler.Draw();
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}