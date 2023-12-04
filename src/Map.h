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
	Chunk(int ix, int iy) { build(ix, iy); buffer(); }

	void draw(Shader& shader) {
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		shader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	int width = 32;

	void build(int startx, int starty) {
		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("EwAUrsc/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPw==");
		// Create an array of floats to store the noise output in
		std::vector<float> noiseOutput(width * width);

		// Generate a width x width area of noise
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), startx, starty, width, width, 0.001f, 1337);
		int index = 0;
		int resolution = 1;
		for (int y = starty; y < starty + width; y=y+resolution)
		{
			for (int x = startx; x < startx + width; x=x+resolution)
			{
				processNoiseData(x, y, index, noiseOutput[index]);
				index = index + resolution;
			}
		}
	}

	void processNoiseData(int x, int y, int index, float noiseOutput) {

		vertices.push_back(x);
		vertices.push_back(noiseOutput * 20);
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

class Planet {
public:

	Planet() { build(); buffer(); buildWater(); bufferWater(); }
	//Planet() { build(); buffer(); }
	void draw(Shader& pshader, Shader& wshader) {
		drawPlanet(pshader);
		drawWater(wshader);
	 }

private:
	unsigned int VBO, VAO, EBO;
	unsigned int wVBO, wVAO, wEBO;
	unsigned int indicesRow = 3;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<int> lineIndices;
	std::vector<float> wvertices;
	std::vector<int> windices;
	std::vector<int> wlineIndices;
	float radius = 200.f;
	float sectorCount = 1024.f;
	float stackCount = 1024.f;
	float mountainIntensity = 0.1f;

	void drawPlanet(Shader& pshader) {
		pshader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(radius, radius, radius));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		pshader.setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void drawWater(Shader& wshader) {
		wshader.use();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(5.f, 0.f, 5.f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		wshader.setMat4("model", model);
		glBindVertexArray(wVAO);
		glDrawElements(GL_TRIANGLES, windices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void build() {

		FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("EwAUrsc/DQAEAAAAAAAgQAkAAGZmJj8AAAAAPw==");
		// Create an array of floats to store the noise output in
		std::vector<float> noiseOutput((sectorCount + 1) * (stackCount + 1));

		// Generate a width x width area of noise
		fnGenerator->GenUniformGrid2D(noiseOutput.data(), 0, 0, sectorCount + 1, stackCount + 1, 0.004f, 1337);
		int index = 0;

		float x, y, z, xy;                              // vertex position

		float sectorStep = 2 * M_PI / sectorCount;
		float stackStep = M_PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2

			// add (sectorCount+1) vertices per stack
			// first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				// vertex position (x, y, z)
				xy = radius * cosf(stackAngle) * ((noiseOutput[index] * mountainIntensity) + 1);             // r * cos(u)
				z = radius * sinf(stackAngle) * ((noiseOutput[index] * mountainIntensity) + 1);              // r * sin(u)
				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

				vertices.push_back(x);
				vertices.push_back(y);
				vertices.push_back(z);

				index++;
			}
		}

		// generate CCW index list of sphere triangles
		// k1--k1+1
		// |  / |
		// | /  |
		// k2--k2+1

		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				lineIndices.push_back(k1);
				lineIndices.push_back(k2);
				if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				{
					lineIndices.push_back(k1);
					lineIndices.push_back(k1 + 1);
				}
			}
		}
	}

	void buildWater() {
		radius = radius - 5;
		sectorCount = sectorCount / 2.f;
		stackCount = stackCount / 2.f;
		float x, y, z, xy;                              // vertex position

		float sectorStep = 2 * M_PI / sectorCount;
		float stackStep = M_PI / stackCount;
		float sectorAngle, stackAngle;

		for (int i = 0; i <= stackCount; ++i)
		{
			stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
			// vertex position (x, y, z)
			xy = radius * cosf(stackAngle);             // r * cos(u)
			z = radius * sinf(stackAngle);              // r * sin(u)
			// add (sectorCount+1) vertices per stack
			// first and last vertices have same position and normal, but different tex coords
			for (int j = 0; j <= sectorCount; ++j)
			{
				sectorAngle = j * sectorStep;           // starting from 0 to 2pi

				x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
				y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

				wvertices.push_back(x);
				wvertices.push_back(y);
				wvertices.push_back(z);
			}
		}

		// generate CCW index list of sphere triangles
		// k1--k1+1
		// |  / |
		// | /  |
		// k2--k2+1

		int k1, k2;
		for (int i = 0; i < stackCount; ++i)
		{
			k1 = i * (sectorCount + 1);     // beginning of current stack
			k2 = k1 + sectorCount + 1;      // beginning of next stack

			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if (i != 0)
				{
					windices.push_back(k1);
					windices.push_back(k2);
					windices.push_back(k1 + 1);
				}

				// k1+1 => k2 => k2+1
				if (i != (stackCount - 1))
				{
					windices.push_back(k1 + 1);
					windices.push_back(k2);
					windices.push_back(k2 + 1);
				}

				// store indices for lines
				// vertical lines for all stacks, k1 => k2
				wlineIndices.push_back(k1);
				wlineIndices.push_back(k2);
				if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
				{
					wlineIndices.push_back(k1);
					wlineIndices.push_back(k1 + 1);
				}
			}
		}
	}

	void buffer() {
		//Planet

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	}

	void bufferWater() {

		//Water

		glGenVertexArrays(1, &wVAO);
		glGenBuffers(1, &wVBO);
		glGenBuffers(1, &wEBO);

		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glBindVertexArray(wVAO);

		glBindBuffer(GL_ARRAY_BUFFER, wVBO);
		glBufferData(GL_ARRAY_BUFFER, wvertices.size() * sizeof(float), &wvertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, windices.size() * sizeof(float), &windices[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	}
};

class Map {
public:
	std::vector<Planet> planets;

	Map(Camera& icamera) : camera(icamera) {
		buildPLanet();
	}

	void draw(Shader& pshader, Shader& wshader) {
		//checkPos();
		//setMaterial(shader);
		drawPlanet(pshader, wshader);
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
		}
		
	}

private:
	Camera& camera;
	int renderedX = 0;
	int renderedZ = 0;
	int width = 32;
	std::vector<Chunk> chunks;

	void setMaterial(Shader& shader) {
		shader.setVec3("material.ambient", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.diffuse", glm::vec3(0.9f, 0.9f, 0.9f));
		shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader.setFloat("material.shininess", 0.8f);
		shader.setInt("matOrText", 1);
	}

	void buildChunks() {
		int size = 8;
		for (int y = -size; y < size; y++) {
			for (int x = -size; x < size; x++) {
				chunks.push_back(Chunk(x * width, y * width));
			}
		}
	}

	void buildPLanet() {
		planets.push_back(Planet());
	}

	void drawChunks(Shader& shader) {
		for (int i = 0; i < chunks.size(); i++) {
			chunks[i].draw(shader);
		}
	}

	void drawPlanet(Shader& pshader, Shader& wshader) {
		planets[0].draw(pshader, wshader);
	}
};