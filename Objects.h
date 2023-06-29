#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <Model.h>
#include <SimpleGeometry.h>



// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


class Player {
public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    float MovementSpeed = 10.0f;

    float jumpVelocity = 0.0f;
    float fallVelocity = 0.0f;
    float gravity = 9.8f * 2.0f; // Adjust this value as needed
    bool isJumping = false;
    float groundLevel = 0.0f;
    bool ground = true;
    float playerYaw=0.f;
    float speed = 0.3f;

    Player() {
    }

    void ProcessKeyboardPlayer(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        playerYaw = camera.Yaw;
        float angleRad = glm::radians(playerYaw);
        if (direction == FORWARD) {
            Position.x += cos(angleRad) * velocity * speed;
            Position.z -= sin(-angleRad) * velocity * speed;
        }
        if (direction == BACKWARD) {
            Position.x -= cos(angleRad) * velocity * speed;
            Position.z += sin(-angleRad) * velocity * speed;
        }
        if (direction == LEFT) {
            Position.x -= sin(-angleRad) * velocity * speed;
            Position.z -= cos(angleRad) * velocity * speed;
        }
        if (direction == RIGHT) {
            Position.x += sin(-angleRad) * velocity * speed;
            Position.z += cos(angleRad) * velocity * speed;
        } 
       
    }

    void Jump()
    {
        if (!isJumping) {
            jumpVelocity = 8.0f; // Adjust this value as needed
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
    }

    void walkOnGround() {
        if (Position.x > -150.5f && Position.x < 250.5f && Position.z > -150.5f && Position.z < 250.5f && Position.y > -4.6f) {
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

    void draw(Shader& shader, Model model, glm::vec3 scale) {
        model.draw(shader, glm::vec3(Position.x, Position.y, Position.z), -playerYaw - 90.f, glm::vec3(0.0f, 1.0f, 0.0f), scale);
    }
};

Player player;

class MCube : private CCube {
public: 
    
    MCube(glm::vec3 color, float shininess) {
        cubeColor = color;
        cubeShininess = shininess;
    }

    void draw(Shader& shader, glm::vec3 pos, glm::vec3 scale) {
        setMaterial(shader);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, pos);
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, scale);
        shader.setMat4("model", model);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (indices.size() * sizeof(float)) / indicesRow, GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);        
    }

private:
    glm::vec3 cubeColor = glm::vec3(0.2f, 0.6f, 0.11f);
    float cubeShininess = 32.0f;

    void setMaterial(Shader& shader) {
        shader.setVec3("material.ambient", cubeColor);
        shader.setVec3("material.diffuse", cubeColor);
        shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        shader.setFloat("material.shininess", cubeShininess);
        shader.setInt("matOrText", 1);
    }
};