#version 330 core

#define MAX_LIGHTS 8

struct Material {
    vec3 tint;
    float shininess;
    sampler2D specularMap;
    sampler2D diffuseMap;
    sampler2D emissionMap;
};

struct DirLight {
    vec3 direction;

    vec3 diffuse;
    vec3 specular;
    float intensity;
};

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;
    float intensity;
    float range;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float innerCutOff;
    float outerCutOff;

    vec3 diffuse;
    vec3 specular;

    float intensity;
    float range;
};

//FROM VERTEX SHADER
in vec4 ourColor;
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

//OUT FRAGMENT SHADER
out vec4 FragColor;

//UNIFORMS
uniform Material material;
uniform int numPointLights;
uniform DirLight directionalLight;
uniform PointLight pointLights[MAX_LIGHTS];

uniform int numSpotLights;
uniform SpotLight spotlights[MAX_LIGHTS];

uniform vec3 cameraPos;
uniform vec3 ambientLight;

//FUNCTIONS
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 fragmentNormal = normalize(normal);
    vec3 fragToCameraVector = normalize(cameraPos - fragPos);
    vec3 ambientLight = vec3(texture(material.diffuseMap, texCoord)) * ambientLight;

    vec3 result = ambientLight;
    result += CalcDirLight(directionalLight, fragmentNormal, fragToCameraVector);

    for (int i = 0; i < min(numPointLights, MAX_LIGHTS); i++) {
        result += CalcPointLight(pointLights[i], fragmentNormal, fragPos, fragToCameraVector);
    }

    for (int i = 0; i < min(numSpotLights, MAX_LIGHTS); i++) {
        result += CalcSpotLight(spotlights[i], fragmentNormal, fragPos, fragToCameraVector);
    }

    FragColor = vec4(result, 1);

    //Color precision
    FragColor.rgb = round(FragColor.rgb * 32) / 32;
};



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // combine results
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, texCoord
    ));
    return (diffuse + specular) * light.intensity;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0;
    float linear = 0.09;
    if (distance > light.range) {
        attenuation = 0.0;
    } else {
        float factor = 1.0 - (distance / light.range);
        attenuation = 1.0 / (1.0 + linear * distance) * factor;
    }

    // combine results
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specularMap, texCoord));

    diffuse *= attenuation;
    specular *= attenuation;

    return (diffuse + specular) * light.intensity;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));


    if (theta > light.outerCutOff)
    {
        float diff = max(dot(normal, lightDir), 0.0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        // attenuation
        float distance = length(light.position - fragPos);
        float attenuation = 1.0;
        float linear = 0.09;
        if (distance > light.range) {
            attenuation = 0.0;
        } else {
            float factor = 1.0 - (distance / light.range);
            attenuation = 1.0 / (1.0 + linear * distance) * factor;
        }

        //Apply smoothing
        float epsilon = light.innerCutOff - light.outerCutOff;
        float smoothIntensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

        // combine results
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseMap, texCoord));
        vec3 specular = light.specular * spec * vec3(texture(material.specularMap, texCoord));

        diffuse *= smoothIntensity * attenuation;
        specular *= smoothIntensity * attenuation;

        return (diffuse + specular) * light.intensity;
    }
    else {
        return vec3(0);
    }
}



