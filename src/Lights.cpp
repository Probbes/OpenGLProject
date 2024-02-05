#include "Lights.h"

Light::Light() {}

PointLight::PointLight(Camera icamera, glm::vec3 icolor, glm::vec3 ipos) : camera(icamera), color(icolor), pos(ipos) {}

void PointLight::draw(Shader& shader, unsigned int i) {
	shader.setVec3("light[" + std::to_string(i) + "].ambient", color);
	shader.setVec3("light[" + std::to_string(i) + "].diffuse", color);
	shader.setVec3("light[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
	shader.setVec3("light[" + std::to_string(i) + "].position", pos);
	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("lightPower", 1.f, 1.f, 1.f);

	shader.setFloat("light[" + std::to_string(i) + "].constant", 1.0f);
	shader.setFloat("light[" + std::to_string(i) + "].linear", 0.09f);
	shader.setFloat("light[" + std::to_string(i) + "].quadratic", 0.032f);
}



SunLight::SunLight(Camera icamera, glm::vec3 icolor, glm::vec3 irot) :camera(icamera), color(icolor), rot(irot) {}

void SunLight::draw(Shader& shader, unsigned int i) {
	shader.use();
	shader.setVec3("dirLight[" + std::to_string(i) + "].ambient", glm::vec3(0.6f, 0.6f, 1.f));
	shader.setVec3("dirLight[" + std::to_string(i) + "].diffuse", color);
	shader.setVec3("dirLight[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
	shader.setVec3("dirLight[" + std::to_string(i) + "].direction", rot);

	shader.setVec3("viewPos", camera.Position);
	shader.setVec3("lightPower", 1.f, 1.f, 1.f);
}
