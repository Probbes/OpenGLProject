#include <glm/glm.hpp>

#include "ShaderClass.h"
#include "CameraClass.h"

class Light {
public:
	Light();
};

class PointLight : public Light {
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 pos = glm::vec3(1.f, 1.f, 1.f);

	PointLight(Camera icamera, glm::vec3 icolor, glm::vec3 ipos);

	void draw(Shader& shader, unsigned int i);

private:
	Camera camera;
};

class SunLight : public Light {
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 rot = glm::vec3(1.f, 1.f, 1.f);

	SunLight(Camera icamera, glm::vec3 icolor, glm::vec3 irot);

	void draw(Shader& shader, unsigned int i);

private:
	Camera camera;
};