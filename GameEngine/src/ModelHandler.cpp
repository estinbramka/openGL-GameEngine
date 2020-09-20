#include "ModelHandler.h"

ModelHandler::ModelHandler(GLuint _ShaderProgram, float *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices, Camera *_camera)
{
	ShaderProgram = _ShaderProgram;
	pVertices = _pVertices;
	pIndices = _pIndices;
	sizeVertices = _sizeVertices;
	sizeIndices = _sizeIndices;
	camera = _camera;

	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_WorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotate = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, pVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, pIndices, GL_STATIC_DRAW);

	gCameraLocation = glGetUniformLocation(ShaderProgram, "gCamera");
	if (gCameraLocation == -1)
	{
		fprintf(stderr, "Error with glGetUniformLocation\n");
	}

	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	if (gSampler == -1)
	{
		fprintf(stderr, "Error with glGetUniformLocation\n");
	}

	ModelHandler::GetWorldTransformation();
	m_CameraTransformation = camera->GetTransformation();
	m_Transformation = m_CameraTransformation * m_WorldTransformation;

	glUniformMatrix4fv(gCameraLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniform1i(gSampler, 0);
}

void ModelHandler::Animation(float deltaTime)
{
	static float Scale = 0.0f;

	Scale += deltaTime*150;
	
	//ModelHandler::Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	//ModelHandler::WorldPos(sinf(Scale), 0.0f, 0.0f);
	//ModelHandler::Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
	ModelHandler::Rotate(0.0f, Scale, 0.0f);
	ModelHandler::WorldPos(0.0f, 0.0f, 3.0f);

	ModelHandler::GetWorldTransformation();

	m_CameraTransformation = camera->GetTransformation();

	m_Transformation = m_CameraTransformation * m_WorldTransformation;

	glUniformMatrix4fv(gCameraLocation, 1, GL_FALSE, &m_Transformation[0][0]);
}

void ModelHandler::Draw()
{
	glUseProgram(ShaderProgram);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void ModelHandler::Scale(float x, float y, float z)
{
	m_Scale = glm::vec3(x, y, z);
}

void ModelHandler::WorldPos(float x, float y, float z)
{
	m_WorldPos = glm::vec3(x, y, z);
}

void ModelHandler::Rotate(float x, float y, float z)
{
	m_Rotate = glm::vec3(x, y, z);
}

glm::mat4 ModelHandler::GetWorldTransformation()
{
	m_WorldTransformation = glm::mat4(1.0f);
	m_WorldTransformation = glm::translate(m_WorldTransformation, m_WorldPos);
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_WorldTransformation = glm::scale(m_WorldTransformation, m_Scale);

	return m_WorldTransformation;
}

void ModelHandler::LoadTextures(std::string texturePath, GLenum format)
{
	texture;
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
	unsigned char *data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
