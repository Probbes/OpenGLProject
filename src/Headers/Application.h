#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
//#include <windows.h> //For sleep() but breaks std::min/max in generator/cellular
//#define sleep(x) Sleep(1000 * (x))

#include "shaderClass.h"
#include "CameraClass.h"
#include "Model.h"
#include "Lights.h"
#include "Gui.h"
#include "Map.h"
#include "SkyBox.h"

static Camera camera(glm::vec3(-1.0f, 1.0f, -0.0f));
static bool cameraSwitch = true; // true = free cam mvmt; false = player camera mvmt
static bool mouseSwitch = true; // true = Mouse moves camera; false = mouse cursor
const unsigned int SCR_WIDTH = 1500;
const unsigned int SCR_HEIGHT = 900;
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f;	// time between current frame and last frame
static float lastFrame = 0.0f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void processInput(GLFWwindow* window, Player& player, Map& map);

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

class Application {
public:
    GLFWwindow* window;
    GUI gui;

    Application(GLFWwindow* iwindow);

    void init();

    void initObj();

    void loop();

    void cleanup();

private:
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    std::vector<Model> models;
    std::vector<SunLight> sunLights;
    int sunLightsNumber = 0;
    std::vector<PointLight> pointLights;
    int pointLightsNumber = 0;
    std::vector<Shader> shaders;

    Map map{ camera };
    Skybox skybox{};
};