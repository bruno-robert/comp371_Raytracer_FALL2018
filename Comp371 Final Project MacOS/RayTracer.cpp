//
//  RayTracer.cpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 16/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//

#include "RayTracer.hpp"

RayTracer::RayTracer(Scene * scene, int width, int height) {
    this->scene = scene;
    this->width = width;
    this->height = height;
    
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
    glm::vec3 color = glm::vec3(0.0f);
    SceneObject * closestObject;//the closest object
    float iClosest = INFINITY; //the closes distance to an object
    
    
    //TODO: loop through all the objects and find the closest one
    
    //TODO: Create a shadow ray from closest object's intersection x number of lights
    
    //TODO: compute pixel's color
    
    return color;
}

//Raytraces the image (computes using raytracing the values of each pixel)
void RayTracer::raytrace() {
    Camera cam = scene->cameraArray[0];//the main camera of the scene
    
    //TODO: for each pixel, call the trace fuction and save the pixel color
    
    //TODO: create img file and save it to disk
    
}
