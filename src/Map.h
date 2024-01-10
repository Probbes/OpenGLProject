#pragma once
#include <vector>
#include <math.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoise/FastNoise.h>

#include "ShaderClass.h"
#include "quadtree/Quadtree.h"

class Chunk {
public:
	Chunk(int ix, int iy, int isize) : startx(ix), starty(iy), size(isize) {
		vertices.reserve(subdivisions * subdivisions * 6);
		indices.reserve(subdivisions * subdivisions * 2);
		build();
		buffer();
	}

	void draw(Shader& pshader, Shader& wshader) {
		drawChunk(pshader);
	}

	void rebuildChunk() {
		build();
		buffer();
	}

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	float size;
	int subdivisions = 1000;
	int startx, starty;

	void drawChunk(Shader& shader) {
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void build() {

		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQAIAAAA16MQQAkAAMP1KD8AAACAPw==");
		std::vector<float> noiseOutput((subdivisions + 1) * (subdivisions + 1));
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), startx * (subdivisions / size), starty * (subdivisions / size), subdivisions + 1, subdivisions + 1, (size / subdivisions) / 2000, 1);

		// Clear the vectors
		vertices.clear();
		indices.clear();

		float step = 1.0f / subdivisions * size;
		float x, z;
		int noiseIndex = 0;

		float posX, posY, posZ;

		glm::vec3 U = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 V = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);

		// Generate vertices position
		for (int i = 0; i <= subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				x = j * step;
				z = i * step;

				posX = x + startx;
				posY = noiseOutput[noiseIndex] * 300.f;
				posZ = z + starty;
				//Position
				vertices.push_back(posX);
				vertices.push_back(posY);
				vertices.push_back(posZ);

				if (j != subdivisions && i != subdivisions) {
					U = glm::vec3(step, noiseOutput[noiseIndex + 1] * 300.f, 0) - glm::vec3(0, posY, 0);
					V = glm::vec3(0, noiseOutput[(noiseIndex + subdivisions) + 1] * 300.f, step) - glm::vec3(0, posY, 0);
				}
				//else if (j == subdivisions && i != subdivisions) {
				//	U = glm::vec3(step, noiseOutput[noiseIndex - 1] * 300.f, 0) - glm::vec3(0, posY, 0) ;
				//	V = glm::vec3(0, noiseOutput[(noiseIndex + subdivisions) + 1] * 300.f, step) - glm::vec3(0, posY, 0);
				//}
				else if (i == subdivisions && j != subdivisions) {
					U = glm::vec3(step, noiseOutput[noiseIndex + 1] * 300.f, 0) - glm::vec3(0, posY, 0);
					V = glm::vec3(0, noiseOutput[(noiseIndex - subdivisions) + 1] * 300.f, step) - glm::vec3(0, posY, 0);
				}
				else {
					U = glm::vec3(0.0, 0.0, 0.0);
					V = glm::vec3(0.0, 0.0, 0.0);
				}

				Normal = glm::cross(U, V);
				Normal.y = 1.f;
				Normal = glm::normalize(Normal);

				//if (i == subdivisions) { std::cout << noiseIndex << " - " << j << " : " << U.x << " x " << V.x << " - " << U.y << " x " << V.y << " - " << U.z << " x " << V.z << std::endl; }
				//std::cout << noiseIndex << " - " << j << " : " << noiseOutput[noiseIndex + subdivisions] * 300.f << " - " << posY << std::endl;
				//if (i == subdivisions) { }
				//if (i >= subdivisions - 1) { std::cout << i << " - " << j << " : " << Normal.x << " - " << Normal.y << " - " << Normal.z << std::endl; }
				vertices.push_back(Normal.x);
				vertices.push_back(Normal.y);
				vertices.push_back(Normal.z);

				noiseIndex++;
			}
		}

		//Generate indices
		for (int i = 0; i < subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				if (j == subdivisions) {
					indices.push_back(j + ((subdivisions + 1) * i));
					indices.push_back(j + ((subdivisions + 1) * (i + 1)));

					indices.push_back(j + ((subdivisions + 1) * (i + 1)));
					indices.push_back(0 + ((subdivisions + 1) * (i + 1)));
				}
				else {
					indices.push_back(j + ((subdivisions + 1) * i));
					indices.push_back(j + ((subdivisions + 1) * (i + 1)));
				}
			}
		}
	}

	void buffer() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class Map {
public:

	Map(Camera& icamera) : camera(icamera) {
		buildChunks(); buildWater(); bufferWater();
	}
	void draw(Shader& pshader, Shader& wshader) {
		//setMaterial(pshader);
		drawChunks(pshader, wshader);
		drawWater(wshader);
	}

private:
	Camera& camera;
	int chunkWidth = 3000;
	std::vector<Chunk> chunks;
	unsigned int wVBO, wVAO;
	std::vector<float> wvertices;
	std::vector<int> windices;
	int chunkNumber = 1;

	void setMaterial(Shader& shader) {
		shader.use();
		shader.setVec3("material.ambient", glm::vec3(0.1f, 0.8f, 0.1f));
		shader.setVec3("material.diffuse", glm::vec3(0.1f, 0.8f, 0.1f));
		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 0.2f);
		shader.setInt("matOrText", 1);
	}

	void buildChunks() {
		int max = (chunkNumber * chunkNumber) + chunkNumber;
		for (int y = -chunkNumber; y < chunkNumber; y++) {
			for (int x = -chunkNumber; x < chunkNumber; x++) {
				chunks.push_back(Chunk(x * chunkWidth, y * chunkWidth, chunkWidth));
			}
		}
		//chunks.push_back(Chunk(0, 0, chunkWidth));
	}

	void drawChunks(Shader& pshader, Shader& wshader) {
		for (int i = 0; i < chunks.size(); i++) {
			chunks[i].draw(pshader, wshader);
		}
	}

	void drawWater(Shader& shader) {
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10000.f, 1.f, 10000.f));
		shader.setMat4("model", model);
		glBindVertexArray(wVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void buildWater() {
		//First triangle
		wvertices.push_back(-0.5);	//Bottom Left
		wvertices.push_back(0);
		wvertices.push_back(-0.5);

		wvertices.push_back(-0.5);	//Upper Left
		wvertices.push_back(0);
		wvertices.push_back(0.5);

		wvertices.push_back(0.5);	//Bottom Right
		wvertices.push_back(0);
		wvertices.push_back(-0.5);

		//Second triangle
		wvertices.push_back(0.5);	//Bottom Right
		wvertices.push_back(0);
		wvertices.push_back(-0.5);

		wvertices.push_back(-0.5);	//Upper Left
		wvertices.push_back(0);
		wvertices.push_back(0.5);

		wvertices.push_back(0.5);	//Upper Right
		wvertices.push_back(0);
		wvertices.push_back(0.5);


	}

	void bufferWater() {
		glGenVertexArrays(1, &wVAO);
		glGenBuffers(1, &wVBO);

		glBindVertexArray(wVAO);

		glBindBuffer(GL_ARRAY_BUFFER, wVBO);
		glBufferData(GL_ARRAY_BUFFER, wvertices.size() * sizeof(float), &wvertices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};