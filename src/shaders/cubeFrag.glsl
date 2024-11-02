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
    vec3 attenuation;
    float spotCutoffIn;
    float spotCutoffOut;
};

in vec3 FragPos;
in vec4 FragPosV4;
in vec3 Normal;
in vec3 LightPos;
in vec2 TexCoords;
in vec3 LightDir;

out vec4 FragColor;

uniform vec3 lightColor;
uniform Material material;
uniform Light light;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    float dist = length(LightPos - FragPos);
    float attenuation = 1 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * pow(dist, 2));

//    vec3 lightDir = normalize(LightDir); // Directional light
//    vec3 lightDir = normalize(LightPos - FragPos); // Point light
//    vec3 norm = normalize(Normal);
//    float diff = max(dot(norm, lightDir), 0.0f);
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightDir);
    vec3 fragDir = normalize(LightPos - FragPos);

    // Spotlight
    float theta = dot(fragDir, lightDir);
    float intencity = smoothstep(0.0, 1.0, (theta - light.spotCutoffOut) / (light.spotCutoffIn - light.spotCutoffOut));

    float sideTheta = dot(norm, fragDir);
    float diff = smoothstep(0.0, 1.0, (sideTheta - cos(radians(90.0))) / (cos(radians(85.0)) - cos(radians(90.0))));

//    float diff = max(sign(dot(norm, fragDir)), 0.0);
    vec2 texCoords = FragPosV4.xy / dist * 2 + 0.5;

    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)) * vec3(texture(material.emission, clamp(texCoords, 0.0, 1.0)));

    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTexelColor = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * specTexelColor * spec;

    FragColor = vec4(ambient + (diffuse + specular) * intencity * attenuation, 1.0);
};
