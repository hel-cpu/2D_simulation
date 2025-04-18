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

glm::vec3 position(0.0f, 0.5f, 0.0f);
glm::vec3 velocity(0.0f, 0.0f, 0.0f); 
glm::vec3 acceleration(0.0f, -GRAVITY, 0.0f);  

const int numBalls = 2;

glm::vec3 defaultLocation[numBalls] = {
    glm::vec3(-0.5f, 0.5f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f)
};
glm::vec3 positionLocation[numBalls] = {
    glm::vec3(-0.5f, 0.5f, 0.0f),
    glm::vec3(0.5f, 0.5f, 0.0f)
};

glm::vec3 velocityLocation[numBalls] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 0.0f)
};

glm::vec3 accelerationLocation[numBalls] = {
    glm::vec3(0.0f, -GRAVITY, 0.0f),
    glm::vec3(0.0f, -GRAVITY, 0.0f)
};

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

void handleInput(GLFWwindow* window, glm::vec3 &acceleration) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        acceleration.x = GRAVITY;
    } else {
        acceleration.x = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        acceleration.x = -GRAVITY;
    }

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        acceleration.y = GRAVITY;
    } else {
        acceleration.y = -GRAVITY;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        acceleration.y = -GRAVITY;
    } 
}

void handleCollision(glm::vec3 &position, glm::vec3 &velocity) {
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
        velocity.y = -velocity.y * 0.8f; 
    }
    if (position.y >= topBound) {
        position.y = topBound;
        velocity.y = -velocity.y * 0.8f;
    }
    if (position.x >= rightBound) {
        position.x = rightBound;
        velocity.x = -velocity.x * 0.8f;
    }
    if (position.x <= leftBound) {
        position.x = leftBound;
        velocity.x = -velocity.x * 0.8f;
    }
}

void handleBallcollision(glm::vec3 &pos1, glm::vec3 &vel1, glm::vec3 &pos2, glm::vec3 &vel2, float radius) {
    float elasticity = 0.5f;
    float mass1 = 1.0f;
    float mass2 = 1.0f;

    glm::vec3 diff = pos2 - pos1;
    float distance = glm::length(diff);
    float minDistance = radius * 2.0f;

    if(distance < minDistance) {
        glm::vec3 normal = glm::normalize(diff);
        float relVelocity = glm::dot(vel2 - vel1, normal);

        if (relVelocity < 0.0f) {
            float j = -(1 + elasticity) * relVelocity;
            j /= (1 / mass1 + 1 / mass2);

            glm::vec3 impulse = j * normal;
            vel1 -= impulse / mass1;
            vel2 += impulse / mass2;

            float correction = 0.8 * (minDistance - distance) / 2.0f;
            pos1 -= correction * normal;
            pos2 += correction * normal;
        }
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
        
        float radius = 0.5f * 0.2f;

        static float lastTime = 0.0f;
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        for (int i = 0; i < numBalls; i++) {
            velocityLocation[i] += accelerationLocation[i] * deltaTime;
            positionLocation[i] += velocityLocation[i] * deltaTime + 0.5f * accelerationLocation[i] * deltaTime * deltaTime;
        }
        handleInput(window, accelerationLocation[0]);
        for (int i = 0; i < numBalls; i++) {
            handleCollision(positionLocation[i], velocityLocation[i]);
            handleBallcollision(positionLocation[0], velocityLocation[0],
                                positionLocation[1], velocityLocation[1], radius
                            );

            if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                positionLocation[i] = defaultLocation[i];
            }
        }

        ShaderProgram.use();

        glm::mat4 view = glm::mat4(1.0f);
        
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        
        glBindVertexArray(VAO);
        for(int i = 0; i < numBalls; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, positionLocation[i]);
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            
            glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE ,glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }   

    glfwTerminate();
    exit(0);
}