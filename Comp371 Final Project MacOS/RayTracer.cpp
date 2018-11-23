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
    glm::vec3 color = glm::vec3(0.2f);//backgroud color
    SceneObject * closestObject = nullptr;//the closest object
    float iClosest = INFINITY; //the closes distance to an object
    
    glm::vec3 direction = pixelPos - origin;//direction of the ray
    direction = glm::normalize(direction);
    
    float i1 = INFINITY, i2 = INFINITY;
    
    //TODO: loop through all the objects and find the closest one
    for(int i = 0; i < scene->sObjArray.size(); i++) {
        if (scene->sObjArray[i]->intersect(origin, direction, i1, i2)) {
            if (i1 < 0) i1 = i2; //if the first intersection is behind us (we are in the sphere)
            if (i1 < iClosest) {//if this intersection is closer than the previous one
                iClosest = i1;
                closestObject = scene->sObjArray[i];
            }
        }
    }
    

    // if there are no intersections return the background color
    if (!closestObject) return color;
    
    glm::vec3 intersection = origin + (direction * iClosest);//position of the intersection
    glm::vec3 intersectionNormal = closestObject->getNormal(intersection);
    for(int j = 0; j < scene->lightArray.size(); j++) {
        Light * currentLight = &scene->lightArray[j];
        float isLit = 1;
        glm::vec3 lightDirection = currentLight->position - intersection;//from intersection to light
        lightDirection = glm::normalize(lightDirection);
        
        //"The path of the righteous man is beset on all sides by the inequities of the selfish and the tyranny of evil men. Blessed is he who, in the name of charity and good will, shepherds the weak through the valley of the darkness."
        for(int i = 0; i < scene->sObjArray.size(); i++) {
            if (scene->sObjArray[i]->intersect(intersection, lightDirection, i1, i2)) {
                isLit = 0;
                break;
            }
        }
        
        
        //the relection of the light with respects to the normal vector
        glm::vec3 reflectedLight = glm::reflect(lightDirection, intersectionNormal);
        reflectedLight = glm::normalize(reflectedLight);
        
        glm::vec3 viewDirection = origin - intersection;//from the ray intersection to the camera(origin)
        viewDirection = glm::normalize(viewDirection);
        
        glm::vec3 ambient = (closestObject->ambientColor * currentLight->ambient);
        glm::vec3 diffuse = isLit * (closestObject->diffuseColor * currentLight->diffuse * (float)fmax(glm::dot(intersectionNormal, lightDirection), 0));
        
        
        glm::vec3 specular = isLit * (closestObject->specularColor * currentLight->specular * pow(glm::dot(viewDirection, reflectedLight), closestObject->shininess));
        
        
        
        color += ambient + diffuse + specular;
    }
    return color;//And God said, "Let there be light," and there was light.
}

//Raytraces the image (computes using raytracing the values of each pixel)
//"At this time, humans first created machines in their own image, in effect ensuring their own demise."
//"And for a time it was good"
void RayTracer::raytrace() {
    Camera cam = scene->cameraArray[0];//the main camera of the scene
    float ar = cam.aspectRatio;//(double)height/(double)width; //aspect ratio
    float ipd = cam.focalLength;//image plane distance
    float fov = cam.fov/2;
    
    //width of the screen in units (not pixels)
    float screenWidth = (2 * ipd * sin(fov / 2)) / cos(fov / 2);
    float screenHeight = screenWidth / ar;
    
    height = width / ar;
    
    float pixelWidth = screenWidth / width; //width of a pixel
    float pixelheigh = screenHeight / height; //height of a pixel (pixels are not necesseraly square)
    
    
    glm::vec3 *image = new glm::vec3[width * height], *pixel = image;//stores all the pixel values
    
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++, ++pixel) {
            
            /*
             We start tracing from the bottom right pixel (+x, -y) because that is the way ppm files are written
             Changing the starting point and direction just flips the image without actually chainging anything.
             */
            
            //position x of the current pixel
            float posx = cam.position.x + (pixelWidth * ((width/2) - x)) + (pixelWidth/2);
            //position y of the current pixel
            float posy = cam.position.y - (pixelheigh * ((height/2) - y)) + (pixelheigh/2);
            //position z of the current pixel
            float posz = -ipd;
            
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
