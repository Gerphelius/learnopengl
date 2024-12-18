#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // TODO: Pass precomputed MVP matrix instead of separated ones?
    gl_Position = projection * view * model * vec4(aPos, 1.0f);

    FragPos = vec3(view * model * vec4(aPos, 1.0f));
    Normal = normalize(transpose(inverse(mat3(view * model))) * aNormal);
    TexCoords = aTexCoords;
};