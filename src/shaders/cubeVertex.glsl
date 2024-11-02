#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec4 FragPosV4;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;
out vec3 LightDir;

uniform mat4 model;
uniform mat4 view;
uniform mat4 cameraView;
uniform mat4 projection;
uniform vec3 lightPos;
uniform vec3 lightDir;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    FragPosV4 = cameraView * vec4(aPos, 1.0f);

    LightPos = vec3(view * vec4(lightPos, 1.0f));
    Normal = transpose(inverse(mat3(view * model))) * aNormal;
    TexCoords = aTexCoords;

    LightDir = vec3(view * vec4(lightDir, 0.0f));
};