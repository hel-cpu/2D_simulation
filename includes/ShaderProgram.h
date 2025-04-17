#pragma once

#include <gl/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class ShaderProgram{
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath);
    
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

private:
    void checkCompileErrors(GLuint shader, std::string type);

public:
    GLuint ID;
};