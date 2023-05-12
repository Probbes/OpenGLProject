#pragma once

#include <CameraClass.h>


// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

static Camera camera(glm::vec3(-1.0f, 1.0f, -0.0f));

class Player {
public:
    glm::vec3 Position;
    float MovementSpeed = 10.0f;

    float jumpVelocity = 0.0f;
    float fallVelocity = 0.0f;
    float gravity = 9.8f * 2.0f; // Adjust this value as needed
    bool isJumping = false;
    float groundLevel = -4.5f;
    bool ground = true;

    Player(glm::vec3 position) {
        //float MovementSpeed = 1.0f;
        //glm::vec3 playerposition = {
        //    glm::vec3(Position)
        //};
        Position.x = position.x;
        Position.y = groundLevel;
        Position.z = position.z;
    }

    void ProcessKeyboardPlayer(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        float angleRad = glm::radians(camera.Yaw);
        if (direction == FORWARD) {
            Position.x += cos(angleRad) * velocity;
            Position.z -= sin(-angleRad) * velocity;
        }
        if (direction == BACKWARD) {
            Position.x -= cos(angleRad) * velocity;
            Position.z += sin(-angleRad) * velocity;
        }
        if (direction == LEFT) {
            Position.x -= sin(-angleRad) * velocity;
            Position.z -= cos(angleRad) * velocity;
        }
        if (direction == RIGHT) {
            Position.x += sin(-angleRad) * velocity;
            Position.z += cos(angleRad) * velocity;
        } 
    }

    void Jump()
    {
        if (!isJumping) {
            jumpVelocity = 12.0f; // Adjust this value as needed
            isJumping = true;
        }
    }

    // Update function (called in the game loop)
    void Update(float deltaTime)
    {
        // Apply gravity to the jump velocity
        

        walkOnGround();
        jumpVelocity -= gravity * deltaTime;

        // Check if the player has landed on the ground
        if (isJumping == true) {
            Position.y += jumpVelocity * deltaTime;
            if (Position.y <= groundLevel) {
                Position.y = groundLevel; // Snap the player to the ground level
                jumpVelocity = 0.0f; // Reset jump velocity
                isJumping = false; // Set jumping state to false
            }
        }

        if (ground == false) {
            fall();
        }
        if (Position.y < -100.0f) {
            Position = glm::vec3(0.0f, groundLevel, 0.0f);
        }
        std::cout << fallVelocity << std::endl;
    }

    void walkOnGround() {
        if (Position.x > -5.5f && Position.x < 15.5f && Position.z > -5.5f && Position.z < 15.5f && Position.y > -4.6f) {
            ground = true;
            fallVelocity = 0.0f;
        }
        else {
            ground = false;
        }
    }

    void fall() {
        fallVelocity -= gravity * deltaTime;
        Position.y += fallVelocity * deltaTime;
    }

};

Player player(glm::vec3(0.0f, -4.5f, 0.0f));

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   //0
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   //36
        //---

    /*
         -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
          0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
          0.0f,  0.5f, -0.5f, 1.0f, 1.0f,
          0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
         -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

          0.0f,  0.5f,  0.0f, 1.0f, 1.0f,
          0.0f,  0.5f, -0.5f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  -0.5f, 0.0f, 0.0f,
          0.0f,  0.5f, -0.5f, 1.0f, 0.0f,

          0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
          0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.0f,  0.0f, 0.0f,
         -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,

         -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 1.0f,

         -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
          0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
          0.0f,  0.5f, -0.5f, 0.0f, 1.0f,
          */
};

/*
unsigned int indices[] = {
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle,
};
*/