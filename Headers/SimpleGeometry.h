#pragma once
#ifndef GEOMETRY_SPHERE_H
#define GEOMETRY_SPHERE_H
#include <vector>
#include <math.h>

#include "ShaderClass.h"

//CSphere code made by Song Ho Ahn
class CSphere
{
public:
    CSphere() {
        createSphere();
        buffer();
    }

    void draw(Shader& shader) {
        //shader.draw(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (indices.size() * sizeof(float)) / indicesRow, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    unsigned int VBO, VAO, EBO;
    unsigned int indicesRow = 3;
    float radius = 1.f;
    float sectorCount = 6.f;
    float stackCount = 8.f;
    std::vector<float> vertices;

    std::vector<int> indices;

    std::vector<int> lineIndices;

    void createSphere() {

        float x, y, z, xy;                              // vertex position

        float sectorStep = 2 * M_PI / sectorCount;
        float stackStep = M_PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = radius * cosf(stackAngle);             // r * cos(u)
            z = radius * sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) vertices per stack
            // first and last vertices have same position and normal, but different tex coords
            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position (x, y, z)
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);
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

    void buffer() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesT), indicesT, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }
};

class CCube {
public:
    CCube() {
        build(5);
        buffer();
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    unsigned int VBO, VAO, EBO;
    unsigned int indicesRow = 3;
    std::vector<float> vertices;
    std::vector<int> indices;

    void build(int nb) {
        int nbm = nb - 1;
        //front
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(i / nbm);
                vertices.push_back(j / nbm);
                vertices.push_back(0);

                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(1.f);
            }
        }

        //back
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(i / nbm);
                vertices.push_back(j / nbm);
                vertices.push_back(1);

                vertices.push_back(0.f);
                vertices.push_back(0.f);
                vertices.push_back(-1.f);
            }
        }
        //bottom
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(i / nbm);
                vertices.push_back(0);
                vertices.push_back(j / nbm);

                vertices.push_back(0.f);
                vertices.push_back(-1.f);
                vertices.push_back(0.f);
            }
        }
        //up
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(i / nbm);
                vertices.push_back(1);
                vertices.push_back(j / nbm);

                vertices.push_back(0.f);
                vertices.push_back(1.f);
                vertices.push_back(0.f);
            }
        }

        //left
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(0);
                vertices.push_back(i / nbm);
                vertices.push_back(j / nbm);

                vertices.push_back(-1.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
            }
        }
        //right
        for (float i = 0; i <= nbm; i++) {
            for (float j = 0; j <= nbm; j++) {
                vertices.push_back(1);
                vertices.push_back(i / nbm);
                vertices.push_back(j / nbm);

                vertices.push_back(1.f);
                vertices.push_back(0.f);
                vertices.push_back(0.f);
            }
        }
        int start, k1, k2 = 0;
        for (int faces = 0; faces < 6; faces++) {
            start = faces * (nb * nb);
            k1 = start;
            k2 = start + nb;
            for (k1; k1 < (start + (nb * nb)) - nb; k1++) {
                if ((k1 + 1) % nb != 0) {
                    indices.push_back(k1);
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);

                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
                k2++;
            }
        }
    }

    void buffer() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(verticesT), verticesT, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesT), indicesT, GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        

        // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);
    }
};

#endif