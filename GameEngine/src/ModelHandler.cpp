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

	Scale += 0.1f;
	
	//ModelHandler::Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
	//ModelHandler::WorldPos(sinf(Scale), 0.0f, 0.0f);
	//ModelHandler::Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
	

	ModelHandler::Rotate(0.0f, Scale, 0.0f);
	ModelHandler::WorldPos(0.0f, 0.0f, 3.0f);

	ModelHandler::GetWorldTransformation();

	glm::mat4 Projection = glm::perspective(glm::radians(60.0f), WINDOW_WIDTH / (float)WINDOW_HEIGHT, 1.0f, 100.0f);
	//Projection[2][3] = 1;
	/*
	for (int i = 0;i < 4;i++)
	{
		for (int j = 0;j < 4;j++)
		{
			std::cout << Projection[j][i] << " ";
		}
		std::cout << std::endl;
	}
	*/

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view;
	view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	m_WorldTransformation = Projection * view * m_WorldTransformation;

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
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_WorldTransformation = glm::rotate(m_WorldTransformation, glm::radians(m_Rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_WorldTransformation = glm::scale(m_WorldTransformation, m_Scale);

	return m_WorldTransformation;
}
