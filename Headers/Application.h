#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "shaderClass.h"
#include "CameraClass.h"
#include "Model.h"
#include "Objects.h"
#include "SimpleGeometry.h"
#include "Lights.h"
#include "Text.h"
#include "Gui.h"

Camera camera(glm::vec3(-1.0f, 1.0f, -0.0f));
Player player(camera);
bool cameraSwitch = true;
bool mouseSwitch = true;
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        cameraSwitch = !cameraSwitch;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        mouseSwitch = !mouseSwitch;
}

void processInput(GLFWwindow* window) {

    if (!cameraSwitch) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            player.ProcessKeyboardPlayer(FORWARD, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            player.ProcessKeyboardPlayer(BACKWARD, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            player.ProcessKeyboardPlayer(LEFT, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            player.ProcessKeyboardPlayer(RIGHT, deltaTime);
        };
    }

    else if (cameraSwitch) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboardFree(FORWARD, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboardFree(BACKWARD, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboardFree(LEFT, deltaTime);
        };
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboardFree(RIGHT, deltaTime);
        };
    };
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (!cameraSwitch)
        camera.ProcessMouseMovement(xoffset, yoffset);
    else if (cameraSwitch)
        camera.ProcessMouseMovementFree(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

class Application {
public:
    GLFWwindow* window;
    GUI gui;

    Application() {
        window = createWindow();
        gui.init(window);
    }

    GLFWwindow* createWindow() {
        // glfw: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window creation
        GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);

        // glad: load all OpenGL function pointers
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
        }

        return window;
    }

	void init() {
        shaders.push_back(Shader("Shader/lightshader.vs", "Shader/lightShader.fs"));
        shaders.push_back(Shader("Shader/textShader.vs", "Shader/textShader.fs"));
        shaders[0].use();

        //Initialize some parameters
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_STENCIL_TEST);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

    void initObj() {
        models.push_back(Model("../models/Robot/LilRobot.obj"));
        models.push_back(Model("../models/Box/Box.obj"));
        //Model room("../models/Room/room.obj");
        
        sunLights.push_back(SunLight(camera, glm::vec3(1.f, 0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f)));
        sunLights.push_back(SunLight(camera, glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f)));
        sunLightsNumber = (int)sunLights.size();
        shaders[0].setInt("numberOfSun", sunLightsNumber);

        pointLights.push_back(PointLight(camera, glm::vec3(1.f, 0.5f, 0.5f), glm::vec3(1.f, 0.f, 0.f)));
        pointLights.push_back(PointLight(camera, glm::vec3(0.5f, 0.f, 0.5f), glm::vec3(0.f, 0.f, 1.f)));
        pointLightsNumber = (int)pointLights.size();
        shaders[0].setInt("numberOfPointLight",pointLightsNumber);
        
    }

    void loop() {
        // render loop
        while (!glfwWindowShouldClose(window))
        {
            sleep(0.01);
            // per-frame time logic
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            glfwPollEvents();

            // Start the Dear ImGui frame
            gui.loopInit();
            ImGui::Text("Camera Position %f %f %f", camera.Position.x, camera.Position.y, camera.Position.z);
            ImGui::Text("Camera Yaw %f", camera.Yaw);
            ImGui::Text("--------------------------------");
            ImGui::Text("Player Position %f %f %f", player.Position.x, player.Position.y, player.Position.z);

            // input
            processInput(window);
            glfwSetKeyCallback(window, key_callback);

            // render
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            if (cameraSwitch == false) {
                player.Update(deltaTime);
                camera.Update(deltaTime, player.Position);
            }

            if (mouseSwitch)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            else if (!mouseSwitch)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            view = camera.GetViewMatrix();
            projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            shaders[0].setMat4("view", view);
            shaders[0].setMat4("projection", projection);
            
            //Lights
            ImGui::Text("--------------------------------");
            pointLights[0].pos.x = sin(glfwGetTime()) * 1.f;
            for (int i = 0; i < sunLightsNumber; i++) {
                ImGui::Text("SunLight %i color %f %f %f", i, sunLights[i].color.x, sunLights[i].color.y, sunLights[i].color.z);
                ImGui::Text("SunLight %i rotation %f %f %f", i, sunLights[i].rot.x, sunLights[i].rot.y, sunLights[i].rot.z);
                sunLights[i].draw(shaders[0], i);
            }
            for (int i = 0; i < pointLightsNumber; i++) {
                ImGui::Text("PointLight %i color %f %f %f", i, pointLights[i].color.x, pointLights[i].color.y, pointLights[i].color.z);
                ImGui::Text("PointLight %i rotation %f %f %f", i, pointLights[i].pos.x, pointLights[i].pos.y, pointLights[i].pos.z);
                pointLights[i].draw(shaders[0], i);
            }
            
            //Player
            player.draw(shaders[0], models[0], glm::vec3(0.05f, 0.05f, 0.05f));

            //Objects
            for (int i = 1; i < models.size(); i++) {
                models[i].draw(shaders[0], glm::vec3(1.f, 1.f, 1.f), 0.f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
            }

            //End
            gui.render();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
        }
    }

    void cleanup() {
        // glfw: terminate, clearing all previously allocated GLFW resources.
        gui.end();
        glfwTerminate();
    }

private:
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    std::vector<Model> models;
    std::vector<SunLight> sunLights;
    int sunLightsNumber = 0;
    std::vector<PointLight> pointLights;
    int pointLightsNumber = 0;
    std::vector<Shader> shaders;
};