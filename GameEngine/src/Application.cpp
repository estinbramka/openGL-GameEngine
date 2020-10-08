#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "ShaderHandler.h"
#include "ModelHandler.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Lighting.h"
#include "Mesh.h"
//#include <assimp/Importer.hpp>

void ProcessInput(GLFWwindow* window);
void Mouse_Callback(GLFWwindow* window, double xpos, double ypos);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera *camera;

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World", NULL, NULL);
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
	/*float Vertices[4*3+4*2]= {
		// positions                 // texture coords
		-1.0f, -1.0f, 0.5773f,		 0.0f, 0.0f, // top right
		0.0f, -1.0f, -1.15475f,		 0.5f, 0.0f, // bottom right
		1.0f, -1.0f, 0.5773f,		 1.0f, 0.0f, // bottom left
		0.0f, 1.0f, 0.0f,			 0.5f, 1.0f  // top left 
	};*/

	Vertex Vertices[] = {
		{glm::vec3(-1.0f, -1.0f,  0.5773f),	glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  0.0f)},
		{glm::vec3(0.0f, -1.0f, -1.15475f),	glm::vec2(0.5f, 0.0f), glm::vec3(0.0f,  0.0f,  0.0f)},
		{glm::vec3(1.0f, -1.0f,  0.5773f),	glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f,  0.0f)},
		{glm::vec3(0.0f,  1.0f,  0.0f),		glm::vec2(0.5f, 1.0f), glm::vec3(0.0f,  0.0f,  0.0f)}
	};
	
	unsigned int Indices[] = { 0, 3, 1,
							   1, 3, 2,
							   2, 3, 0,
							   0, 1, 2 };

	ShaderHandler shaderHandler;
	GLuint shaderProgram = shaderHandler.CompileShaders();

	camera = new Camera();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, Mouse_Callback);

	ModelHandler modelHandler(shaderProgram, Vertices, Indices, sizeof(Vertices), sizeof(Indices), camera);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	modelHandler.LoadTextures("./res/textures/test.png", GL_RGBA);

	/*lighting test*/
	GLuint m_matSpecularIntensityLocation;
	GLuint m_matSpecularPowerLocation;
	m_matSpecularIntensityLocation = glGetUniformLocation(shaderProgram, "gMatSpecularIntensity");
	m_matSpecularPowerLocation = glGetUniformLocation(shaderProgram, "gSpecularPower");
	glUniform1f(m_matSpecularIntensityLocation, 1.0f);
	glUniform1f(m_matSpecularPowerLocation, 32);

	Lighting lighting(shaderProgram);
	Mesh mesh;
	mesh.LoadMesh("./res/meshes/ufo/Low_poly_UFO.3DS");
	//----------------------------------------------------------------------------------------------------------------------------

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/*Time logic here*/
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/*Keyboard Input here*/
		ProcessInput(window);

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelHandler.Animation(deltaTime);
		modelHandler.Draw();
		mesh.Render();
		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void ProcessInput(GLFWwindow * window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard('W', deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard('S', deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard('A', deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard('D', deltaTime);
}

void Mouse_Callback(GLFWwindow * window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX, lastY;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;
	//std::cout << xoffset << " - " << yoffset << std::endl;
	camera->ProsessMouse(xoffset, yoffset);
}
