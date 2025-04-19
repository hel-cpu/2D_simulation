#pragma once

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class Mesh {
private:
    GLuint VAO, VBO;
    int vertexCount;
    GLenum drawMode;
    bool initialized;

    std::vector<glm::vec3> vertices;

public:
    Mesh();
    ~Mesh();

    void createCircle(float radius, int vCount);

    void initializeBuffers();
    void updateBuffers();

    void bind();
    void draw();
    void unbind();

    void cleanup();
};