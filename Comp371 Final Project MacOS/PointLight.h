#pragma once
#include "glm/glm.hpp"

class PointLight {
public:
	int status;//1: On, 0: Off
	glm::vec3 position;//position
	glm::vec3 color;//light color

	//used to calculate attenuation
	float constant;
	float linear_v;
	float quadratic;

	//the light's ambient, diffuse and specular coeficients
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	PointLight();
	PointLight(glm::vec3 const & position, glm::vec3 const & color);
	void init(glm::vec3 const & position, glm::vec3 const & color);
	

	
};
