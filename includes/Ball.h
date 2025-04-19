#pragma once

#include "Mesh.h"

class Ball : public Mesh{
private:
    glm::vec3 position;
    glm::vec3 velocity;
    
    float radius;
    float mass;
    
    float gravity = 9.81f;
    float elasticity = 0.8f;
    
public:

    Ball(glm::vec3 initialPos, float radius, float mass);
    ~Ball();

    void update(float deltaTime);
    void handleCollision(int &width, int &height, float &viewSize);
    void handleBallCollision(Ball& otherBall);

    // Getters and setters
    glm::vec3 getPosition() const { return position; }
    void setPosition(const glm::vec3& pos) { position = pos; }

    glm::vec3 getVelocity() const { return velocity; }
    void setVelocity(const glm::vec3& vel) { velocity = vel; }

    glm::vec3 acceleration;
    
    float getRadius() const { return radius; }
    float getMass() const { return mass; }
    float getGravity() const { return gravity; }
};