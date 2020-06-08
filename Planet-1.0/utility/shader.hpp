//
//  shader.hpp
//  glinat0
//
//  Created by Shabnam Sahay on 29/03/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#ifndef shader_h
#define shader_h

#include<GL/glew.h>
#include <glm/glm.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"readin.hpp"

class Shader {
    
public:
    //the program ID
    GLuint ID;
    
    //constructor reads and builds the shader
    Shader (const char* vertexPath, const char* fragmentPath);
    
    //use/activate the shader
    void use();
    
    //utility uniform functions: query a uniform location and set its value
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    
private:
    
    void checkCompileErrors(unsigned int shader, std::string type);
    
};


#endif /* shader_h */
