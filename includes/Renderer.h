#pragma once

#include "ShaderProgram.h"
#include "Ball.h"
#include "Application.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer {
private:
    ShaderProgram ShaderProgram;

    glm::mat4 projection;
    glm::mat4 view;
    float viewSize;
    int windowHeight;
    int windowWidth;
public:
    
    Renderer(int width, int height);
    ~Renderer();    

    void updateProjection(GLFWwindow* window);
    void render(GLFWwindow* window, Ball& ball1, float deltaTime);

    
};
