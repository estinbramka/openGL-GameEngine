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

	// Normalize all the vertex normals
	for (unsigned int i = 0; i < (sizeVertices / (8 * sizeof(float))); i++) {
		pVertices[i].normal = glm::normalize(pVertices[i].normal);
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
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	gEyeWorldPosLocation = glGetUniformLocation(ShaderProgram, "gEyeWorldPos");
	
	if (
		gWVPLocation == -1 ||
		gWorldLocation == -1 ||
		gSampler == -1 ||
		gEyeWorldPosLocation == -1
		)
	{
		fprintf(stderr, "Error with glGetUniformLocation in ModelHandler class\n");
	}

	ModelHandler::GetWorldTransformation();
	m_CameraTransformation = camera->GetTransformation();
	m_Transformation = m_CameraTransformation * m_WorldTransformation;

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
	glUniform3f(gEyeWorldPosLocation, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
}

ModelHandler::ModelHandler(GLuint _ShaderProgram, const std::string & Filename, Camera * _camera)
{
	ShaderProgram = _ShaderProgram;
	pVertices = NULL;
	pIndices = NULL;
	sizeVertices = NULL;
	sizeIndices = NULL;
	camera = _camera;

	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_WorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotate = glm::vec3(0.0f, 0.0f, 0.0f);

	LoadMesh(Filename);

	gWVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	gSampler = glGetUniformLocation(ShaderProgram, "gSampler");
	gEyeWorldPosLocation = glGetUniformLocation(ShaderProgram, "gEyeWorldPos");

	if (
		gWVPLocation == -1 ||
		gWorldLocation == -1 ||
		gSampler == -1 ||
		gEyeWorldPosLocation == -1
		)
	{
		fprintf(stderr, "Error with glGetUniformLocation in ModelHandler class\n");
	}

	ModelHandler::GetWorldTransformation();
	m_CameraTransformation = camera->GetTransformation();
	m_Transformation = m_CameraTransformation * m_WorldTransformation;

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
	glUniform3f(gEyeWorldPosLocation, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);
}

void ModelHandler::Animation(float deltaTime)
{
	static float Scale = 0.0f;

	Scale += deltaTime*50;
	
	//ModelHandler::Scale(0.07, 0.07, 0.07);
	//ModelHandler::WorldPos(sinf(Scale), 0.0f, 0.0f);
	//ModelHandler::Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
	ModelHandler::Rotate(-90.0f, Scale, 0.0f);
	//ModelHandler::WorldPos(0.0f, 0.0f, 3.0f);
}

void ModelHandler::Draw()
{
	ModelHandler::GetWorldTransformation();
	m_CameraTransformation = camera->GetTransformation();
	m_Transformation = m_CameraTransformation * m_WorldTransformation;

	glUniformMatrix4fv(gWVPLocation, 1, GL_FALSE, &m_Transformation[0][0]);
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
	glUniform3f(gEyeWorldPosLocation, camera->GetPosition().x, camera->GetPosition().y, camera->GetPosition().z);

	if (pVertices != NULL)
	{
		glUseProgram(ShaderProgram);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		textureObj->Bind(GL_TEXTURE0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
	else
	{
		Render();
	}
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
	glUniform1i(gSampler, 0);

	textureObj = new Texture(texturePath, GL_TEXTURE_2D);
	textureObj->Load();
}
