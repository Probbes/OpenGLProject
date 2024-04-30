#include "CameraClass.h"

#include <vector>

// constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}
// constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

// processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime, glm::vec3 player)
{
	//playerPosition = glm::vec3(player.x, player.y + 4.5f, player.z) / 10.0f;
}

void Camera::ProcessKeyboardFree(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity * 5.0f;
	if (direction == BACKWARD)
		Position -= Front * velocity * 5.0f;
	if (direction == LEFT)
		Position -= Right * velocity * 5.0f;
	if (direction == RIGHT)
		Position += Right * velocity * 5.0f;
}

// processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch -= yoffset;
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (Pitch > -1.0f) {
		Pitch = -1.0f;
	}
	if (Pitch < -89.0f) {
		Pitch = -89.0f;
	}
	updateCameraVectorsFree();
}

void Camera::ProcessMouseMovementFree(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectorsFree();
}

// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	Zoom -= (float)yoffset;

	if (Zoom < 10.0f)
		Zoom = 10.0f;
	if (Zoom > 90.0f)
		Zoom = 90.0f;
}

void Camera::Update(float deltaTime, glm::vec3 player) {
	playerPosition = glm::vec3(player.x, player.y + 0.2f, player.z);

	updateCameraPosition();
	updateCameraVectors();
}

// calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors()
{
	Front = glm::normalize(playerPosition - Position);
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::updateCameraVectorsFree()
{
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::updateCameraPosition()
{
	float Theta = -Pitch;
	Position.x = playerPosition.x - 1.0f * sin(2 * M_PI * (Theta / 360)) * cos(2 * M_PI * (Yaw / 360));
	Position.y = playerPosition.y + 1.0f * cos(2 * M_PI * (Theta / 360));
	Position.z = playerPosition.z - 1.0f * sin(2 * M_PI * (Theta / 360)) * sin(2 * M_PI * (Yaw / 360));
}