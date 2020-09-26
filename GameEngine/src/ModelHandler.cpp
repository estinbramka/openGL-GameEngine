#include "ModelHandler.h"

void ModelHandler::CalcNormals()
{
	// Accumulate each triangle normal into each of the triangle vertices
	for (unsigned int i = 0; i < (sizeIndices / sizeof(unsigned int)); i += 3) {
		unsigned int Index0 = pIndices[i];
		unsigned int Index1 = pIndices[i + 1];
		unsigned int Index2 = pIndices[i + 2];
		glm::vec3 v1 = pVertices[Index1].position - pVertices[Index0].position;
		glm::vec3 v2 = pVertices[Index2].position - pVertices[Index0].position;
		glm::vec3 Normal = glm::cross(v1, v2);
		Normal = glm::normalize(Normal);

		pVertices[Index0].normal += Normal;
		pVertices[Index1].normal += Normal;
		pVertices[Index2].normal += Normal;
	}

	std::cout << (sizeIndices / sizeof(unsigned int)) << "-" << (sizeVertices / (8*sizeof(float))) << "\n";

	// Normalize all the vertex normals
	for (unsigned int i = 0; i < (sizeVertices / (8 * sizeof(float))); i++) {
		pVertices[i].normal = glm::normalize(pVertices[i].normal);
		std::cout << pVertices[i].normal.x << " " << pVertices[i].normal.y << " " << pVertices[i].normal.z << "\n";
	}
}

ModelHandler::ModelHandler(GLuint _ShaderProgram, Vertex *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices, Camera *_camera)
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

	CalcNormals();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, pVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, pIndices, GL_STATIC_DRAW);

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	if (gWVPLocation == -1)
	{
		fprintf(stderr, "Error with glGetUniformLocation\n");
	}

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	if (gWorldLocation == -1)
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

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
	glUniform1i(gSampler, 0);

	//CalcNormals();
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

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
}

void ModelHandler::Draw()
{
	glUseProgram(ShaderProgram);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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
