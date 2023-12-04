#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in float noise[64*64];
in int gl_VertexID;

out vec2 TexCoords;
out vec4 vertexColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int noiseSize = 64 * 64;
//uniform float noise[64 * 64];

void main()
{
    vec3 changePos = aPos;
    //for (int i = 0; i < noiseSize; i++) {
    //    if (gl_VertexID == i) {changePos.y = noise[i] * 10; break;}
    //}
    TexCoords = aTexCoords;    
    gl_Position = projection * view * model * vec4(changePos, 1.0);
    vertexColor = vec4(0.5, 0.0, 0.0, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    FragPos = vec3(model * vec4(changePos, 1.0));
}