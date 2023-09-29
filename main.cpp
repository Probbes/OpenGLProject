#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#ifdef __unix__
# include <unistd.h>
#elif defined _WIN32
# include <windows.h>
#define sleep(x) Sleep(1000 * (x))
#endif 

#include <shaderClass.h>
#include <Inputs.h>
#include <CameraClass.h>
#include <Model.h>
#include <Objects.h>
#include <SimpleGeometry.h>
#include <Lights.h>
#include <Text.h>

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

int main()
{
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
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program and buffers program
    Shader lightShader("Shader/lightshader.vs", "Shader/lightShader.fs");
    Shader textShader("Shader/textShader.vs", "Shader/textShader.fs");

    //Initialize some parameters
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    char pathRobot[] = "../models/Robot/LilRobot.obj";
    Model robot(pathRobot);

    char pathRoom[] = "../models/Room/room.obj";
    Model room(pathRoom);

    char pathBox[] = "../models/Box/Box.obj";
    Model box(pathBox);

    Light light;

    glm::vec3 pointLightPositions[] = {
        glm::vec3(8.f, 2.f, 8.f),
        glm::vec3(0.f, 2.f, 0.f)
    };

    glm::vec3 sunLightRotations[] = {
        glm::vec3(1.5f, 2.f, 1.8f)
    };
    lightShader.use();
    lightShader.setInt("numberOfPLight", sizeof(pointLightPositions) / sizeof(pointLightPositions[0]));
    lightShader.setInt("numberOfSun", sizeof(sunLightRotations) / sizeof(sunLightRotations[0]));

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        sleep(0.01);
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

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

        lightShader.use();
        lightShader.setProjView();
        
        //Lights
        for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++) {
            light.drawPointLight(lightShader, pointLightPositions[i], i);
        }

        for (int i = 0; i < sizeof(sunLightRotations) / sizeof(sunLightRotations[0]); i++) {
            light.drawSun(lightShader, sunLightRotations[i], i);
        }

        //Player
        player.draw(lightShader, robot, glm::vec3(0.05f, 0.05f, 0.05f));

        //Objects
        room.draw(lightShader, glm::vec3(0.f, -0.05f, 0.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
        //box.draw(lightShader, glm::vec3(1.0f, 0.f, 0.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f));
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}