#include "ModelHandler.h"

ModelHandler::ModelHandler(GLuint _ShaderProgram, glm::vec3 *_pVertices, unsigned int *_pIndices, size_t _sizeVertices, size_t _sizeIndices)
{
	ShaderProgram = _ShaderProgram;
	pVertices = _pVertices;
	pIndices = _pIndices;
	sizeVertices = _sizeVertices;
	sizeIndices = _sizeIndices;

	m_Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_WorldPos = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotate = glm::vec3(0.0f, 0.0f, 0.0f);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeVertices, pVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndices, pIndices, GL_STATIC_DRAW);

	gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
	if (gWorldLocation == -1)
	{
		fprintf(stderr, "Error with glGetUniformLocation\n");
	}
	ModelHandler::GetWorldTransformation();
	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
}

void ModelHandler::Animation()
{
	static float Scale = 0.0f;

	Scale += 0.001f;

	ModelHandler::Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	ModelHandler::WorldPos(sinf(Scale), 0.0f, 0.0f);
	ModelHandler::Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);

	ModelHandler::GetWorldTransformation();

	glUniformMatrix4fv(gWorldLocation, 1, GL_FALSE, &m_WorldTransformation[0][0]);
}

void ModelHandler::Draw()
{
	glUseProgram(ShaderProgram);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
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
	m_WorldTransformation = glm::rotate(m_WorldTransformation, ToRadian(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, ToRadian(m_Rotate.y), glm::vec3(0.0f, -1.0f, 0.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, ToRadian(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_WorldTransformation = glm::scale(m_WorldTransformation, m_Scale);

	return m_WorldTransformation;
}
