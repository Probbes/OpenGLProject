#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model
{
public:
    // model data 
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;
    bool gammaCorrection;
    glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
    float rotation = 0.f;
    glm::vec3 rotationAxis = glm::vec3(0.f, 0.f, 1.f);
    glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);

    // constructor, expects a filepath to a 3D model.
    Model(std::string const& path, glm::vec3 ipos, float irotation, glm::vec3 irotationAxis, glm::vec3 iscale, bool gamma = false);

    // draws the model, and thus all its meshes
    void draw(Shader& shader);
    void cleanup();

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

