#include "SceneLoader.h"

//----helper functions----//

static bool sameSide(const glm::vec3 &p1 ,const glm::vec3 &p2, const glm::vec3 &a, const glm::vec3 &b) {
    glm::vec3 cp1 = glm::cross(b-a, p1-a);
    glm::vec3 cp2 = glm::cross(b-a, p2-a);
    if (glm::dot(cp1, cp2) >= 0 ) return true;
    else return false;
}


//getLineValue checks the line prefix for validity and then sets the given variable with the line's value
//multiple overloads for different variables types
//TODO: get these to throw exeptions
static bool getLineValue(std::string str, glm::vec3 &vec, const std::string &prefix) {
    std::istringstream iss(str);
    std::string word;
    while(iss >> word) {
        if(word == prefix || word == (prefix + ":")) {//FIXME: this doesn't check if the word already has the ":" and ths could accept words of type "word::" (same for other getLineValue() methods
            iss >> word;
            vec.x = std::stof(word);
            iss >> word;
            vec.y = std::stof(word);
            iss >> word;
            vec.z = std::stof(word);
            return 1;
        } else {
            std::cout << "prefix not recognised: " << word << std::endl;
            return 0;
        }
    }
    return 0;
}

static bool getLineValue(std::string str ,int *val, const std::string &prefix) {
    std::istringstream iss(str);
    std::string word;
    while(iss >> word) {
        if(word == prefix || word == (prefix + ":")) {
            iss >> word;
            *val = std::stoi(word);
            return 1;
        } else {
            std::cout << "prefix not recognised: " << word << std::endl;
            return 0;
        }
    }
    return 0;
    
}

static bool getLineValue(std::string str ,float *val, const std::string &prefix) {
    std::istringstream iss(str);
    std::string word;
    while(iss >> word) {
        if(word == prefix || word == (prefix + ":")) {
            iss >> word;
            *val = std::stof(word);
            return 1;
        } else {
            std::cout << "prefix not recognised: " << word << std::endl;
            return 0;
        }
    }
    return 0;
}

static bool getLineValue(std::string str, char * path, const std::string &prefix) {
    std::istringstream iss(str);
    std::string word;
    if(prefix == "") {//if there is no prefix
        while(iss >> word) {
            strcpy(path, word.c_str());
            return 1;
        }
    }
    
    
    while(iss >> word) {
        if(word == prefix || word == (prefix + ":")) {
            iss >> word;
            strcpy(path, word.c_str());
            return 1;
        } else {
            std::cout << "prefix not recognised: " << word << std::endl;
            return 0;
        }
    }
    return 0;
}

//----Constructors for helper classes----//
SceneObject::SceneObject() {
    
}

SceneObject::SceneObject(const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess) {
    this->ambientColor = ambientColor;
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->shininess = shininess;
}

bool SceneObject::intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2) {
    std::cout << "This is the base class, this isn't right!" << std::endl;
    return false;
}
glm::vec3 SceneObject::getNormal(const glm::vec3 &intersection) {
    std::cout << "This is the base class, this isn't right!" << std::endl;
    glm::vec3 a = glm::vec3(0.0f);
    return a;
}


Camera::Camera(glm::vec3 position, int fov, int focalLength, float aspectRatio) {
    this->position = position;
    this->fov = fov;
    this->focalLength = focalLength;
    this->aspectRatio = aspectRatio;
}

Plane::Plane(const glm::vec3 &normal, const glm::vec3 &position,
             const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
             const glm::vec3 &specularColor, float shininess) : SceneObject(ambientColor, diffuseColor, specularColor, shininess)
{
    this->normal = normal;
    this->position = position;
}

bool Plane::intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2) {
    
    float denom = glm::dot(direction, normal);
    // we use abs() because a hit is a hit independent of the direction of hte plane normal
    if (std::abs(denom) > 0.0001f)
    {
        float t = glm::dot((position - origin), normal) / denom;
        if (t >= 0) {
            i1 = i2 = t;//there is only one intersection so just set to be equal
            return true; // you might want to allow an epsilon here too
        }
    }
    return false;
    
}

glm::vec3 Plane::getNormal(const glm::vec3 &intersection) {
    return this->normal;
}

Sphere::Sphere(const glm::vec3 &position, float radius,
               const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor,
               const glm::vec3 &specularColor, float shininess){
    this->ambientColor = ambientColor;
    this->diffuseColor = diffuseColor;
    this->specularColor = specularColor;
    this->position = position;
    this->radius = radius;
    this->shininess = shininess;
}

/*
 Returns true if there is an intersection between the given line and the sphere i1 and i2 are the two intersection points. We usually want to use the closest one.
 */
