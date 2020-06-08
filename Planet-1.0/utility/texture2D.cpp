//
//  texture.cpp
//  glinat0
//
//  Created by Shabnam Sahay on 03/04/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include"texture2D.hpp"

Texture::Texture (const char* filePath)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    //set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    //set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    //load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //tell stbi_image.h to flip loaded textures vertically
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    
    int num = 0;
    while(true) {
        if((filePath[num])!='\0') {
            num = num + 1;
        }
        else break;
    }
    
    GLint internalformat;
    GLenum format;
    
    std::string extension = "";
    for (int i = num-3; i<num; i++) {
        extension = extension + filePath[i];
    }

    if (extension == "png") {
        internalformat = GL_RGBA;
        format = GL_RGBA;
    }
    else {
        internalformat = GL_RGB;
        format = GL_RGB;
    }
    
    
    
    //now that texture is bound, generating the texture using image data
    if(data)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        //generate all required mipmaps for currently bound texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    //free the image memory now
    stbi_image_free(data);
    
}

void Texture::use(int unit)
{
    glActiveTexture(GL_TEXTURE0+unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(texture, name.c_str()), (int)value);
}

void Texture::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(texture, name.c_str()), value);
}

void Texture::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(texture, name.c_str()), value);
}
