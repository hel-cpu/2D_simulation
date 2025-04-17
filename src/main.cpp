#include <iostream>
#include <vector>
#include <cmath>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 1200
#define GRAVITY 9.81f

bool isFullscreen = false;

glm::vec3 position(0.0f, 0.5f, 0.0f);
glm::vec3 velocity(0.0f, 0.0f, 0.0f); 
glm::vec3 acceleration(GRAVITY, -GRAVITY, 0.0f);  

std::vector<glm::vec3> vertices;

glm::mat4 projection = glm::mat4(1.0f);
float viewSize = 1.0f;

void updateProjection(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::ortho(-viewSize * aspect, viewSize * aspect, -viewSize, viewSize, -1.0f, 1.0f);
}

void buildCircle(float radius, int vCount){
    float angle = 360.0f / vCount;

    int triangleCount = vCount - 2;
    std::vector<glm::vec3> temp;

    for (int i = 0; i < vCount; i++) {
        float currentAngle = angle * i;
        float x = radius * cos(glm::radians(currentAngle));
        float y = radius * sin(glm::radians(currentAngle));
        float z = 0.0f;

        temp.push_back(glm::vec3(x, y, z));
    }

    for (int i = 0; i < triangleCount; i++) {
        vertices.push_back(temp[0]);
        vertices.push_back(temp[i + 1]);
        vertices.push_back(temp[i + 2]);
    }
}

void frameBuffer_call_back(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void handleCollision() {
    int width, height;

    glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);  
    // Calculate effective bounds based on current aspect ratio
    
    float radius = 0.5f * 0.2f;
    float rightBound = viewSize * aspect - radius;
    float leftBound = -rightBound + radius;
    float topBound = viewSize - radius;
    float bottomBound = -viewSize + radius;
    
    // Collision detection with adjusted bounds
    if (position.y <= bottomBound) {
        position.y = bottomBound;
        velocity.y = -velocity.y; 
    }
    if (position.y >= topBound) {
        position.y = topBound;
        velocity.y = -velocity.y;
    }
    if (position.x >= rightBound) {
        position.x = rightBound;
        velocity.x = -velocity.x;
    }
    if (position.x <= leftBound) {
        position.x = leftBound;
        velocity.x = -velocity.x;
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Circle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Faild to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        exit(1);
    }

    buildCircle(0.5f, 36);

    ShaderProgram ShaderProgram(SHADER_DIR "vertexShader.glsl", SHADER_DIR "fragmentShader.glsl");

    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        static float lastTime = 0.0f;
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        velocity += acceleration * deltaTime;
        position += velocity * deltaTime + 0.5f * acceleration * deltaTime * deltaTime;

        handleCollision();

        ShaderProgram.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        
        model = glm::translate(model, position);
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));

        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE ,glm::value_ptr(model));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }   

    glfwTerminate();
    exit(0);
}