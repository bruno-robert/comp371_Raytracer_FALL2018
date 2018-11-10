#pragma once

//#include <cstring>
//#include <vector>
//#include <string>
//#include <stdlib.h>
#include <iostream>//basic file operations
#include <fstream>//basic file operations
#include "glm.hpp"//glm
#include <string>
#include <vector>
#include <sstream>

const bool VERBOSE = true;//MARK: debug verbose on/off

//----Helper objects----//
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
    char * path;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    Mesh(char * path, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess);
};

class Triangle : public SceneObject {
public:
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
    Triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess);
};

//Light Object
class Light : public SceneObject {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Light(const glm::vec3 &posisiton, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    
};
//getLineValue checks the line prefix for validity and then sets the given variable with the line's value
//multiple overloads for different variables types
//TODO: get these to throw exeptions
static bool getLineValue(std::string str ,glm::vec3 *vec, const std::string &prefix);
static bool getLineValue(std::string str ,int *val, const std::string &prefix);
static bool getLineValue(std::string str ,float *val, const std::string &prefix);
static bool getLineValue(std::string str ,char * path, const std::string &prefix);

//----SceneObj----//
//Scene objects load data from scene files and 
//store the data in various objects that are declared bellow
class SceneObj 
{
public:
	SceneObj();
	SceneObj(const char * path);
    ~SceneObj();
	bool loadScene();
	void setNumberOfObjects(int number);
	int getNumberOfObjects();
    //std::vector<SceneObject*> objectArray;//MARK: removable old
    std::vector<Camera> cameraArray;
    std::vector<Plane> planeArray;
    std::vector<Sphere> sphereArray;
    std::vector<Mesh> meshArray;
    std::vector<Light> lightArray;
    std::vector<Triangle> triangleArray;
private:
	const char * path;
	int numberOfObjects;
};
