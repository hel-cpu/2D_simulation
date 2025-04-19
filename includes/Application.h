#pragma once

#include <iostream>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "ShaderProgram.h"
#include "Renderer.h"
#include "Ball.h" 

class Application {
private:
    GLFWwindow* window;
    std::unique_ptr<Ball> ball1;
    int width, height;
    const char* title;

public:
    Application(int width, int height, const char* title);
    ~Application();

    void handleInput();
    void run();
};  