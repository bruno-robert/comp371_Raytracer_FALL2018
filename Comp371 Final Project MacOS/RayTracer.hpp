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

class RayTracer {
public:
    RayTracer(SceneObj * scene);
    ~RayTracer();
private:
    SceneObj Scene;
    int width, height;//width and heigh of the image
    
};


#endif /* RayTracer_hpp */
