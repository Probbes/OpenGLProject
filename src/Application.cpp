#include "Application.h"

Application::Application(GLFWwindow* iwindow) : window(iwindow) {
	gui.init(window);
}

void Application::init() {
	shaders.push_back(Shader("Shader/VS_lightshader.glsl", "Shader/FS_lightShader.glsl"));
	shaders.push_back(Shader("Shader/VS_mapShader.glsl", "Shader/FS_mapShader.glsl"));
	shaders.push_back(Shader("Shader/VS_WaterShader.glsl", "Shader/FS_WaterShader.glsl"));
	shaders.push_back(Shader("Shader/VS_skyboxShader.glsl", "Shader/FS_skyboxShader.glsl"));

	//Initialize some parameters
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_STENCIL_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);

	glEnable(GL_MULTISAMPLE);
}

void Application::initObj() {
	shaders[0].use();
	models.push_back(Model("../assets/models/Box/Box.obj", glm::vec3(1.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));

	sunLights.push_back(SunLight(camera, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.0f, 0.5f, 0.5f)));
	sunLightsNumber = (int)sunLights.size();
	shaders[0].setInt("numberOfSun", sunLightsNumber);
	shaders[1].use();
	shaders[1].setInt("numberOfSun", sunLightsNumber);

	//pointLights.push_back(PointLight(camera, glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 0.f, 0.f)));
	pointLightsNumber = (int)pointLights.size();
	shaders[0].setInt("numberOfPointLight", pointLightsNumber);
	//shaders[1].use();
	//shaders[1].setInt("numberOfSun", sunLightsNumber);
	//shaders[1].setInt("numberOfPointLight", pointLightsNumber);
}

void Application::loop() {
	// render loop
	while (!glfwWindowShouldClose(window))
	{
		//sleep(0.001);    //block framerate max
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();

		// Start the Dear ImGui frame
		gui.loopInit();
		// ImGui::ShowDemoWindow();
		ImGui::Text("FPS %f", 1 / deltaTime);
		ImGui::Text("--------------------------------");
		ImGui::Text("Camera Position %f %f %f", camera.Position.x, camera.Position.y, camera.Position.z);
		ImGui::Text("Camera Yaw %f", camera.Yaw);

		// input
		processInput(window, map.player, map);
		glfwSetKeyCallback(window, key_callback);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//Free cam or player cam
		if (cameraSwitch == false) {
			camera.Update(deltaTime, map.player.Position);
		}
		//Free cursor or mouse moves cam
		if (mouseSwitch)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else if (!mouseSwitch)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		//Camere & Shader WIP
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 5000.0f);

		shaders[0].use();
		shaders[0].setMat4("view", view);
		shaders[0].setMat4("projection", projection);
		shaders[1].use();
		shaders[1].setMat4("view", view);
		shaders[1].setMat4("projection", projection);
		shaders[2].use();
		shaders[2].setMat4("view", view);
		shaders[2].setMat4("projection", projection);
		shaders[3].use();
		shaders[3].setMat4("view", view);
		shaders[3].setMat4("projection", projection);

		//Lights
		for (int i = 0; i < sunLightsNumber; i++) {
			sunLights[i].draw(shaders[0], i);
		}

		//Player
		//map.player.draw(shaders[0], glm::vec3(0.05f, 0.05f, 0.05f));

		//Objects
		//for (int i = 1; i < models.size(); i++) {
		//    models[i].draw(shaders[0]);
		//}

		for (int i = 0; i < sunLightsNumber; i++) {
			sunLights[i].draw(shaders[1], i);
		}

		map.draw(shaders[1], shaders[2], shaders[0]);

		skybox.draw(shaders[3], view, projection, camera);

		//gui
		gui.render();
	}
}

void Application::cleanup() {
	// glfw: terminate, clearing all previously allocated GLFW resources.
	gui.end();
	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
		cameraSwitch = !cameraSwitch;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		mouseSwitch = !mouseSwitch;
}

void processInput(GLFWwindow* window, Player& player, Map& map) { // ZQSD

	if (!cameraSwitch) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			player.ProcessKeyboardPlayer(FORWARD, deltaTime, map.checkPosMapHeight(player.Position.x, player.Position.z));
		};
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			player.ProcessKeyboardPlayer(BACKWARD, deltaTime, map.checkPosMapHeight(player.Position.x, player.Position.z));
		};
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			player.ProcessKeyboardPlayer(LEFT, deltaTime, map.checkPosMapHeight(player.Position.x, player.Position.z));
		};
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			player.ProcessKeyboardPlayer(RIGHT, deltaTime, map.checkPosMapHeight(player.Position.x, player.Position.z));
		};
	}

	else if (cameraSwitch) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			camera.MovementSpeed = SPEED + 10;
		}
		else {
			camera.MovementSpeed = SPEED;
		};
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

	if (mouseSwitch) {
		if (!cameraSwitch)
			camera.ProcessMouseMovement(xoffset, yoffset);
		else if (cameraSwitch)
			camera.ProcessMouseMovementFree(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
