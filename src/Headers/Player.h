#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Model.h"

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

    Player(Camera& icamera);

    void loadModel();

    void ProcessKeyboardPlayer(Camera_Movement direction, float deltaTime);

    void draw(Shader& shader);

private:
    Camera& camera;
};