bool Sphere::intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2) {
    float radius2 = radius * radius;//radius squared
    glm::vec3 l = position - origin;//(¨L)line from the origin (camera) to the center of the sphere
    float tca = glm::dot(l, direction);//(tc)length of line that goes from origin to between the 2 intersections
    if (tca < 0) return false;//return false
    float d2 = glm::dot(l, l) - tca * tca;//sqrt(d2) = the distance between position and origin + direction * infinity
    if (d2 > radius2) return false;//the line doesn't intersect
    float thc = sqrt(radius2 - d2);//half the distance between the points
    
    //first and seccond solution of the equation (the two intersection points)
    i1 = tca - thc;
    i2 = tca + thc;
    //kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html explais this quite well although the naming is different
    return true;
    
    
}

glm::vec3 Sphere::getNormal(const glm::vec3 &intersection) {
    glm::vec3 normal = intersection - position;
    normal = glm::normalize(normal);
    return normal;//we need a normal from the center to the outside
}

Mesh::Mesh(char * path, const glm::vec3 &ambientColor,
           const glm::vec3 &diffuseColor, const glm::vec3 &specularColor,
           float shininess) : SceneObject(ambientColor, diffuseColor, specularColor, shininess)
{
    this->path = path;
}

bool Mesh::intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2) {
    return false;//TODO:
}

glm::vec3 Mesh::getNormal(const glm::vec3 &intersection) {
    glm::vec3 a = glm::vec3(0.0f);
    return a;//TODO:
}

Triangle::Triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess) : SceneObject(ambientColor, diffuseColor, specularColor, shininess) {
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
}

bool Triangle::intersect(const glm::vec3 &origin, const glm::vec3 &direction, float &i1, float &i2) {
    glm::vec3 tNormal = glm::cross((v1 - v2), (v2 - v3));//vector that is normal to the triangle's plane
    tNormal = glm::normalize(tNormal);
    
    float denom = glm::dot(direction, tNormal);
    // we use abs() because a hit is a hit independent of the direction of hte plane normal
    if (std::abs(denom) > 0.0001f)
    {
        float t = glm::dot((v1 - origin), tNormal) / denom;
        if (t >= 0) {
            i1 = i2 = t;//there is only one intersection so just set to be equal
            
            glm::vec3 intersect = t * direction + origin;
            
            //We check if the point is in the triangle
            if (sameSide(v3, intersect, v1, v2) && sameSide(v2, intersect, v3, v1) && sameSide(v1, intersect, v3, v2)) {
                return true; // you might want to allow an epsilon here too
            } else {
                return false;
            }
        }
    }
    return false;
    
    
}

glm::vec3 Triangle::getNormal(const glm::vec3 &intersection) {
    glm::vec3 a = glm::vec3(0.0f);
    return a;//TODO:
}

