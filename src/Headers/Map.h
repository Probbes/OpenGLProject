#pragma once
#include <vector>
#include <math.h>
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <FastNoise/FastNoise.h>
//#include <quadtree/Quadtree.h>

#include "ShaderClass.h"
#include "Player.h"

class Chunk {
public:
	Chunk(float ix, float iz, float isize);

	void draw(Shader& pshader);

	void rebuildChunk();

	float checkPosChunkHeight(float x, float z);

	glm::vec2 getStart() { return glm::vec2(startx, startz); }

	float getSize() { return size; }

private:
	unsigned int VBO, VAO, EBO;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> noiseOutput;

	float step;
	float size;
	int subdivisions = 200;
	float startx, startz;

	glm::vec3 randColorTest;

	void buildNoise();

	void build();

	void buffer();
};

struct QuadtreeNode {
	glm::vec2 min;
	glm::vec2 center;
	float size;
	int level;
	QuadtreeNode* children[4];

	QuadtreeNode(glm::vec2 min, float isize, int ilevel) : min(min), size(isize), level(ilevel) {
		for (int i = 0; i < 4; ++i) {
			children[i] = nullptr;
		}
		center = glm::vec2(min.x + size / 2.f, min.y + size / 2.f);
	}
};

class Quadtree {
public:
	QuadtreeNode* currentNode;
	Quadtree(glm::vec2 min, float size, int imaxDepth);

	void setCurrentNode(glm::vec2 point) { currentNode = getNodeWherePointIs(point); }

	//Return a vector of nodes that don't have children
	std::vector<QuadtreeNode*> returnAllNull();

	//Rebuild the quadtree for a specific point
	void subdivideUpdate(glm::vec2 point) {
		subdivideNode(root, maxDepth, point);
	}

	//Return the smallest node where the point is
	QuadtreeNode* getNodeWherePointIs(glm::vec2 point) {
		return getNodeWherePointIsHelper(point, root, maxDepth);
	}

private:
	QuadtreeNode* root;
	int maxDepth;

	//Build the quadtree for a specific point (first occurence is vec2(0,0)
	void subdivideNode(QuadtreeNode* node, int level, glm::vec2 point);

	void returnAllNullHelper(QuadtreeNode* node, std::vector<QuadtreeNode*>& result);

	//Return the index of the closest node's children to a point
	int minimumLenght(glm::vec2 point, QuadtreeNode* node);

	QuadtreeNode* getNodeWherePointIsHelper(glm::vec2 point, QuadtreeNode* node, int level);
};

class Map {
public:
	Player player;

	Map(Camera& icamera);

	void update();

	void draw(Shader& pshader, Shader& wshader, Shader& playerShader);

	void getPlayerPos();

	//float checkPosMapHeight(float x, float z);

private:
	Camera& camera;
	std::vector<Chunk> chunks;
	unsigned int wVBO, wVAO;
	std::vector<float> wvertices;
	std::vector<int> windices;
	glm::vec2 playerPos;
	Quadtree quadtree{ glm::vec2(-5000, -5000), 10000.f, 5 };

	void setMaterial(Shader& shader);

	void drawChunks(Shader& pshader);

	void drawWater(Shader& shader);

	void buildWater();

	void bufferWater();

	void buildChunksFromQuadtree();

	void movePlayerY();
};