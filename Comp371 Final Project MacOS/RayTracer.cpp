//
//  RayTracer.cpp
//  Comp371 Final Project MacOS
//
//  Created by Bruno Robert on 16/11/2018.
//  Copyright © 2018 Bruno Robert. All rights reserved.
//

#include "RayTracer.hpp"

//constructor
RayTracer::RayTracer(Scene * scene, int width, std::string outputPath) {
    this->scene = scene;
    this->width = width;
    height = width / scene->cameraArray[0].aspectRatio;
    this-> outputPath = outputPath;
}

//deconsctructor
RayTracer::~RayTracer() {
    //TODO: make sure scene is deleted properly
}

/*
 Traces a ray the passed pixel (origin + direction) and returns the color of that pixel.
 Returns the Color of the object the ray hits
 If the ray doesn't intersect, returns the background color
*/
glm::vec3 RayTracer::trace(glm::vec3 &origin, glm::vec3 &pixelPos, int depth) {
    glm::vec3 color = glm::vec3(0.0f);//backgroud color
    SceneObject * closestObject = nullptr;//the closest object
    float iClosest = INFINITY; //the closes distance to an object
    
    glm::vec3 direction = pixelPos - origin;//direction of the ray
    direction = glm::normalize(direction);
    
    float i1 = INFINITY, i2 = INFINITY;//intersection points (i1 is the closest one i2 is the optional further intersection)
    
    //for each object in teh scene we test for intersection
    for(int i = 0; i < scene->sObjArray.size(); i++) {
        if (scene->sObjArray[i]->intersect(origin, direction, i1, i2)) {//if there is an intersection
            if (i1 < 0) i1 = i2; //if the first intersection is behind us (we are in the sphere)
            if (i1 < iClosest) {//if this intersection is closer than the previous one
                iClosest = i1;//set the distance to intersection
                closestObject = scene->sObjArray[i];//set the closest object
            }
        }
    }
    

    // if there are no intersections return the background color
    if (!closestObject) return color;
    
    
    glm::vec3 intersection = origin + (direction * iClosest);//position of the intersection
    glm::vec3 intersectionNormal = closestObject->getNormal(intersection);//normal at intersection
    for(int j = 0; j < scene->lightArray.size(); j++) {//we loop through all the lights
        Light * currentLight = &(scene->lightArray[j]);
        float isLit = 1;//the light hits the object by default
        glm::vec3 lightDirection = currentLight->position - intersection;//from intersection to light
        lightDirection = glm::normalize(lightDirection);
        
        /*
         "The path of the righteous man is beset on all sides by the inequities of the selfish and the
         tyranny of evil men. Blessed is he who, in the name of charity and good will, shepherds the weak
         through the valley of darkness, for he is truly his brother's keeper and the finder of lost
         children."
         */
        for(int i = 0; i < scene->sObjArray.size(); i++) {//check for intersection with shadow ray
            if (scene->sObjArray[i]->intersect(intersection, lightDirection, i1, i2)) {
                isLit = 0;//the light doesn't hit the object since the shadow ray intersects an object
                break;
            }
        }
        
        glm::vec3 viewDirection = origin - intersection;//from the ray intersection to the camera(origin)
        viewDirection = glm::normalize(viewDirection);
        
        //checking the direction of the normal
        //normally if the file is good this should change anything
        if(glm::dot(viewDirection, intersectionNormal) <= 0) {
            //"And you will know my name is the Lord when I lay my vengeance upon thee."
            intersectionNormal = -intersectionNormal;
        }
        
        //the relection of the light with respects to the normal vector
        glm::vec3 reflectedLight = glm::reflect(-lightDirection, intersectionNormal);
        reflectedLight = glm::normalize(reflectedLight);
        
        //checking the direction of the reflected light
        if(glm::dot(reflectedLight, intersectionNormal) <= 0) {
            //"And you will know my name is the Lord when I lay my vengeance upon thee."
            reflectedLight = -reflectedLight;
        }
        
        //ambient component
        glm::vec3 ambient = (closestObject->ambientColor * currentLight->ambient);
        
        //diffuse component
        glm::vec3 diffuse = isLit * ((closestObject->diffuseColor * currentLight->diffuse) * (float)fmax((float)glm::dot(intersectionNormal, lightDirection), 0.0f));
        
        //specular component
        glm::vec3 specular = isLit * ((closestObject->specularColor * currentLight->specular) * pow((float)fmax((float)glm::dot(viewDirection, reflectedLight), 0.0f), closestObject->shininess));
        
        //add the 3 components to the pixel color
        color = color + ambient + diffuse + specular;
    }
    //pixel color
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
    
    
    //we itterate through all pixels to trace the right amount of rays
    for(int y = 0; y < height; y++) {
        //std::cout << "this is a test" << std::endl;
        
        std::cout << "\rworking ... " << ((float)y/height)*100 << "%" << std::flush;
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
    
    //now we simply output the pixels to the image file
    std::cout << "writing to file..." << std::endl;
    // Save result to a PPM/BMP image (keep these flags if you compile under Windows)
    std::ofstream ofs(outputPath, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
        ofs << (unsigned char)(std::min(float(1), fabs(image[i].x)) * 255) <<
        (unsigned char)(std::min(float(1), fabs(image[i].y)) * 255) <<
        (unsigned char)(std::min(float(1), fabs(image[i].z)) * 255);
    }
    ofs.close();
    delete [] image;
    std::cout << "File written" << std::endl;
}
