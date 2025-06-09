#include "Map.h"

//--------------------CHUNK------------------------
Chunk::Chunk(float ix, float iz, float isize) : startx(ix), startz(iz), size(isize) {
	step = 1.0f / subdivisions * size;
	buildNoise();
	build();
	buffer();

	//assign random color to chunk
	randColorTest = glm::vec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
}

void Chunk::draw(Shader& shader) {
	shader.use();
	shader.setVec3("test", randColorTest);
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.f, 0.f, 0.f));
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLE_STRIP, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Chunk::rebuildChunk() {
	build();
	buffer();
}

float Chunk::checkPosChunkHeight(float x, float z) {
	float zeroX = x - startx;
	float zeroZ = z - startz;
	float remainderX = fmod(x, step);
	float remainderZ = fmod(z, step);

	int index = ((int)(zeroZ / step) * (subdivisions + 1) + (int)(zeroX / step)) * 6;

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

void Chunk::buildNoise() {
	noiseOutput.resize((subdivisions + 3) * (subdivisions + 3));
	FastNoise::SmartNode<> fnGenerator = FastNoise::NewFromEncodedNodeTree("DQAIAAAA16MQQAkAAMP1KD8AAACAPw==");
	fnGenerator->GenUniformGrid2D(noiseOutput.data(), startx * (subdivisions / size), startz * (subdivisions / size), subdivisions + 3, subdivisions + 3, (size / subdivisions) / 2000, 1);
}

void Chunk::build() {

	// Clear the vectors
	vertices.clear();
	indices.clear();

	float x, z;
	int noiseIndex = subdivisions + 4;

	float posX, posY, posZ;

	glm::vec3 U = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 V = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Normal = glm::vec3(0.0f, 0.0f, 0.0f);

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
			U = glm::vec3(1	, noiseOutput[noiseIndex + 1] * 300.f, 0) - glm::vec3(-1, noiseOutput[noiseIndex - 1] * 300.f, 0);
			V = glm::vec3(0, noiseOutput[(noiseIndex + subdivisions) + 3] * 300.f, 1) - glm::vec3(0, noiseOutput[(noiseIndex - subdivisions) - 3] * 300.f, -1);
			/*U = glm::vec3(step, noiseOutput[noiseIndex + 1] * 300.f, 0) - glm::vec3(-step, noiseOutput[noiseIndex - 1] * 300.f, 0);
			V = glm::vec3(0, noiseOutput[(noiseIndex + subdivisions) + 3] * 300.f, step) - glm::vec3(0, noiseOutput[(noiseIndex - subdivisions) - 3] * 300.f, -step);*/
			Normal = glm::cross(U, V);
			//Normal.y = abs(Normal.y);
			Normal.y = 1.f;
			Normal = glm::normalize(Normal);
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

void Chunk::buffer() {
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

//--------------------Quadtree--------------------
Quadtree::Quadtree(glm::vec2 min, float size, int imaxDepth) : maxDepth(imaxDepth) {
	root = new QuadtreeNode(min, size, maxDepth + 1);
	subdivideNode(root, maxDepth, glm::vec2(0, 0));
	setCurrentNode(glm::vec2(0, 0));
}

std::vector<QuadtreeNode*> Quadtree::returnAllNull() {
	std::vector<QuadtreeNode*> result;
	returnAllNullHelper(root, result);
	return result;
};

void Quadtree::subdivideNode(QuadtreeNode* node, int level, glm::vec2 point) {
	if (level == 1) {
		return;
	}

	float halfSize = node->size / 2.f;

	glm::vec2 mid = glm::vec2(node->min.x + halfSize, node->min.y + halfSize);
	node->children[0] = new QuadtreeNode(node->min, halfSize, level);
	node->children[1] = new QuadtreeNode(glm::vec2(mid.x, node->min.y), halfSize, level);
	node->children[2] = new QuadtreeNode(glm::vec2(node->min.x, mid.y), halfSize, level);
	node->children[3] = new QuadtreeNode(mid, halfSize, level);

	subdivideNode(node->children[minimumLenght(point, node)], level - 1, point);
}

void Quadtree::returnAllNullHelper(QuadtreeNode* node, std::vector<QuadtreeNode*>& result) {
	bool hasChildren = false;
	for (int i = 0; i < 4; i++) {
		if (node->children[i] != nullptr) {
			hasChildren = true;
			returnAllNullHelper(node->children[i], result);
		}
	}
	if (!hasChildren) {
		result.push_back(node);
	}
};

int Quadtree::minimumLenght(glm::vec2 point, QuadtreeNode* node) {
	if (node->children[0]->level == 0) return -1;

	float min = glm::distance(point, node->children[0]->center);
	int minIndex = 0;

	for (int i = 1; i < 4; i++) {
		float distance = glm::distance(point, node->children[i]->center);
		if (distance < min) {
			min = distance;
			minIndex = i;
		}
	}
	return minIndex;
}

QuadtreeNode* Quadtree::getNodeWherePointIsHelper(glm::vec2 point, QuadtreeNode* node, int level) {
	if (level > 1) {
		getNodeWherePointIsHelper(point, node->children[minimumLenght(point, node)], level - 1);
	}
	else { return node; }
}

//--------------------MAP------------------------
Map::Map(Camera& icamera) : camera(icamera), player(icamera) {
	getPlayerPos();
	buildChunksFromQuadtree();
	buildWater(); bufferWater();
}

void Map::update() {
	getPlayerPos();
	//Quadtree check and build map
	glm::vec2 cameraPos = glm::vec2(camera.Position.x, camera.Position.z);
	glm::vec2 nodeMin = quadtree.currentNode->min;
	float nodeSize = quadtree.currentNode->size;
	if (cameraPos.x < nodeMin.x || cameraPos.x > nodeMin.x + nodeSize || cameraPos.y < nodeMin.y || cameraPos.y > nodeMin.y + nodeSize) {
		quadtree.subdivideUpdate(cameraPos);
		buildChunksFromQuadtree();
		quadtree.setCurrentNode(cameraPos);
	}

	//Player y position
	movePlayerY();
}

void Map::draw(Shader& pshader, Shader& wshader, Shader& playerShader) {
	//setMaterial(pshader);
	drawChunks(pshader);
	drawWater(wshader);
	player.draw(playerShader);
}

void Map::getPlayerPos() {
	playerPos = glm::vec2(player.Position.x, player.Position.z);
}

void Map::setMaterial(Shader& shader) {
	shader.use();
	shader.setVec3("material.ambient", glm::vec3(0.1f, 0.8f, 0.1f));
	shader.setVec3("material.diffuse", glm::vec3(0.1f, 0.8f, 0.1f));
	shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
	shader.setFloat("material.shininess", 0.2f);
	shader.setInt("matOrText", 1);
}

void Map::drawChunks(Shader& pshader) {
	for (int i = 0; i < chunks.size(); i++) {
		chunks[i].draw(pshader);
	}
}

void Map::drawWater(Shader& shader) {
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

void Map::buildWater() {
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

void Map::bufferWater() {
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

void Map::buildChunksFromQuadtree() {
	chunks.clear();
	std::vector<QuadtreeNode*> allNodes = quadtree.returnAllNull();
	for (int i = 0; i < allNodes.size(); i++) {
		chunks.push_back(Chunk(allNodes[i]->min.x, allNodes[i]->min.y, allNodes[i]->size));
	}
}

void Map::movePlayerY() {
	for (int i = 0; i < chunks.size(); i++) {
		glm::vec2 chunkStart = chunks[i].getStart();
		float chunkSize = chunks[i].getSize();
		if (playerPos.x >= chunkStart.x && playerPos.x < chunkStart.x + chunkSize && playerPos.y >= chunkStart.y && playerPos.y < chunkStart.y + chunkSize) {
			player.Position.y = chunks[i].checkPosChunkHeight(playerPos.x, playerPos.y);
		}
	}	
}