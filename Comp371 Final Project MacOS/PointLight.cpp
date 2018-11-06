#include "PointLight.h"
/*
Point light class
Use to instanciate Point light objects to be used in openGL
The shader should include a similar Struct:

struct PointLight {
	int status;//1: On, 0: Off
	vec3 position;//position
	vec3 color;//light color

	//used to calculate attenuation
	float constant;
	float linear_v;
	float quadratic;

	//the light's ambient, diffuse and specular coeficients
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

You can use PointLight object to store the light's variables/values
*/

//Default Constructor
PointLight::PointLight() {
	position = glm::vec3(0.0f);
	color = glm::vec3(1.0f);

	/*
		Value Table
		Distance	Constant	Linear	Quadratic
		7			1.0			0.7		1.8
		13			1.0			0.35	0.44
		20			1.0			0.22	0.20
		32			1.0			0.14	0.07
		50			1.0			0.09	0.032
		65			1.0			0.07	0.017
		100			1.0			0.045	0.0075
		160			1.0			0.027	0.0028
		200			1.0			0.022	0.0019
		325			1.0			0.014	0.0007
		600			1.0			0.007	0.0002
		3250		1.0			0.0014	0.000007
		*/
	constant = 1.0f;
	linear_v = 0.0014f;
	quadratic = 0.000007f;

	ambient = glm::vec3(0.25f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);

	status = 1;
}

//Init constructor
PointLight::PointLight(glm::vec3 const & position, glm::vec3 const & color) {
	this->position = position;
	this->color = color;

	/*
		Value Table
		Distance	Constant	Linear	Quadratic
		7			1.0			0.7		1.8
		13			1.0			0.35	0.44
		20			1.0			0.22	0.20
		32			1.0			0.14	0.07
		50			1.0			0.09	0.032
		65			1.0			0.07	0.017
		100			1.0			0.045	0.0075
		160			1.0			0.027	0.0028
		200			1.0			0.022	0.0019
		325			1.0			0.014	0.0007
		600			1.0			0.007	0.0002
		3250		1.0			0.0014	0.000007
		*/
	constant = 1.0f;
	linear_v = 0.0014f;
	quadratic = 0.000007f;

	ambient = glm::vec3(0.25f);
	diffuse = glm::vec3(1.0f);
	specular = glm::vec3(1.0f);

	status = 1;
}

//Initialiser function
void PointLight::init(glm::vec3 const & position, glm::vec3 const & color) {
	PointLight::position = position;
	PointLight::color = color;

	/*
		Value Table
		Distance	Constant	Linear	Quadratic
		7			1.0			0.7		1.8
		13			1.0			0.35	0.44
		20			1.0			0.22	0.20
		32			1.0			0.14	0.07
		50			1.0			0.09	0.032
		65			1.0			0.07	0.017
		100			1.0			0.045	0.0075
		160			1.0			0.027	0.0028
		200			1.0			0.022	0.0019
		325			1.0			0.014	0.0007
		600			1.0			0.007	0.0002
		3250		1.0			0.0014	0.000007
		*/
	PointLight::constant = 1.0f;
	PointLight::linear_v = 0.0014f;
	PointLight::quadratic = 0.000007f;

	PointLight::ambient = glm::vec3(0.25f);
	PointLight::diffuse = glm::vec3(1.0f);
	PointLight::specular = glm::vec3(1.0f);

	PointLight::status = 1;
}
