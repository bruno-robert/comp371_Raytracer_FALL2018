#include "SceneLoader.h"

SceneObj::SceneObj() {
    this->path = "/Users/Bruno/Desktop/test1234.txt";//WIN: change to win path
	numberOfObjects = 0;
}

SceneObj::SceneObj(const char * path) {
	this->path = path;
	numberOfObjects = 0;
}

bool SceneObj::loadScene() {
	
	//loop through file and instanciate each object
	std::ifstream ifs;
	ifs.open(path, std::fstream::in);
	if (!ifs.is_open())
	{
		std::cout << "Error - cannot open the file." << std::endl;
		return 0;
	}

	const int LINE_SIZE = 256;//maximum size of a line
	char line[LINE_SIZE];//stores teh current line

	//Read first line and create the Proper number of Objects
	if (ifs.good()) {
		ifs.getline(line, LINE_SIZE);
		try {
			numberOfObjects = std::stoi(line);
		}
		catch (std::exception const &e) {
		}
		
	}

    std::cout << "The scene has " << numberOfObjects << " objects" << std::endl;

	while (ifs.good()) {//check if ther is a line
		ifs.getline(line, LINE_SIZE);//retrieve current line
		//std::cout << line << std::endl;
	}
	ifs.close();//close the filestream

	return true;
}


//----Constructors for helper classes----//
SceneObject::SceneObject() {

}

Camera::Camera(glm::vec3 position, int fov, int focalLength, float aspectRatio) {
	this->position = position;
	this->fov = fov;
	this->focalLength = focalLength;
	this->aspectRatio = aspectRatio;
}

Plane::Plane(const glm::vec3 &normal, const glm::vec3 &position,
	const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
	const glm::vec3 &specularColor, float shininess) 
{
	this->normal = normal;
	this->position = position;
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor;
	this->shininess = shininess;
}

Sphere::Sphere(const glm::vec3 &position, float radius,
	const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
	const glm::vec3 &specularColor, float shininess) 
{
	this->position = position;
	this->radius = radius;
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor;
	this->shininess = shininess;
}

Mesh::Mesh(const char * path, const glm::vec3 &ambientColor,
	const glm::vec3 &diffuseColor, const glm::vec3 &specularColor,
	float shininess) 
{
	this->path = path;
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor; 
	this->shininess = shininess;
}

Light::Light(const glm::vec3 &posisiton, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) {
	this->position = position;
	this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}
