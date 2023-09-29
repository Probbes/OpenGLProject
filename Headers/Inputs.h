#pragma once

#include <Objects.h>

bool cameraSwitch = true;
bool mouseSwitch = true;

// camera
//Camera camera; // (glm::vec3(0.0f, 0.0f, 3.0f));
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
    if (!cameraSwitch) {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            player.Jump();
    };
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        mouseSwitch = !mouseSwitch;
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window){

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
        camera.ProcessMouseMovement( xoffset, yoffset);
    else if (cameraSwitch)
        camera.ProcessMouseMovementFree(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}