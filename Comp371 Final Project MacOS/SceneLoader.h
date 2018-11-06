#pragma once

//#include <cstring>
//#include <vector>
//#include <string>
//#include <stdlib.h>
#include <iostream>//basic file operations
#include <fstream>//basic file operations
#include "glm.hpp"//glm
#include <string>

//Scene objects load data from scene files and 
//store the data in various objects that are declared bellow
class SceneObj 
{
public:
	SceneObj();
	SceneObj(const char * path);
	bool loadScene();
	void setNumberOfObjects(int number);
	int getNumberOfObjects();
private:
	const char * path;
	int numberOfObjects;
};

class SceneObject {
public:
	SceneObject();
};

//camera object
class Camera : public SceneObject {
public:
	glm::vec3 position;
	int fov;
	int focalLength;
	float aspectRatio;
	Camera(glm::vec3 position, int fov, int focalLength, float aspectRatio);
};

//plane object
class Plane : public SceneObject {
public:
	glm::vec3 normal;
	glm::vec3 position;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	Plane(const glm::vec3 &normal, 
		const glm::vec3 &position, 
		const glm::vec3 &ambientColor, 
		const glm::vec3 &diffuseColor, 
		const glm::vec3 &specularColor, 
		float shininess);
};

//sphere object
class Sphere : public SceneObject {
public:
	glm::vec3 position;
	float radius;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	Sphere(const glm::vec3 &position,
		float radius,
		const glm::vec3 &ambientColor,
		const glm::vec3 &diffuseColor,
		const glm::vec3 &specularColor,
		float shininess);
};

//Mesh object
class Mesh : public SceneObject {
public:
	const char * path;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	float shininess;
	Mesh(const char * path,
		const glm::vec3 &ambientColor,
		const glm::vec3 &diffuseColor,
		const glm::vec3 &specularColor,
		float shininess);
};

//Light Object
class Light : public SceneObject {
public:
    //TODO: update light to take into acount the new parameters
	glm::vec3 position;
	glm::vec3 color;
	Light(const glm::vec3 &posisiton, const glm::vec3 &color);

};
