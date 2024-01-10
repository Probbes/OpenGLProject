#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 vertexColor;

uniform vec3 viewPos;

void main()
{

    FragColor = vec4(0.0, 0.4, 1.0, 0.9);

    //if (radius > 205) {FragColor = vec4(1.0, 1.0, 1.0, 1.0);}
    //else if (radius > 195) {FragColor = vec4(0.0, 0.8, 0.0, 1.0);}
    //else {FragColor = vec4(0.2, 0.2, 0.2, 1.0);}
    //FragColor = vec4(abs(Normal), 1.0);
    //FragColor = vec4(FragPos, 1.0);
    //FragColor = vec4()

}