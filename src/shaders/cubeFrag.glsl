#version 330 core

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight
{
    vec3 position;

    vec3 color;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform Material material;
uniform PointLight pointLights[2];

vec3 GetPointLightColor(PointLight light, vec3 normal, vec3 fragPos);

void main()
{
    vec3 outputColor = vec3(0.0);

    for (int i = 0; i < pointLights.length; i++)
    {
        outputColor += GetPointLightColor(pointLights[i], Normal, FragPos);
    }

    FragColor = vec4(outputColor, 1.0);
};

vec3 GetPointLightColor(PointLight light, vec3 normal, vec3 fragPos)
{
    float dist = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specTexelColor = vec3(texture(material.specular, TexCoords));
    vec3 specular = light.specular * specTexelColor * spec;

    return (diffuse + specular) * attenuation * light.color;
};


//    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//
//    float dist = length(LightPos - FragPos);
//    float attenuation = 1 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * pow(dist, 2));
//
////    vec3 lightDir = normalize(LightDir); // Directional light
////    vec3 lightDir = normalize(LightPos - FragPos); // Point light
////    float diff = max(dot(Normal, lightDir), 0.0f);
////    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//
//    vec3 lightDir = normalize(LightDir);
//    vec3 fragDir = normalize(LightPos - FragPos);
//
//    // Spotlight
//    float theta = dot(fragDir, lightDir);
//    float intencity = smoothstep(0.0, 1.0, (theta - light.spotCutoffOut) / (light.spotCutoffIn - light.spotCutoffOut));
//
//    float sideTheta = dot(Normal, fragDir);
//    float diff = smoothstep(0.0, 1.0, (sideTheta - cos(radians(90.0))) / (cos(radians(85.0)) - cos(radians(90.0))));
//    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
//
//    vec3 viewDir = normalize(-FragPos);
//    vec3 reflectDir = reflect(-lightDir, Normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specTexelColor = vec3(texture(material.specular, TexCoords));
//    vec3 specular = light.specular * specTexelColor * spec;

//    FragColor = vec4(ambient + (diffuse + specular) * intencity * attenuation, 1.0);
