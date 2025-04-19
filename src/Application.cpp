#include "Application.h"

Application::Application(int width, int height, const char *title)
{
    this->width = width;
    this->height = height;
    this->title = title;
    
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(1);
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(1);
    }

    glfwMakeContextCurrent(window);
    
    if(!glewInit() == GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwDestroyWindow(window);
        exit(1);
    }

    ball1 = std::make_unique<Ball>(glm::vec3(-0.5f, 0.5f, 0.0f), 0.1f, 1.0f);

}

Application::~Application()
{
    glfwTerminate();
    glfwDestroyWindow(window);
    exit(0);
}

void Application::run()
{
    Renderer Renderer(width, height);
    
    glfwSwapInterval(1);
    glViewport(0, 0, width, height);
    
    double LastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window)) {
        double CurrentTime = glfwGetTime();
        float deltaTime = static_cast<float>(CurrentTime - LastTime);
        LastTime = CurrentTime;

        Renderer.render(window, *ball1, deltaTime);
        
        handleInput();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::handleInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        ball1->acceleration.x = ball1->getGravity();
    } else {
        ball1->acceleration.x = 0.0f;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        ball1->acceleration.x = -ball1->getGravity();;
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        ball1->acceleration.y = ball1->getGravity();
    } else {
        ball1->acceleration.y = -ball1->getGravity();
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        ball1->acceleration.y = -ball1->getGravity();;
    }
}
