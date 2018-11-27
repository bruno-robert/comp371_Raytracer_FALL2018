//
//  main.cpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 06/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//
// With help from http://kylehalladay.com/blog/tutorial/math/2013/12/24/Ray-Sphere-Intersection.html
// With potential snipets from https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
// Used this method for the triangle intersection http://blackpawn.com/texts/pointinpoly/

#include <iostream>
#include "SceneLoader.h"
#include "RayTracer.hpp"
#include <fstream>

int main(int argc, const char * argv[]) {
    //the local folder of the scene files
    std::string sceneFolderPath = "/Users/Bruno/OneDrive - Concordia University - Canada/Documents/Concordia/Fall 2018/Comp 371/Xcode/Comp371 Final Project MacOS/Comp371 Final Project MacOS/Scene_Files/";
    std::string sceneFile = "input_scene.txt";//the scene file we want to use
    
    Scene scene(sceneFolderPath, sceneFile);//create scene object
    scene.loadScene();//load the scene from the fime
    
    scene.getObjectInfo();//optional, verbose the scene info to the console
    
    std::string outputFile = "/Users/Bruno/OneDrive - Concordia University - Canada/Documents/Concordia/Fall 2018/Comp 371/Xcode/Comp371 Final Project MacOS/Comp371 Final Project MacOS/output.bmp";//the path to the output file
    
    RayTracer rt(&scene, 1920, outputFile);//create the Raytracer and pass the scene object to it
    
    rt.raytrace();// start tracing the rays and create the image file
    
    return 0;
}
