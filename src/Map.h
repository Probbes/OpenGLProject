#pragma once
#include <vector>
#include <math.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoise/FastNoise.h>

#include "ShaderClass.h"
#include "Player.h"
#include "quadtree/Quadtree.h"

class Chunk {
public:
	Chunk(int ix, int iz, int isize) : startx(ix), startz(iz), size(isize) {
		step = 1.0f / subdivisions * size;
		vertices.reserve(subdivisions * subdivisions * 6);
		indices.reserve(subdivisions * subdivisions * 2);
		noiseOutput.reserve((subdivisions + 3) * (subdivisions + 3));
		buildNoise();
		build();
		buffer();
	}

	void draw(Shader& pshader) {
		drawChunk(pshader);
	}

	void rebuildChunk() {
		build();
		buffer();
	}

	float checkPosChunkHeight(float x, float z) {
		//find index
		int x_int = static_cast<int>(x * 100);
		int z_int = static_cast<int>(z * 100);
		int step_int = static_cast<int>(step * 100);

		std::div_t resultX = std::div(x_int, step_int);
		std::div_t resultZ = std::div(z_int, step_int);

		double quotientX = static_cast<double>(resultX.quot);
		double quotientZ = static_cast<double>(resultZ.quot);

		float remainderX = fmod(x, step);
		float remainderZ = fmod(z, step);

		int index = (quotientZ * (subdivisions + 1) + quotientX) * 6;

		//Barycentric coordinates
		glm::vec3 p1 = glm::vec3(0, vertices[index + 1], 0);
		glm::vec3 p2 = glm::vec3(step, vertices[index + 7], 0);
		glm::vec3 p3 = glm::vec3(0, vertices[index + (subdivisions + 1) * 6 + 1], step);

		float denom = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

		float a = ((p2.z - p3.z) * (remainderX - p3.x) + (p3.x - p2.x) * (remainderZ - p3.z)) / denom;
		float b = ((p3.z - p1.z) * (remainderX - p3.x) + (p1.x - p3.x) * (remainderZ - p3.z)) / denom;
		float c = 1 - a - b;

		if (a <= 1 && a >= 0 && b <= 1 && b >= 0 && c <= 1 && c >= 0) {
			return a * p1.y + b * p2.y + c * p3.y;
		}
		else {	//to redo : not very optimized
			p1 = glm::vec3(step, vertices[index + (subdivisions + 2) * 6 + 1], step);
			denom = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

			a = ((p2.z - p3.z) * (remainderX - p3.x) + (p3.x - p2.x) * (remainderZ - p3.z)) / denom;
			b = ((p3.z - p1.z) * (remainderX - p3.x) + (p1.x - p3.x) * (remainderZ - p3.z)) / denom;
			c = 1 - a - b;

			return a * p1.y + b * p2.y + c * p3.y;
		}
	}

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> noiseOutput;

	float step;
	float size;
	int subdivisions = 1000;
	int startx, startz;

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

	void buildNoise() {
		//Why do I have to create another noise variable ?
		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQAIAAAA16MQQAkAAMP1KD8AAACAPw==");
		std::vector<float> noise((subdivisions + 3) * (subdivisions + 3));	//(Subdivisions + 1) + 2 -> extra vertices for normal
		fnGenerator->GenUniformGrid2D(noise.data(), startx * (subdivisions / size), startz * (subdivisions / size), subdivisions + 3, subdivisions + 3, (size / subdivisions) / 2000, 1);
		noiseOutput = noise;
	}

	void build() {

		// Clear the vectors
		vertices.clear();
		indices.clear();

		float x, z;
		int noiseIndex = subdivisions + 4;

		float posX, posY, posZ;

		glm::vec3 U = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 V = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);
		//std::cout << noiseOutput[subdivisions];

		// Generate vertices position
		for (int i = 0; i <= subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				x = j * step;
				z = i * step;

				//Position
				posX = x + startx;
				posY = noiseOutput[noiseIndex] * 300.f;
				posZ = z + startz;
				vertices.push_back(posX);
				vertices.push_back(posY);
				vertices.push_back(posZ);

				//Normal
				U = glm::vec3(step, noiseOutput[noiseIndex + 1] * 300.f, 0) - glm::vec3(-step, noiseOutput[noiseIndex - 1] * 300.f, 0);
				V = glm::vec3(0, noiseOutput[(noiseIndex + subdivisions) + 3] * 300.f, step) - glm::vec3(0, noiseOutput[(noiseIndex - subdivisions) - 3] * 300.f, -step);
				Normal = glm::cross(U, V);
				Normal.y = 1.f;
				Normal = glm::normalize(Normal);

				//if (j == subdivisions) { std::cout << noiseIndex << " - " << j << " : " << U.x << " x " << V.x << " - " << U.y << " x " << V.y << " - " << U.z << " x " << V.z << std::endl; }
				//std::cout << noiseIndex << " - " << j << " : " << noiseOutput[noiseIndex + subdivisions + 3] * 300.f << " - " << noiseOutput[noiseIndex - subdivisions - 3] * 300.f << std::endl;
				//if (i == subdivisions) { }
				//std::cout << i << " - " << j << " : " << Normal.x << " - " << Normal.y << " - " << Normal.z << std::endl;
				vertices.push_back(Normal.x);
				vertices.push_back(Normal.y);
				vertices.push_back(Normal.z);
				
				if (j == subdivisions) { noiseIndex = noiseIndex + 3; }
				else { noiseIndex++; }
				
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
	Player player;

	Map(Camera& icamera) : camera(icamera), player(icamera) {
		buildChunks(); buildWater(); bufferWater();
	}

	void draw(Shader& pshader, Shader& wshader, Shader& playerShader) {
		//setMaterial(pshader);
		drawChunks(pshader);
		drawWater(wshader);
		player.draw(playerShader);
	}

	float checkPosMapHeight(float x, float z) {
		if (x >= 0 && z >= 0) {
			return chunks[3].checkPosChunkHeight(x, z);
		}
		else if (x < 0 && z >= 0) {
			return chunks[2].checkPosChunkHeight(chunkWidth - abs(x), z);
		}
		else if (x >= 0 && z < 0) {
			return chunks[1].checkPosChunkHeight(x, chunkWidth - abs(z));
		}
		else if (x < 0 && z < 0) {
			return chunks[0].checkPosChunkHeight(chunkWidth - abs(x), chunkWidth - abs(z));
		}
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
		for (int z = -chunkNumber; z < chunkNumber; z++) {
			for (int x = -chunkNumber; x < chunkNumber; x++) {
				chunks.push_back(Chunk(x * chunkWidth, z * chunkWidth, chunkWidth));
			}
		}
		//chunks.push_back(Chunk(0, 0, chunkWidth));
	}

	void drawChunks(Shader& pshader) {
		for (int i = 0; i < chunks.size(); i++) {
			chunks[i].draw(pshader);
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