Light::Light(const glm::vec3 &pos, const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular) {
    this->position = pos;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Scene::Scene() {
    this->path = "/Users/Bruno/Desktop/test1234.txt";//WIN: change to win path
	numberOfObjects = 0;
}

Scene::Scene(const char * path) {
	this->path = path;
	numberOfObjects = 0;
}

Scene::~Scene() {
    //TODO: delete objectArray properly
}

/*
 Reads the scene file and create the apropriate objects in the apropriate vectors(arrays)
 */
bool Scene::loadScene() {
	std::ifstream ifs;
	ifs.open(path, std::fstream::in);
	if (!ifs.is_open()) {
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
            std::cout << "the number of objects wasn't translated to an integer" << std::endl;
		}
	}
    //MARK: verbose
    if (VERBOSE) {
        std::cout << "The scene has " << numberOfObjects << " objects" << std::endl;
    }
    
    //loop through file and instanciate each object
	while (ifs.good()) {//check if ther is a line
		ifs.getline(line, LINE_SIZE);//retrieve current line
        std::string currentLine = line;
        if(currentLine == "camera" || currentLine == "camera\n" || currentLine == "camera\r\n" || currentLine == "camera\r") {
            glm::vec3 pos = glm::vec3(0.0f);//position
            int fov = 0;//field of view
            int fl = 0;//focal length
            float ar = 0;//aspectratio
            bool err = false;
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, pos, "pos:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get fov
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &fov, "fov:"))
                    err = true;
            } else {
                err = true;
            }
            if(ifs.good()) {//get fl
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &fl, "f:"))
                    err = true;
            } else {
                err = true;
            }
            if(ifs.good()) {//get ar
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &ar, "a:"))
                    err = true;
            } else {
                err = true;
            }
            if(err) {
                std::cout << "missing data in camera (or something went wrong)" << std::endl;
            } else {
                Camera camera(pos, fov, fl, ar);
                cameraArray.push_back(camera);
            }
        }
        else if(currentLine == "triangle" || currentLine == "triangle\n" || currentLine == "triangle\r\n" || currentLine == "triangle\r") {
            glm::vec3 v1;
            glm::vec3 v2;
            glm::vec3 v3;
            glm::vec3 ambientColor;
            glm::vec3 diffuseColor;
            glm::vec3 specularColor;
            float shininess = 0;
            bool err = false;
            
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, v1, "v1:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, v2, "v2"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, v3, "v3:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, ambientColor, "amb:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, diffuseColor, "dif:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, specularColor, "spe:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &shininess, "shi:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }

            if(err) {
                std::cout << "missing data in triangle (or something went wrong)" << std::endl;
            } else {
                //create triangle and push back
                Triangle *triangle = new Triangle(v1, v2, v3, ambientColor, diffuseColor, specularColor, shininess);
                triangleArray.push_back(*triangle);
                sObjArray.push_back(triangle);
            }
        }
        else if(currentLine == "sphere" || currentLine == "sphere\n" || currentLine == "sphere\r\n" || currentLine == "sphere\r") {
            glm::vec3 position;
            float radius = 0;
            glm::vec3 ambientColor;
            glm::vec3 diffuseColor;
            glm::vec3 specularColor;
            float shininess = 0;
            bool err = false;
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, position, "pos:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &radius, "rad:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, ambientColor, "amb:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, diffuseColor, "dif:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, specularColor, "spe:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &shininess, "shi:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(err) {
                std::cout << "missing data in triangle (or something went wrong)" << std::endl;
            } else {
                //create triangle and push back
                Sphere *sphere = new Sphere(position, radius, ambientColor, diffuseColor, specularColor, shininess);
                sphereArray.push_back(*sphere);
                sObjArray.push_back(sphere);
            }
            
        } else if(currentLine == "light" || currentLine == "light\n" || currentLine == "light\r\n" || currentLine == "light\r") {
            glm::vec3 position;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
            bool err = false;
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, position, "pos:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, ambient, "amb:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, diffuse, "dif:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, specular, "spe:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(err) {
                std::cout << "missing data in triangle (or something went wrong)" << std::endl;
            } else {
                //create triangle and push back
                Light light(position, ambient, diffuse, specular);
                lightArray.push_back(light);
            }
        } else if(currentLine == "mesh" || currentLine == "mesh\n" || currentLine == "mesh\r\n" || currentLine == "mesh\r") {
            char * path = nullptr;
            glm::vec3 ambientColor;
            glm::vec3 diffuseColor;
            glm::vec3 specularColor;
            float shininess = 0;//TODO: finish Mesh
            bool err = false;
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, path, ""))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, ambientColor, "amb:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, diffuseColor, "dif:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, specularColor, "spe:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &shininess, "shi:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(err) {
                std::cout << "missing data in mesh (or something went wrong)" << std::endl;
            } else {
                //create triangle and push back
                Mesh *mesh = new Mesh(path, ambientColor, diffuseColor, specularColor, shininess);
                meshArray.push_back(*mesh);
                sObjArray.push_back(mesh);
            }

        } else if(currentLine == "plane" || currentLine == "plane\n" || currentLine == "plane\r\n" || currentLine == "plane\r") {
            glm::vec3 normal;
            glm::vec3 position;
            glm::vec3 ambientColor;
            glm::vec3 diffuseColor;
            glm::vec3 specularColor;
            float shininess = 0.0;
            bool err = false;
            
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, normal, "nor:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, position, "pos:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, ambientColor, "amb:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, diffuseColor, "dif:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, specularColor, "spe:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &shininess, "shi:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(err) {
                std::cout << "missing data in mesh (or something went wrong)" << std::endl;
            } else {
                //create triangle and push back
                Plane *plane = new Plane(normal, position, ambientColor, diffuseColor, specularColor, shininess);
                planeArray.push_back(*plane);
                sObjArray.push_back(plane);//TODO: check the pointer problem, this might not work since the plane is saved to the stack and not the heap (same for all other objects)
            }
            
        }
	}//END while
	ifs.close();//close the filestream
	return true;
}

void Scene::setNumberOfObjects(int number) {
    this->numberOfObjects = number;
}

int Scene::getNumberOfObjects() {
    return this->numberOfObjects;
}

void Scene::getObjectInfo() {
    std::string str;
    
    std::cout << "There are: " << cameraArray.size() << " Camera Objects" << std::endl;
    
    std::cout << "There are: " << planeArray.size() << " Plane Objects" << std::endl;
    
    std::cout << "There are: " << sphereArray.size() << " Sphere Objects" << std::endl;
    
    std::cout << "There are: " << meshArray.size() << " Mesh Objects" << std::endl;
    
    std::cout << "There are: " << lightArray.size() << " Light Objects" << std::endl;
    
    std::cout << "There are: " << triangleArray.size() << " Triangle Objects" << std::endl;
    
    
}



