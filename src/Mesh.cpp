#include "Mesh.h"
#include <iostream>

Mesh::Mesh() : VBO(0), VAO(0), vertexCount(0), drawMode(GL_TRIANGLES), initialized(false) {}

Mesh::~Mesh()
{
    if (initialized)
    {
        cleanup();
    }
}

void Mesh::createCircle(float radius, int vCount)
{
    vertices.clear();
    float angle = 360.0f / vCount;

    for (int i = 0; i < vCount; i++)
    {
        vertices.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

        float angle1 = angle * (i + 1);
        float x1 = radius * cos(glm::radians(angle1));
        float y1 = radius * sin(glm::radians(angle1));
        vertices.push_back(glm::vec3(x1, y1, 0.0f));

        float angle2 = angle * (i + 2);
        float x2 = radius * cos(glm::radians(angle2));
        float y2 = radius * sin(glm::radians(angle2));
        vertices.push_back(glm::vec3(x2, y2, 0.0f));
    }
    
    vertexCount = vertices.size();

    if (initialized)
    {
        updateBuffers();
    }
    else
    {
        initializeBuffers();
    }
}

void Mesh::initializeBuffers()
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    initialized = true;
}

void Mesh::updateBuffers()
{
    if (!initialized)
    {
        initializeBuffers();
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::bind()
{
    glBindVertexArray(VAO);
}

void Mesh::draw()
{
    if (!initialized) {
        initializeBuffers();
    } 

    bind();
    glDrawArrays(drawMode, 0, vertexCount);
    unbind();
}

void Mesh::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Mesh::cleanup()
{
    if (initialized)
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        initialized = false;
    }
}
