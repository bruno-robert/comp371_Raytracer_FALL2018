//
//  RayTracer.cpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 16/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//

#include "RayTracer.hpp"

RayTracer::RayTracer(Scene * scene, int width, std::string outputPath) {
    this->scene = scene;
    this->width = width;
    height = width / scene->cameraArray[0].aspectRatio;
    this-> outputPath = outputPath;
}

RayTracer::~RayTracer() {
    //TODO: make sure scene is deleted properly
}

/*
 Traces a ray the passed pixel (origin + direction) and returns the color of that pixel.
 Returns the Color of the object the ray hits
 If the ray doesn't intersect, returns the background color
*/
glm::vec3 RayTracer::trace(glm::vec3 &origin, glm::vec3 &pixelPos, int depth) {
    glm::vec3 color = glm::vec3(0.1f);//backgroud color
    bool intersect = false;//TODO: remove this variable as it's maybe not necessary
    SceneObject * closestObject = nullptr;//the closest object
    float iClosest = INFINITY; //the closes distance to an object
    
    glm::vec3 direction = pixelPos - origin;//direction of the ray
    direction = glm::normalize(direction);
    
    float i1 = INFINITY, i2 = INFINITY;
    
    //TODO: loop through all the objects and find the closest one
    for(int i = 0; i < scene->sphereArray.size(); i++) {
        if (scene->sphereArray[i].intersect(origin, direction, i1, i2)) {
            if (i1 < 0) i1 = i2; //if the first intersection is behind us (we are in the sphere)
            if (i1 < iClosest) {//if this intersection is closer than the previous one
                iClosest = i1;
                closestObject = &scene->sphereArray[i];
                intersect = true;
            }
        }
    }
    //TODO: Create a shadow ray from closest object's intersection x number of lights
    // if there are no intersections return the background color
    if (!closestObject) return color;
    
    glm::vec3 intersection = origin + direction * iClosest;//position of the intersection
    glm::vec3 intersectionNormal = closestObject->getNormal(intersection);
    intersectionNormal = glm::normalize(intersectionNormal);
    for(int j = 0; j < scene->lightArray.size(); j++) {
        Light * currentLight = &scene->lightArray[j];
        float isLit = 1;
        glm::vec3 lightDirection = currentLight->position - intersection;//from intersection to light
        lightDirection = glm::normalize(lightDirection);
        
        for(int i = 0; i < scene->sphereArray.size(); i++) {
            if (scene->sphereArray[i].intersect(intersection, lightDirection, i1, i2)) {
                isLit = 0;
                break;
            }
        }
        glm::vec3 reflectedLight = glm::reflect(lightDirection, intersectionNormal);
        reflectedLight = glm::normalize(reflectedLight);
        
        glm::vec3 viewDirection = origin - intersection;
        viewDirection = glm::normalize(viewDirection);
        
        //TODO: specular
        color += isLit * ((closestObject->ambientColor * currentLight->ambient) + //ambient
                          (closestObject->diffuseColor * glm::dot(intersectionNormal, lightDirection) * currentLight->diffuse) +
                          (closestObject->specularColor * currentLight->specular * pow(glm::dot(viewDirection, reflectedLight), closestObject->shininess)));
        
    }
    
    
    return color;
}

//Raytraces the image (computes using raytracing the values of each pixel)
void RayTracer::raytrace() {
    Camera cam = scene->cameraArray[0];//the main camera of the scene
    double ar = cam.aspectRatio;//(double)height/(double)width; //aspect ratio
    double ipd = cam.focalLength;//image plane distance
    double fov = cam.fov/2;
    
    //width of the screen in units (not pixels)
    double screenWidth = (2 * ipd * sin(fov / 2)) / cos(fov / 2);
    double screenHeight = screenWidth / ar;
    
    height = width / ar;
    
    double pixelWidth = screenWidth / width; //width of a pixel
    double pixelheigh = screenHeight / height; //height of a pixel (pixels are not necesseraly square)
    
    
    glm::vec3 *image = new glm::vec3[width * height], *pixel = image;//stores all the pixel values
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++, ++pixel) {
            //position x of the current pixel
            double posx = cam.position.x - (pixelWidth * ((width/2) - x)) + (pixelWidth/2);
            //position y of the current pixel
            double posy = cam.position.y - (pixelheigh * ((height/2) - y)) + (pixelheigh/2);
            //position z of the current pixel
            double posz = -ipd;
            
            glm::vec3 pixelPos(posx, posy, posz);//position of the pixel
            
            *pixel = trace(cam.position, pixelPos, 0);//calculate the pixel color
        }
        
        
    }
    std::cout << "writing to file..." << std::endl;
    // Save result to a PPM image (keep these flags if you compile under Windows)
    std::ofstream ofs(outputPath, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
        (unsigned char)(std::min(float(1), image[i].y) * 255) <<
        (unsigned char)(std::min(float(1), image[i].z) * 255);
    }
    ofs.close();
    delete [] image;
    std::cout << "File written" << std::endl;
}
