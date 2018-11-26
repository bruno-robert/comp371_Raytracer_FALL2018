//
//  main.cpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 06/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//
// With help from kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html
// With potential snipets from www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing

#include <iostream>
#include "SceneLoader.h"
#include "RayTracer.hpp"
#include <fstream>

int main(int argc, const char * argv[]) {
    
//    std::fstream outfile;
//    outfile.open("/Users/Bruno/Desktop/test1234.txt", std::fstream::out);//WIN: this path needs to be changed to local windows path
//    outfile << "this is a test" << std::endl;
//    outfile.close();
    std::string sceneFolderPath = "/Users/Bruno/OneDrive - Concordia University - Canada/Documents/Concordia/Fall 2018/Comp 371/Xcode/Comp371 Final Project MacOS/Comp371 Final Project MacOS/Scene_Files/";
    std::string sceneFile = "sceneMap.txt";
    
    
    Scene scene(sceneFolderPath, sceneFile);
    scene.loadScene();
    
    scene.getObjectInfo();
    
    std::string outputFile = "/Users/Bruno/OneDrive - Concordia University - Canada/Documents/Concordia/Fall 2018/Comp 371/Xcode/Comp371 Final Project MacOS/Comp371 Final Project MacOS/bruno-first-render.ppm";
    
    RayTracer rt(&scene, 1920, outputFile);
    
    rt.raytrace();

    
    return 0;
}
