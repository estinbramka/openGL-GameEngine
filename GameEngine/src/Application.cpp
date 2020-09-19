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
	glm::vec3 Vertices[4];
	Vertices[0] = glm::vec3(-1.0f, -1.0f, 0.5773f);
	Vertices[1] = glm::vec3(0.0f, -1.0f, -1.15475f);
	Vertices[2] = glm::vec3(1.0f, -1.0f, 0.5773f);
	Vertices[3] = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec2 TextureCoo[4];

	TextureCoo[0] = glm::vec2(0.0f, 0.0f);
	TextureCoo[1] = glm::vec2(0.5f, 0.0f);
	TextureCoo[2] = glm::vec2(1.0f, 0.0f);
	TextureCoo[3] = glm::vec2(0.5f, 1.0f);

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

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	const char *texturePath = "res/textures/test.png";
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	GLuint gSampler;
	gSampler = glGetUniformLocation(shaderProgram, "gSampler");
	glUniform1i(gSampler, 0);


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
		glClear(GL_COLOR_BUFFER_BIT);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		modelHandler.Animation(deltaTime);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureCoo), 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		modelHandler.Draw();
		glDisableVertexAttribArray(1);
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
	std::cout << xoffset << " - " << yoffset << std::endl;
	camera->ProsessMouse(xoffset, yoffset);
}
