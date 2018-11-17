//
//  RayTracer.hpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 16/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//

#ifndef RayTracer_hpp
#define RayTracer_hpp

#include <stdio.h>
#include "SceneLoader.h"
#include "glm.hpp"//glm

#if defined __linux__ || defined __APPLE__
// "Compiled for Linux
#else
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793
#define INFINITY 1e8
#endif

class RayTracer {
public:
    RayTracer(Scene * scene, int width, int height);
    ~RayTracer();
    glm::vec3 trace(glm::vec3 &origin, glm::vec3 &pixelPos, int depth);
    void raytrace();
private:
    Scene * scene;//scene object containing all the information of the scene
    int width, height;//width and heigh of the image (i.e. resolution)
};


#endif /* RayTracer_hpp */
