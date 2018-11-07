#include "SceneLoader.h"

SceneObj::SceneObj() {
    this->path = "/Users/Bruno/Desktop/test1234.txt";//WIN: change to win path
	numberOfObjects = 0;
}

SceneObj::SceneObj(const char * path) {
	this->path = path;
	numberOfObjects = 0;
}

SceneObj::~SceneObj() {
    //TODO: delete objectArray properly
}

bool SceneObj::loadScene() {
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
        //std::cout << line << std::endl;//MARK: removable
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
                std::cout << "missing data in camera (something went wrong)" << std::endl;
            } else {
                Camera Camera(pos, fov, fl, ar);
                cameraArray.push_back(Camera);
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
            float shininess;
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
            
            if(err) {
                //TODO: code
            } else {
                //TODO: code
            }
        }
	}//END while
    
    
	ifs.close();//close the filestream

	return true;
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
