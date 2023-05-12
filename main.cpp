/*
* Main function
* 
*/

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include <shaderClass.h>
#include <Inputs.h>
#include <CameraClass.h>
#include <TexturesHeader.h>
#include <Buffers.h>



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

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program and buffers program
    Shader shader("Shader/shader.vs", "Shader/shader.fs");
    Buffers buffers;
    

    //Initialize some parameters
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  -1.0f,  0.0f),
        glm::vec3(1.0f,  -1.0f,  0.0f),
        glm::vec3(0.0f,  -1.0f,  1.0f),
        glm::vec3(1.0f,  -1.0f,  1.0f),

        /*
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
        */
    };

    //Textures
    unsigned int texture1;
    unsigned int texture2;
    unsigned int texture3;

    glGenTextures(1, &texture1);
    generateTextures(texture1,"textures/grass.jpg");
    glGenTextures(1, &texture2);
    generateTextures(texture2,"textures/rock.jpg");
    glGenTextures(1, &texture3);
    generateTextures(texture3, "textures/cartoon.jpg");

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    shader.use();
    shader.setInt("texture1", 0);
    //shader.setInt("texture2", 1);

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //std::cout << glfwGetTime() << std::endl;

        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // bind textures on corresponding texture units
        //bindTextures(texture1);
        //bindTextures(texture2);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // render container
        glBindVertexArray(buffers.VAO);
        shader.use();

        shader.setProjView();

        // draw
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //for (unsigned int i = 0; i < 3; i++) {
        //    shader.multCubes(i, cubePositions);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        for (unsigned int i = 0; i < cubePositions->length() + 1; i++)
        {
            shader.multCubes(i, cubePositions);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        player.Update(deltaTime);
        camera.Update(deltaTime, player.Position);

        glBindTexture(GL_TEXTURE_2D, texture3);
        shader.shaderPlayer(player);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    buffers.cleanupBuffers();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}