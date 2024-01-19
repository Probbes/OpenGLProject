#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Model.h"
#include "SimpleGeometry.h"

class Player {
public:
    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
    float MovementSpeed = 100.0f;

    float jumpVelocity = 0.0f;
    float fallVelocity = 0.0f;
    float gravity = 9.8f * 2.0f; // Adjust this value as needed
    bool isJumping = false;
    float groundLevel = 0.0f;
    bool ground = true;
    float playerYaw=0.f;
    float speed = 0.3f;
    std::vector<Model> models;

    Player(Camera& icamera) : camera(icamera) {
        loadModel();
    }

    void loadModel() {
        models.push_back(Model("../assets/models/Robot/LilRobot.obj", glm::vec3(1.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
    }

    void ProcessKeyboardPlayer(Camera_Movement direction, float deltaTime, float height)
    {
        float velocity = MovementSpeed * deltaTime;
        playerYaw = camera.Yaw;
        //std::cout << playerYaw << std::endl;
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
        Position.y = height;
    }

    void draw(Shader& shader) {
        shader.use();
        //model.draw(shader, glm::vec3(Position.x, Position.y, Position.z), -playerYaw - 90.f, glm::vec3(0.0f, 1.0f, 0.0f), scale);
        models[0].scale = glm::vec3(0.05f, 0.05, 0.05f);
        models[0].pos = glm::vec3(Position.x, Position.y, Position.z);
        models[0].rotation = -playerYaw - 90.f;
        models[0].draw(shader);
    }
private:
    Camera& camera;
};

//Player player;

class MCube : private CCube {
public: 
    glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
    MCube(glm::vec3 color, float shininess) : cubeColor(color), cubeShininess(shininess) {}

    void draw(Shader& shader) {
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