#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Definitions.h"

struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;

	BaseLight()
	{
		Color = glm::vec3(0.0f, 0.0f, 0.0f);
		AmbientIntensity = 0.0f;
		DiffuseIntensity = 0.0f;
	}
};

struct DirectionalLight : public BaseLight
{
	glm::vec3 Direction;

	DirectionalLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
	}
};

struct PointLight : public BaseLight
{
	glm::vec3 Position;

	struct
	{
		float Constant;
		float Linear;
		float Exp;
	} Attenuation;

	PointLight()
	{
		Position = glm::vec3(0.0f, 0.0f, 0.0f);
		Attenuation.Constant = 1.0f;
		Attenuation.Linear = 0.0f;
		Attenuation.Exp = 0.0f;
	}
};

struct SpotLight : public PointLight
{
	glm::vec3 Direction;
	float Cutoff;

	SpotLight()
	{
		Direction = glm::vec3(0.0f, 0.0f, 0.0f);
		Cutoff = 0.0f;
	}
};

class Lighting
{
private:
	GLuint ShaderProgram;

	DirectionalLight directionalLight;
	struct
	{
		GLuint Color, AmbientIntensity, DiffuseIntensity, Direction;
	} directionalLightLocation;

	GLuint numPointLightsLocation, numSpotLightsLocation;

	static const unsigned int MAX_POINT_LIGHTS = 2;
	static const unsigned int MAX_SPOT_LIGHTS = 2;

	PointLight pointLights[MAX_POINT_LIGHTS];
	struct
	{
		GLuint Color, AmbientIntensity, DiffuseIntensity, Position;
		struct
		{
			GLuint Constant, Linear, Exp;
		}Attenuation;

	}pointLightsLocation[MAX_POINT_LIGHTS];

	SpotLight spotLights[MAX_SPOT_LIGHTS];
	struct
	{
		GLuint Color, AmbientIntensity, DiffuseIntensity, Position, Direction, Cutoff;
		struct
		{
			GLuint Constant, Linear, Exp;
		}Attenuation;

	}spotLightsLocation[MAX_SPOT_LIGHTS];

	void InitLocations();

public:
	Lighting(GLuint _ShaderProgram);
	void SetDirectionalLight(DirectionalLight _directionalLight);
	void SetPointLights(unsigned int numLights, PointLight *lights);
	void SetSpotLights(unsigned int numLights, SpotLight *lights);
};