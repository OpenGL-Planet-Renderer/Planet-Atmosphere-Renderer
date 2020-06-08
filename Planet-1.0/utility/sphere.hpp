//
//  sphere.hpp
//
//  Created by Shabnam Sahay on 31/05/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#ifndef sphere_h
#define sphere_h

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <cmath>

class Sphere {
    
    int sectorCount = 90;
    int stackCount = 90;
    
    float degtorad(int degrees);
    
public:
    
    float radius;
    
    Sphere(float rad);
    
    float sectorStep, stackStep;
    float sectorAngle, stackAngle;
    
    std::vector<float> sphereVerts;
    std::vector<unsigned int> indices;
    
    int vertSize;
    int indSize;
    
    GLuint sphereVAO, sphereVBO, sphereEBO;
    
};

#endif /* sphere_h */
