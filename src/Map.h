#pragma once
#include <vector>
#include <math.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoise/FastNoise.h>

#include "ShaderClass.h"

class Chunk {
public:

};

class Map {
public:
	Map(Camera& icamera) : camera(icamera) {
		build(0, 0);
		buffer();
	}

	void draw(Shader& shader) {
		checkPos();
		shader.use();
		setMaterial(shader);
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void checkPos() {
		int x = camera.Position.x - 1;
		int z = camera.Position.z - 1;
		int qx = x / width;
		int qz = z / width;
		if (x < 0 && x % width != 0) {
			qx--;
		}
		if (z < 0 && z % width != 0) {
			qz--;
		}
		//std::cout << qx << ":" << renderedX << "-" << qz << ":" << renderedZ << std::endl;
		if (qx < renderedX || qx > renderedX || qz < renderedZ || qz > renderedZ ) {
			renderedX = qx;
			renderedZ = qz;
			vertices.clear();
			indices.clear();

			if (qx < 0 && qz < 0) {
				build(renderedX * width + 1, renderedZ * width + 1);
			}
			else if (qx < 0) {
				build(renderedX * width + 1, renderedZ * width);
			}
			else if (qz < 0) {
				build(renderedX * width, renderedZ * width + 1);
			}
			else {
				build(renderedX * width, renderedZ * width);
			}
			
			buffer();
		}
		
	}

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	int width = 16;
	Camera& camera;
	int renderedX = 0;
	int renderedZ = 0;

	void setMaterial(Shader& shader) {
		shader.setVec3("material.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 0.8f);
		shader.setInt("matOrText", 1);
	}

	void build(int startx, int starty) {
		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("EwAUrsc/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPw==");
		// Create an array of floats to store the noise output in
		std::vector<float> noiseOutput(width * width);

		// Generate a width x width area of noise
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), startx, starty, width, width, 0.02f, 1337);
		int index = 0;
		for (int y = starty; y < starty + width; y++)
		{
			for (int x = startx; x < startx + width; x++)
			{
				processNoiseData(x, y, index, noiseOutput[index]);
				index++;
			}
		}
	}

	void processNoiseData(int x, int y, int index, float noiseOutput) {
		
		vertices.push_back(x);
		vertices.push_back(noiseOutput);
		vertices.push_back(y);
		vertices.push_back(0.f);
		vertices.push_back(1.f);
		vertices.push_back(0.f);

		if (index % width != (width - 1) && index < (width * width) - width) {
			//std::cout << index << "-" << index + 1 << "-" << index + width << "-" << index + 1 << "-" << index + width << "-" << index + width + 1 << std::endl;
			
			indices.push_back(index);
			indices.push_back(index + 1);
			indices.push_back(index + width);

			indices.push_back(index + 1);
			indices.push_back(index + width);
			indices.push_back(index + width + 1);
		}
		
	}

	void buffer() {
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}
};