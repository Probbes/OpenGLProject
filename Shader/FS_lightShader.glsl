#version 330 core

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

struct DirLight {
    vec3 direction;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 vertexColor;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 lightPower;

uniform int matOrText;

uniform Material material;
uniform PointLight light[10];
uniform DirLight dirLight[4];

vec3 CalcPointLight(PointLight light);
vec3 CalcDirLight(DirLight light);

uniform int numberOfSun = 0;
uniform int numberOfPointLight = 0;

void main()
{
    vec3 result;
    for (int i=0; i < numberOfSun; i++){
        result = result + CalcDirLight(dirLight[i]);
    }
    if (numberOfPointLight != 0) {
        for (int j=0; j < numberOfPointLight; j++){
            result = result + CalcPointLight(light[j]);
        }
    }

    vec4 texColor = texture(texture_diffuse1, TexCoords);
    if(texColor.a < 0.1)
        discard;
    FragColor = vec4(result, 1.0);
    //FragColor = vec4(abs(Normal), 1.0);
    //FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
    //FragColor = vec4(0.8,0.8,0.8,1.0);

}

vec3 CalcPointLight(PointLight light)
{
    vec3 ambient = vec3(0);
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);
    float spec = 0.0;

    vec3 lightDir = normalize(light.position - FragPos);

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 

    float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)); 

    if (matOrText == 1){    //If object has nos texture -> use material
        ambient = vec3(0.1) * light.ambient * material.ambient;
        diffuse = vec3(0.5) * lightPower * light.diffuse * (diff * material.diffuse);
    
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular = vec3(1.0) * lightPower * light.specular * (spec * material.specular);
    }

    else if (matOrText == 0){   //If object has texture
        ambient = vec3(0.1) * light.ambient * texture(texture_diffuse1,TexCoords).rgb;
        diffuse = vec3(0.5) * lightPower * light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

        spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);
        specular = vec3(1.0) * lightPower * light.specular * spec * texture(texture_specular1, TexCoords).rgb;
    }

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirLight light)
{
    vec3 ambient = vec3(0);
    vec3 diffuse = vec3(0);
    vec3 specular = vec3(0);
    float spec = 0.0;

    vec3 lightDir = -normalize(light.direction);

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); 
    spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.f);

    if (matOrText == 1){    //If object has nos texture -> use material
        ambient = vec3(0.1) * light.ambient * material.ambient;
        diffuse = vec3(0.5) * lightPower * light.diffuse * (diff * material.diffuse);
        specular = vec3(1.0) * lightPower * light.specular * (spec * material.specular);
    }

    else if (matOrText == 0){   //If object has texture
        ambient = vec3(0.1) * light.ambient * vec3(texture(texture_diffuse1,TexCoords));
        diffuse = vec3(0.5) * lightPower * light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
        specular = vec3(1.0) * lightPower * light.specular * spec * vec3(texture(texture_specular1, TexCoords));
    }

    return (ambient + diffuse + specular);
}  