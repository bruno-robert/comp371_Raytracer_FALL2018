#include "SceneLoader.h"

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
                if(!getLineValue(line, &pos, "pos:"))//get the value
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
            //MARK: verbose
            if (VERBOSE) {
                std::cout << "Camera " << cameraArray.size() << " set!" << std::endl;//FIXME: this doesn't work if it fails, only when it doesn't
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
                if(!getLineValue(line, &v1, "v1:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &v2, "v2"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &v3, "v3:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &ambientColor, "amb:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &diffuseColor, "dif:"))//get the value
                    err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &specularColor, "spe:"))//get the value
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
                Triangle triangle(v1, v2, v3, ambientColor, diffuseColor, specularColor, shininess);
                triangleArray.push_back(triangle);
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
                if(!getLineValue(line, &position, "pos:"))//get the value
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
                if(!getLineValue(line, &ambientColor, "amb:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &diffuseColor, "dif:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &specularColor, "spe:"))//get the value
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
                Sphere sphere(position, radius, ambientColor, diffuseColor, specularColor, shininess);
                sphereArray.push_back(sphere);
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
                if(!getLineValue(line, &position, "pos:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &ambient, "amb:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &diffuse, "dif:"))//get the value
                err = true;//if something went wrong set err to true
            } else {
                err = true;
            }
            if(ifs.good()) {//get position
                ifs.getline(line, LINE_SIZE);
                currentLine = line;
                if(!getLineValue(line, &specular, "spe:"))//get the value
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
            char * path;
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

        }
	}//END while
    
    
	ifs.close();//close the filestream

	return true;
}

void Scene::getObjectInfo() {
    std::string str;
    
    std::cout << "There are: " << cameraArray.size() << " Camera Objects\n" << std::endl;
    
    std::cout << "There are: " << planeArray.size() << " Plane Objects\n" << std::endl;
    
    std::cout << "There are: " << sphereArray.size() << " Sphere Objects\n" << std::endl;
    
    std::cout << "There are: " << meshArray.size() << " Mesh Objects\n" << std::endl;
    
    std::cout << "There are: " << lightArray.size() << " Light Objects\n" << std::endl;
    
    std::cout << "There are: " << triangleArray.size() << " Triangle Objects\n" << std::endl;
    
    
}

//----helper functions----//
static bool getLineValue(std::string str, glm::vec3 *vec, const std::string &prefix) {
    std::istringstream iss(str);
    std::string word;
    while(iss >> word) {
        if(word == prefix || word == (prefix + ":")) {//FIXME: this doesn't check if the word already has the ":" and ths could accept words of type "word::" (same for other getLineValue() methods
            iss >> word;
            vec->x = std::stoi(word);
            iss >> word;
            vec->x = std::stoi(word);
            iss >> word;
            vec->x = std::stoi(word);
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
    while(iss >> word || word == (prefix + ":")) {
        if(word == prefix) {
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
    while(iss >> word || word == (prefix + ":")) {
        if(word == prefix) {
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

/*
 Returns true if there is an intersection between the given line and the sphere i1 and i2 are the two intersection points. We usually want to use the closest one.
 */
bool Sphere::intersect(const glm::vec3 origin, const glm::vec3 direction, float &i1, float &i2) {
    float radius2 = radius * radius;//radius squared
    glm::vec3 l = position - origin;//line from the origin (camera) to the center of the sphere
    float tca = glm::dot(l, direction);
    if (tca < 0) return false;//if the scphere is behind the view plane return false of course
    float d2 = glm::dot(l, l) - tca * tca;
    if (d2 > radius2) return false;//the line doesn't intersect
    float thc = sqrt(radius2 - d2);
    
    //first and seccond solution of the equation (the two intersection points)
    i1 = tca - thc;
    i2 = tca + thc;
    
    return true;
    
}

Mesh::Mesh(char * path, const glm::vec3 &ambientColor,
	const glm::vec3 &diffuseColor, const glm::vec3 &specularColor,
	float shininess) 
{
	this->path = path;
	this->ambientColor = ambientColor;
	this->diffuseColor = diffuseColor;
	this->specularColor = specularColor; 
	this->shininess = shininess;
}

Triangle::Triangle(const glm::vec3 &v1, const glm::vec3 &v2, const glm::vec3 &v3, const glm::vec3 &ambientColor, const glm::vec3 &diffuseColor, const glm::vec3 &specularColor, float shininess) {
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
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
