#pragma once

#include <glm/glm.hpp>

struct DirectionalLight {
	glm::vec3 Direction;
	glm::vec3 Color;

	DirectionalLight() = default;
	DirectionalLight(glm::vec3 direction, glm::vec3 color)
	    : Direction(direction),
	      Color(color) {}
};

struct PointLight {
	glm::vec3 Position;
	glm::vec3 Color;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

	PointLight() = default;

	PointLight(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic)
	    : Position(position),
	      Color(color),
	      ConstantAttenuation(constant),
	      LinearAttenuation(linear),
	      QuadraticAttenuation(quadratic) {}
};

struct SpotLight {
	glm::vec3 Position;
	glm::vec3 Direction;
	glm::vec3 Color;

	float ConstantAttenuation;
	float LinearAttenuation;
	float QuadraticAttenuation;

	float CutoffAngle;
	float ShadowSmoothAngle;

	SpotLight() = default;
	SpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float constant, float linear, float quadratic, float cutoff, float smoothing)
	    : Position(position),
		  Direction(direction),
	      Color(color),
	      ConstantAttenuation(constant),
	      LinearAttenuation(linear),
	      QuadraticAttenuation(quadratic),
		  CutoffAngle(cutoff),
		  ShadowSmoothAngle(smoothing) {}
};