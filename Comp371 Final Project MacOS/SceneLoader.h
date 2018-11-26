#pragma once

//#include <cstring>
//#include <vector>
//#include <string>
//#include <stdlib.h>
#include <iostream>//basic file operations
#include <fstream>//basic file operations
#include "glm.hpp"//glm
#include "objLoader.hpp"
#include <string>
#include <vector>
#include <sstream>

const bool VERBOSE = true;//MARK: debug verbose on/off

class Scene;

//----Helper objects----//
class SceneObject {
public:
    SceneObject();
    SceneObject(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess);
    //TODO: add color attributes
    virtual bool intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2);//TODO: implement in all the children
    virtual glm::vec3 getNormal(const glm::vec3 &intersection);//TODO: implement this in all the children
    
    
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
};

//camera object
class Camera {
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
    Plane(const glm::vec3 &normal,
          const glm::vec3 &position,
          const glm::vec3 &ambientColor,
          const glm::vec3 &diffuseColor,
          const glm::vec3 &specularColor,
          float shininess);
    bool intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2);
    glm::vec3 getNormal(const glm::vec3 &intersection);
private:
    glm::vec3 normal;
    glm::vec3 position;
};

//sphere object
class Sphere : public SceneObject {
public:
    Sphere(const glm::vec3 &position,
           float radius,
           const glm::vec3 &ambientColor,
           const glm::vec3 &diffuseColor,
           const glm::vec3 &specularColor,
           float shininess);
    bool intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2);
    glm::vec3 getNormal(const glm::vec3 &intersection);
private:
    glm::vec3 position;
    float radius;
};

//Triangle Object
class Triangle : public SceneObject {
public:
    Triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess);
    bool intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2);
    glm::vec3 getNormal(const glm::vec3 &intersection);
private:
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
};

//Mesh object
class Mesh {
public:
    Mesh(std::string homePath, std::string path, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess);
    bool gatherMesh(Scene *s);//gets the mesh data from the file declared in path
    
    glm::vec3 ambientColor;
    glm::vec3 diffuseColor;
    glm::vec3 specularColor;
    float shininess;
private:
    //std::vector<Triangle*> mTriangleArray;//The mesh is basiaclly a collection of triangles
    bool isMeshLoaded;
    std::string path;
    std::string homePath;
};

//Light Object
class Light {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    Light(const glm::vec3 &pos, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular);
    
};

//----SceneObj----//
//Scene objects load data from scene files and 
//store the data in various objects that are declared bellow
class Scene 
{
public:
	Scene(std::string homePath, std::string path);
    ~Scene();
	bool loadScene();
	void setNumberOfObjects(int number);
	int getNumberOfObjects();
    void getObjectInfo();
    void addTriangle(Triangle *t);
    //std::vector<SceneObject*> objectArray;//MARK: removable old
    std::vector<Camera> cameraArray;
    std::vector<Plane> planeArray;
    std::vector<Sphere> sphereArray;
    std::vector<Mesh> meshArray;
    std::vector<Light> lightArray;
    std::vector<Triangle> triangleArray;
    
    //this vector will contain all the objects that can render
    //thus simplifiying the process of iterating through all of them
    std::vector<SceneObject*> sObjArray;
private:
	std::string path;
    std::string homePath;
	int numberOfObjects;
};
