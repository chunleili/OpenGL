#pragma once

#include <glad/glad.h> // GLAD
#include <GLFW/glfw3.h>// GLFW
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
    unsigned int m_id; //shaderProgram id

    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    ~Shader();

    void use() const;

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setVec4(const std::string& name, glm::vec4 value) const;

    std::string parseShader(const std::string& filepath);
    unsigned int compileShader(const std::string & source, const unsigned int type);

    unsigned int getId() const { return m_id; }
    
};

