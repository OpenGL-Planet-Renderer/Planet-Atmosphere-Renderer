#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};


struct DirLight {
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);


uniform Material material;
uniform DirLight dirLight;
uniform vec3 viewPos;
uniform bool blinn;


void main()
{
    //properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    //phase 1: directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    FragColor = vec4(result, 1.0);
    
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    
    vec3 lightDir = normalize(-light.direction);
    
    //diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    
    //specular shading
    float spec = 0.0;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess*2.5f);
    }
    else {
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }
    
    //combine results
    vec3 ambient = light.ambient * vec3(texture(material.ambient, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
    
}


