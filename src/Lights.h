#pragma once

#include "SimpleGeometry.h"

class Light {
public:
	CCube rec;

	Light() {
	}

	void drawCube(Shader& shader) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(sin(glfwGetTime()) * 1.f, 2.f, 2.f));
		//model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		glBindVertexArray(rec.VAO);
		glDrawElements(GL_TRIANGLES, (rec.indices.size() * sizeof(float)) / rec.indicesRow, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);    
	}
};

class PointLight : public Light {
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 pos = glm::vec3(1.f, 1.f, 1.f);

	PointLight(Camera icamera, glm::vec3 icolor, glm::vec3 ipos) : camera (icamera), color(icolor), pos(ipos) {}

	void draw(Shader& shader, unsigned int i) {
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

private:
	Camera camera;
};

class SunLight : public Light {
public:
	glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
	glm::vec3 rot = glm::vec3(1.f, 1.f, 1.f);

	SunLight(Camera icamera, glm::vec3 icolor, glm::vec3 irot) :camera(icamera), color(icolor), rot(irot) {}

	void draw(Shader& shader, unsigned int i) {
		shader.setVec3("dirLight[" + std::to_string(i) + "].ambient", color);
		shader.setVec3("dirLight[" + std::to_string(i) + "].diffuse", color);
		shader.setVec3("dirLight[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		shader.setVec3("dirLight[" + std::to_string(i) + "].direction", rot);

		shader.setVec3("viewPos", camera.Position);
		shader.setVec3("lightPower", 1.f, 1.f, 1.f);
	}

private:
	Camera camera;
};