#version 330 core

struct Material
{
//    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform float time;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTexelColor = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * specTexelColor * spec;

    vec3 emission = (1.0f - sign(specTexelColor)) * vec3(texture(material.emission, TexCoords * 0.6f + vec2(0.5f, time / 7.0f)));
    vec3 emission2 = (1.0f - sign(specTexelColor)) * vec3(texture(material.emission, TexCoords + vec2(0.8f, time / 10.0f)));

    FragColor = vec4((ambient + diffuse + specular + emission * 0.8f + emission2 * 0.5f), 1.0f);
};
