//
//  sphere.cpp
//  glinat0
//
//  Created by Shabnam Sahay on 31/05/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include "sphere.hpp"

Sphere::Sphere(float rad) {
    
    radius = rad;
    
    float x,y,z,xy;                           //vertex position
    float nx,ny,nz;                           //vertex normal
    float lengthInv = 1.0f/radius;
    float s, t;                               //vertex texCoords
    
    sectorStep = 2*180.0f/sectorCount;
    stackStep = 180.0f/stackCount;
    
    for (int i = 0; i<=stackCount; ++i) {
        
        stackAngle = degtorad(180.0f/2 - i*stackStep);   //going from 90 deg to -90 deg
        xy = radius*cos(stackAngle);           //r*cos(u)
        z = radius*sin(stackAngle);            //r*sin(u)
        
        //add (sectorCount+1) vertices per stack
        //the first and last vertices have same posn and normal, but different tex coords
        
        for (int j = 0; j<=sectorCount; ++j) {
            
            sectorAngle = degtorad(j*sectorStep);        //going from 0 deg to 360 deg
            
            //vertex position (x,y,z)
            x = xy*cos(sectorAngle);           // r * cos(u) * cos(v)
            y = xy*sin(sectorAngle);           // r * cos(u) * sin(v)
            
            sphereVerts.push_back(x);
            sphereVerts.push_back(y);
            sphereVerts.push_back(z);
            
            //normalized vertex normal (nx,ny,nz)
            
            nx = x*lengthInv;
            ny = y*lengthInv;
            nz = z*lengthInv;
            
            sphereVerts.push_back(nx);
            sphereVerts.push_back(ny);
            sphereVerts.push_back(nz);
            
            //vertex tex coord (s,t) range between [0,1]
            s = (float)j/sectorCount;
            t = (float)i/stackCount;
            
            sphereVerts.push_back(1-s);
            sphereVerts.push_back(t);
            
        }
        
    }
    
    
    int k1,k2;
    
    for (int i = 0; i<stackCount; ++i) {
        
        k1 = i*(sectorCount + 1);     //beginning of current stack
        k2 = k1 + sectorCount + 1;    //beginning of next stack
        
        for (int j = 0; j<sectorCount; ++j, ++k1, ++k2) {
            
            //2 triangles per sector excluding first and last stacks
            
            //k1 => k2 => k1+ 1
            if(i!=0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1+1);
            }
            
            //k1+1=>k2=>k2+1
            if (i!=(stackCount-1)) {
                indices.push_back(k1+1);
                indices.push_back(k2);
                indices.push_back(k2+1);
            }
            
        }
        
    }
    
    vertSize = sphereVerts.size();
    indSize = indices.size();
    
    //generating sphere vao vbo ebo
    
    glGenVertexArrays(1,&sphereVAO);
    glGenBuffers(1,&sphereVBO);
    glGenBuffers(1,&sphereEBO);
    
    //bind the Vertex Array Object first, then bind and set vertex buffer(s)
    glBindVertexArray(sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    
    //configure vertex attributes / copy defined vertex data into buffer's memory
    glBufferData(GL_ARRAY_BUFFER, vertSize*sizeof(float), &sphereVerts[0], GL_STATIC_DRAW);
    
    //copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    
    // normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // texture attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

float Sphere::degtorad(int degrees) {
    const float pi = 3.14159;
    return (degrees*pi)/180;
}
