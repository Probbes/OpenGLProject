#include "Player.h"

Player::Player(Camera& icamera) : camera(icamera) {
	loadModel();
}

void Player::loadModel() {
	models.push_back(Model("../assets/models/Robot/LilRobot.obj", glm::vec3(1.f, 1.f, 1.f), 0.f, glm::vec3(0.f, 1.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
}

void Player::ProcessKeyboardPlayer(Camera_Movement direction, float deltaTime, float height)
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

void Player::draw(Shader& shader) {
	shader.use();
	//model.draw(shader, glm::vec3(Position.x, Position.y, Position.z), -playerYaw - 90.f, glm::vec3(0.0f, 1.0f, 0.0f), scale);
	models[0].scale = glm::vec3(0.05f, 0.05, 0.05f);
	models[0].pos = glm::vec3(Position.x, Position.y, Position.z);
	models[0].rotation = -playerYaw - 90.f;
	models[0].draw(shader);
}