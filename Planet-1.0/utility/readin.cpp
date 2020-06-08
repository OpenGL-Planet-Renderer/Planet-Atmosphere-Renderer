//
//  readin.cpp
//
//  Created by Shabnam Sahay on 30/03/20.
//  Copyright © 2020 PAR. All rights reserved.
//

#include<fstream>
#include<string>
#include<iostream>
#include "readin.hpp"

std::string readFile(const char* filePath)
{
    std::string fileCode;
    
    try {
        std::ifstream fileIn(filePath, std::ios::in);
        
        if(!fileIn.is_open()) {
            std::cerr << "The file " << filePath << " cannot be opened." << std::endl;
            return "";
        }
        
        std::string line;
        while(!fileIn.eof()) {
            std::getline(fileIn, line);
            fileCode.append(line + "\n");
        }
        
        fileIn.close();
    }
    
    catch (std::ifstream::failure e){
        std::cout << "ERROR::FILE_NOT_READ::" << filePath << std::endl;
    }
    
    return fileCode;
}
