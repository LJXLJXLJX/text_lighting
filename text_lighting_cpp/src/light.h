#pragma once
#include <glm/glm.hpp>

struct PointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float _near;
	float _far;
};

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct FlashLight {
	glm::vec3 position;
	glm::vec3 direction;

	float cutoff;
	float cutoff_out;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float _near;
	float _far;
};