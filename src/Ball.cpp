#include "Ball.h"

Ball::Ball(glm::vec3 initialPos, float radius, float mass)
{
    this->position = initialPos;
    this->velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    this->acceleration = glm::vec3(0.0f, gravity, 0.0f);
    this->radius = radius;
    this->mass = mass;
}

Ball::~Ball()
{
    cleanup();
}

void Ball::update(float deltaTime) {
    this->velocity += this->acceleration * deltaTime;
    this->position += this->velocity * deltaTime + 0.5f * this->acceleration * deltaTime * deltaTime;
}


void Ball::handleCollision(int &width, int &height, float &viewSize)
{
    float aspect = static_cast<float>(width) / static_cast<float>(height);  
    // Calculate effective bounds based on current aspect ratio
    
    float radius = this->radius;
    float rightBound = viewSize * aspect - radius;
    float leftBound = -rightBound;
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

void Ball::handleBallCollision(Ball &otherBall)
{
    glm::vec3 diff = this->position - otherBall.position;
    float distance = glm::length(diff);
    float minDistance = this->radius + otherBall.radius;

    if (distance < minDistance) {
        glm::vec3 normal = glm::normalize(otherBall.position - this->position);
        float relVelocity = glm::dot(otherBall.velocity - this->velocity, normal);

        if (relVelocity < 0.0f) {
            float j = -(1 + elasticity) * relVelocity;
            j /= (1 / this->mass + 1 / otherBall.mass);
            
            glm::vec3 impulse = j * normal;
            this->velocity -= impulse / this->mass;
            otherBall.velocity += impulse / otherBall.mass;

            float correction = 0.8f * (minDistance - distance) / 2.0f;
            this->position -= correction * normal;
            otherBall.position += correction * normal;
        }
    }

}