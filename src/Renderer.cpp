#include "Renderer.h"

Renderer::Renderer(int width, int height) : ShaderProgram(SHADER_DIR "vertexShader.glsl", SHADER_DIR "fragmentShader.glsl")
{
    this->windowWidth = width;
    this->windowHeight = height;
    this->viewSize = 1.0f;
    this->projection = glm::mat4(1.0f);
    this->view = glm::mat4(1.0f);

    ShaderProgram.use();
}

Renderer::~Renderer()
{
    
}

void Renderer::updateProjection(GLFWwindow *window)
{
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    projection = glm::ortho(-viewSize * aspect, viewSize * aspect, -viewSize, viewSize, -1.0f, 1.0f);
}

void Renderer::render(GLFWwindow* window, Ball& ball1, float deltaTime)
{
    updateProjection(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.2f, 0.2f, 1.0f);

    ball1.createCircle(ball1.getRadius(), 36);
    ball1.draw();
    ball1.update(deltaTime);
    ball1.handleCollision(windowWidth, windowHeight, viewSize);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, ball1.getPosition());

    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}
