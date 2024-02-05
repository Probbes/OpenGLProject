#pragma once
#include <vector>
#include <math.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoise/FastNoise.h>

#include "ShaderClass.h"
#include "Player.h"
//#include "quadtree/Quadtree.h"

class Chunk {
public:
	Chunk(int ix, int iz, int isize);

	void draw(Shader& pshader);

	void rebuildChunk();

	float checkPosChunkHeight(float x, float z);

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> noiseOutput;

	float step;
	float size;
	int subdivisions = 1000;
	int startx, startz;

	void drawChunk(Shader& shader);

	void buildNoise();

	void build();

	void buffer();
};

class Map {
public:
	Player player;

	Map(Camera& icamera);

	void draw(Shader& pshader, Shader& wshader, Shader& playerShader);

	float checkPosMapHeight(float x, float z);

private:
	Camera& camera;

	int chunkWidth = 3000;
	std::vector<Chunk> chunks;
	unsigned int wVBO, wVAO;
	std::vector<float> wvertices;
	std::vector<int> windices;
	int chunkNumber = 1;

	void setMaterial(Shader& shader);

	void buildChunks();

	void drawChunks(Shader& pshader);

	void drawWater(Shader& shader);

	void buildWater();

	void bufferWater();
};