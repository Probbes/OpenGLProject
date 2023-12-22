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
	Chunk(int ix, int iy, int isize) : startx(ix), starty(iy), size(isize) { build(); buffer(); }

	void draw(Shader& pshader, Shader& wshader) {
		drawChunk(pshader);
	}

	void setSubLevel(int x) {
		if (x >= 0 && x < 3) {
			subLevel = x;
			if (subLevel == 0) { subdivisions = 80; }
			else if (subLevel == 1) { subdivisions = 10; }
			else if (subLevel == 2) { subdivisions = 2; }
		}
	}

	int getSubLevel() { return subLevel; }

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
	int subLevel = 2; //0 = high, 1 = mid, 2 = low

	void drawChunk(Shader& shader) {
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
		// Generate vertices
		for (int i = 0; i <= subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				x = j * step;
				z = i * step;
				vertices.push_back(x + startx);
				vertices.push_back(0.0f + noiseOutput[noiseIndex++] * 300.f);
				vertices.push_back(z + starty);
			}
		}

		// Generate indices
		for (int i = 0; i < subdivisions; ++i) {
			for (int j = 0; j < subdivisions; ++j) {
				int index = i * (subdivisions + 1) + j;
				indices.push_back(index);
				indices.push_back(index + subdivisions + 2);
				indices.push_back(index + 1);

				indices.push_back(index);
				indices.push_back(index + subdivisions + 1);
				indices.push_back(index + subdivisions + 2);
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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class Terrain {
public:
	Terrain() { build(); buffer(); }

	void draw(Shader& pshader) {
		drawTerrain(pshader);
	}

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	int subdivisions = 1000;

	void drawTerrain(Shader& shader) {
		shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(100.f, 1.f, 100.f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void build() {
		float size = 2000.f;

		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQADAAAAmpnZPwkAAFyPQj8AAACAPw==");
		std::vector<float> noiseOutput((subdivisions + 1) * (subdivisions + 1));
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), 0, 0, subdivisions + 1, subdivisions + 1, (1.f / subdivisions) * size / 100, 1337);

		int index = 0;

		// Clear the vectors
		vertices.clear();
		indices.clear();

		

		float step = 2.0f / subdivisions * size;
		float x, z;

		// Generate vertices
		for (int i = 0; i <= subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				x = -size + j * step;
				z = -size + i * step;
				vertices.push_back(x);
				vertices.push_back(0.0f + noiseOutput[index++] * 10.f);
				vertices.push_back(z);
			}
		}

		// Generate indices
		for (int i = 0; i < subdivisions; ++i) {
			for (int j = 0; j < subdivisions; ++j) {
				int index = i * (subdivisions + 1) + j;
				indices.push_back(index);
				indices.push_back(index + subdivisions + 2);
				indices.push_back(index + 1);

				indices.push_back(index);
				indices.push_back(index + subdivisions + 1);
				indices.push_back(index + subdivisions + 2);
			}
		}
		//for (int y = 0; y < subdivisions; y++)
		//{
		//	for (int x = 0; x < subdivisions; x++)
		//	{
		//		processNoiseData(x, y, index, noiseOutput[index]);
		//		index++;
		//	}
		//}
	}

	void processNoiseData(int x, int y, int index, float noiseOutput) {
		vertices[index * 3 + 1] += noiseOutput;
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

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};

class Map {
public:

	Map(Camera& icamera) : camera(icamera) { buildChunks(); buildWater(); bufferWater(); }
	void draw(Shader& pshader, Shader& wshader) {
		checkPos();
		//setMaterial(shader);
		drawChunks(pshader, wshader);
		//drawWater(wshader);
	}

private:
	Camera& camera;
	Terrain terrain{};
	int renderedX = 0;
	int renderedZ = 0;
	int chunkWidth = 2000;
	std::vector<Chunk> chunks;
	unsigned int wVBO, wVAO, wEBO;
	std::vector<float> wvertices;
	std::vector<int> windices;
	int chunkNumber = 1;
	int currentChunk = 21;

	std::vector<int> highChunksId;
	std::vector<int> midChunksId;
	std::vector<int> lowChunksId;

	//Make a ring around the currentchunk instead of a plus sign. idem for the unsubdivisions (ring + 1)
	void checkPos() {

		int x = (int)(camera.Position.x + (chunkNumber * chunkWidth)) / 50;
		int z = (int)(camera.Position.z + (chunkNumber * chunkWidth)) / 50;
		int nb = (z * chunkNumber * 2) + x;
		if (currentChunk != nb) {

			currentChunk = nb;
			buildCircles(nb);
		}
	}

	void setMaterial(Shader& shader) {
		shader.setVec3("material.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 0.8f);
		shader.setInt("matOrText", 1);
	}

	void drawTerrain(Shader& shader) {
		terrain.draw(shader);
	}

	void buildChunks() {
		int max = (chunkNumber * chunkNumber) + chunkNumber;
		for (int y = -chunkNumber; y < chunkNumber; y++) {
			for (int x = -chunkNumber; x < chunkNumber; x++) {
				chunks.push_back(Chunk(x * chunkWidth, y * chunkWidth, chunkWidth));
			}
		}
	}

	// To rework : 
	void buildCircles(int nb) {
		int lenght = 3;
		highChunksId.clear();
		midChunksId.clear();
		lowChunksId.clear();

		//for (int i = -lenght; i <= lenght; i++) {
		//	if (abs(i) == 1 || i== 0) {
		//		highChunksId.push_back(nb + i);
		//		highChunksId.push_back(nb + ((chunkNumber * 2) * 1) + i);
		//		highChunksId.push_back(nb - ((chunkNumber * 2) * 1) + i);
		//	}
		//	if (abs(i) == 2) {
		//		midChunksId.push_back(nb + i);
		//		midChunksId.push_back(nb + ((chunkNumber * 2) * 2) + i);
		//		midChunksId.push_back(nb - ((chunkNumber * 2) * 2) + i);
		//	}
		//	if (abs(i) == 3) {
		//		/*lowChunksId.push_back(nb + i);*/
		//	}
		//}

		////First circle
		//for (int i = 0; i < highChunksId.size(); i++) {
		//	if (chunks[highChunksId[i]].getSubLevel() != 0) {
		//		chunks[highChunksId[i]].setSubLevel(0);
		//		chunks[highChunksId[i]].rebuildChunk();
		//	}
		//}

		////Second circle
		//for (int i = 0; i < midChunksId.size(); i++) {
		//	if (chunks[midChunksId[i]].getSubLevel() != 1) {
		//		chunks[midChunksId[i]].setSubLevel(1);
		//		chunks[midChunksId[i]].rebuildChunk();
		//	}
		//}

		////Third circle
		//for (int i = 0; i < lowChunksId.size(); i++) {
		//	if (chunks[lowChunksId[i]].getSubLevel() != 2) {
		//		chunks[lowChunksId[i]].setSubLevel(2);
		//		chunks[lowChunksId[i]].rebuildChunk();
		//	}
		//}
	}

	void buildSecondCircle(int nb) {

	}

	void buildThirdCircle(int nb) {

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
		model = glm::scale(model, glm::vec3(4000.f, 1.f, 4000.f));
		shader.setMat4("model", model);
		glBindVertexArray(wVAO);
		glDrawElements(GL_TRIANGLES, windices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void buildWater() {
		// Clear the vectors
		wvertices.clear();
		windices.clear();

		int subdivisions = 10;

		float step = 2.0f / subdivisions;
		float x, z;

		// Generate vertices
		for (int i = 0; i <= subdivisions; ++i) {
			for (int j = 0; j <= subdivisions; ++j) {
				x = -1.0f + j * step;
				z = -1.0f + i * step;
				wvertices.push_back(x);
				wvertices.push_back(0.0f);
				wvertices.push_back(z);
			}
		}

		// Generate indices
		for (int i = 0; i < subdivisions; ++i) {
			for (int j = 0; j < subdivisions; ++j) {
				int index = i * (subdivisions + 1) + j;
				windices.push_back(index);
				windices.push_back(index + subdivisions + 2);
				windices.push_back(index + 1);

				windices.push_back(index);
				windices.push_back(index + subdivisions + 1);
				windices.push_back(index + subdivisions + 2);
			}
		}
	}

	void bufferWater() {
		glGenVertexArrays(1, &wVAO);
		glGenBuffers(1, &wVBO);
		glGenBuffers(1, &wEBO);

		glBindVertexArray(wVAO);

		glBindBuffer(GL_ARRAY_BUFFER, wVBO);
		glBufferData(GL_ARRAY_BUFFER, wvertices.size() * sizeof(float), &wvertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, windices.size() * sizeof(float), &windices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
};