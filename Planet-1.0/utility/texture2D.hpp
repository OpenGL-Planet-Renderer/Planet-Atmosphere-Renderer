//
//  texture.hpp
//  glinat0
//
//  Created by Shabnam Sahay on 03/04/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#ifndef texture_h
#define texture_h

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"readin.hpp"
#include"stb_image.h"

class Texture {
    
public:

    GLuint texture;
    
    //constructor
    Texture (const char* filePath);
    
    //use/activate the texture
    void use(int unit);
    
    //utility uniform functions: query a uniform location and set its value
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
};

#endif /* texture_h */
