#include "Lighting.h"

void Lighting::InitLocations()
{
	directionalLightLocation.Color = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Base.Color");
	directionalLightLocation.AmbientIntensity = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Base.AmbientIntensity");
	directionalLightLocation.Direction = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Direction");
	directionalLightLocation.DiffuseIntensity = glGetUniformLocation(ShaderProgram, "gDirectionalLight.Base.DiffuseIntensity");
	numPointLightsLocation = glGetUniformLocation(ShaderProgram, "gNumPointLights");
	numSpotLightsLocation = glGetUniformLocation(ShaderProgram, "gNumSpotLights");

	if (
		directionalLightLocation.Color == -1 ||
		directionalLightLocation.AmbientIntensity == -1 ||
		directionalLightLocation.Direction == -1 ||
		directionalLightLocation.DiffuseIntensity == -1 ||
		numPointLightsLocation == -1 ||
		numSpotLightsLocation == -1
		)
	{
		fprintf(stderr, "Error with glGetUniformLocation in Lighting class\n");
	}

	for (unsigned int i = 0;i < MAX_POINT_LIGHTS;i++)
	{
		std::string Name;
		Name = "gPointLights[" + std::to_string(i) + ']' + ".Base.Color";
		pointLightsLocation[i].Color = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gPointLights[" + std::to_string(i) + ']' + ".Base.AmbientIntensity";
		pointLightsLocation[i].AmbientIntensity = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gPointLights[" + std::to_string(i) + ']' + ".Base.DiffuseIntensity";
		pointLightsLocation[i].DiffuseIntensity = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gPointLights[" + std::to_string(i) + ']' + ".Position";
		pointLightsLocation[i].Position = glGetUniformLocation(ShaderProgram, Name.c_str());
		
		Name = "gPointLights[" + std::to_string(i) + ']' + ".Atten.Constant";
		pointLightsLocation[i].Attenuation.Constant = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gPointLights[" + std::to_string(i) + ']' + ".Atten.Linear";
		pointLightsLocation[i].Attenuation.Linear = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gPointLights[" + std::to_string(i) + ']' + ".Atten.Exp";
		pointLightsLocation[i].Attenuation.Exp = glGetUniformLocation(ShaderProgram, Name.c_str());

		if (
			pointLightsLocation[i].Color == -1  ||
			pointLightsLocation[i].AmbientIntensity == -1 ||
			pointLightsLocation[i].DiffuseIntensity == -1 ||
			pointLightsLocation[i].Position == -1 ||
			pointLightsLocation[i].Attenuation.Constant == -1 ||
			pointLightsLocation[i].Attenuation.Linear == -1 ||
			pointLightsLocation[i].Attenuation.Exp == -1
			)
		{
			fprintf(stderr, "Error with glGetUniformLocation in Lighting class\n");
		}
	}

	for (unsigned int i = 0;i < MAX_SPOT_LIGHTS;i++)
	{
		std::string Name;
		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Base.Color";
		spotLightsLocation[i].Color = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Base.AmbientIntensity";
		spotLightsLocation[i].AmbientIntensity = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Base.DiffuseIntensity";
		spotLightsLocation[i].DiffuseIntensity = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Position";
		spotLightsLocation[i].Position = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Atten.Constant";
		spotLightsLocation[i].Attenuation.Constant = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Atten.Linear";
		spotLightsLocation[i].Attenuation.Linear = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Base.Atten.Exp";
		spotLightsLocation[i].Attenuation.Exp = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Direction";
		spotLightsLocation[i].Direction = glGetUniformLocation(ShaderProgram, Name.c_str());

		Name = "gSpotLights[" + std::to_string(i) + ']' + ".Cutoff";
		spotLightsLocation[i].Cutoff = glGetUniformLocation(ShaderProgram, Name.c_str());

		if (
			spotLightsLocation[i].Color == -1 ||
			spotLightsLocation[i].AmbientIntensity == -1 ||
			spotLightsLocation[i].DiffuseIntensity == -1 ||
			spotLightsLocation[i].Position == -1 ||
			spotLightsLocation[i].Attenuation.Constant == -1 ||
			spotLightsLocation[i].Attenuation.Linear == -1 ||
			spotLightsLocation[i].Attenuation.Exp == -1 ||
			spotLightsLocation[i].Direction == -1 ||
			spotLightsLocation[i].Cutoff == -1
			)
		{
			fprintf(stderr, "Error with glGetUniformLocation in Lighting class\n");
		}
	}

}

Lighting::Lighting(GLuint _ShaderProgram)
{
	ShaderProgram = _ShaderProgram;

	directionalLight.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight.AmbientIntensity = 0.1f;
	directionalLight.DiffuseIntensity = 0.75f;
	directionalLight.Direction = glm::vec3(-1.0f, 0.0f, 0.0f);

	pointLights[0].Color = glm::vec3(0.0f, 0.0f, 1.0f);
	pointLights[0].DiffuseIntensity = 0.5f;
	pointLights[0].Position = glm::vec3(-3.0f, 1.0f, 1.0f);
	pointLights[0].Attenuation.Linear = 0.1f;
	pointLights[1].Color = glm::vec3(0.0f, 1.0f, 0.0f);
	pointLights[1].DiffuseIntensity = 0.5f;
	pointLights[1].Position = glm::vec3(3.0f, 1.0f, 1.0f);
	pointLights[1].Attenuation.Linear = 0.1f;

	spotLights[0].DiffuseIntensity = 0.9f;
	spotLights[0].Color = glm::vec3(0.0f, 0.0f, 1.0f);
	spotLights[0].Position = glm::vec3(0.0f, 0.0f, -3.0f);
	spotLights[0].Direction = glm::vec3(0.0f, 0.0f, 1.0f);
	spotLights[0].Attenuation.Linear = 0.1f;
	spotLights[0].Cutoff = 3.0f;

	spotLights[1].DiffuseIntensity = 0.9f;
	spotLights[1].Color = glm::vec3(1.0f, 0.0f, 0.0f);
	spotLights[1].Position = glm::vec3(0.0f, -0.2f, -3.0f);
	spotLights[1].Direction = glm::vec3(0.0f, 0.0f, 1.0f);
	spotLights[1].Attenuation.Linear = 0.1f;
	spotLights[1].Cutoff = 3.0f;

	InitLocations();
	SetDirectionalLight(directionalLight);
	SetPointLights(2, pointLights);
	SetSpotLights(2, spotLights);
}

void Lighting::SetDirectionalLight(DirectionalLight _directionalLight)
{
	directionalLight = _directionalLight;
	directionalLight.Direction = glm::normalize(directionalLight.Direction);
	glUniform3f(directionalLightLocation.Color, directionalLight.Color.x, directionalLight.Color.y, directionalLight.Color.z);
	glUniform1f(directionalLightLocation.AmbientIntensity, directionalLight.AmbientIntensity);
	glUniform1f(directionalLightLocation.DiffuseIntensity, directionalLight.DiffuseIntensity);
	glUniform3f(directionalLightLocation.Direction, directionalLight.Direction.x, directionalLight.Direction.y, directionalLight.Direction.z);
}

void Lighting::SetPointLights(unsigned int numLights, PointLight * lights)
{
	glUniform1i(numPointLightsLocation, numLights);

	for (unsigned int i = 0;i < numLights;i++)
	{
		pointLights[i] = lights[i];

		glUniform3f(pointLightsLocation[i].Color, pointLights[i].Color.x, pointLights[i].Color.y, pointLights[i].Color.z);
		glUniform3f(pointLightsLocation[i].Position, pointLights[i].Position.x, pointLights[i].Position.y, pointLights[i].Position.z);
		glUniform1f(pointLightsLocation[i].AmbientIntensity, pointLights[i].AmbientIntensity);
		glUniform1f(pointLightsLocation[i].DiffuseIntensity, pointLights[i].DiffuseIntensity);
		glUniform1f(pointLightsLocation[i].Attenuation.Constant, pointLights[i].Attenuation.Constant);
		glUniform1f(pointLightsLocation[i].Attenuation.Exp, pointLights[i].Attenuation.Exp);
		glUniform1f(pointLightsLocation[i].Attenuation.Linear, pointLights[i].Attenuation.Linear);
	}
}

void Lighting::SetSpotLights(unsigned int numLights, SpotLight * lights)
{
	glUniform1i(numSpotLightsLocation, numLights);

	for (unsigned int i = 0;i < numLights;i++)
	{
		spotLights[i] = lights[i];

		spotLights[i].Direction = glm::normalize(spotLights[i].Direction);
		glUniform3f(spotLightsLocation[i].Color, spotLights[i].Color.x, spotLights[i].Color.y, spotLights[i].Color.z);
		glUniform3f(spotLightsLocation[i].Position, spotLights[i].Position.x, spotLights[i].Position.y, spotLights[i].Position.z);
		glUniform1f(spotLightsLocation[i].AmbientIntensity, spotLights[i].AmbientIntensity);
		glUniform1f(spotLightsLocation[i].DiffuseIntensity, spotLights[i].DiffuseIntensity);
		glUniform1f(spotLightsLocation[i].Attenuation.Constant, spotLights[i].Attenuation.Constant);
		glUniform1f(spotLightsLocation[i].Attenuation.Exp, spotLights[i].Attenuation.Exp);
		glUniform1f(spotLightsLocation[i].Attenuation.Linear, spotLights[i].Attenuation.Linear);
		glUniform3f(spotLightsLocation[i].Direction, spotLights[i].Direction.x, spotLights[i].Direction.y, spotLights[i].Direction.z);
		glUniform1f(spotLightsLocation[i].Cutoff, cosf(glm::radians(spotLights[i].Cutoff)));
	}
}
