#version 330 core
struct Material {
    sampler2D diffuseTexture;
	 sampler2D specularTexture;
    vec3 ambient;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuseTexture, TexCoords) )* material.diffuseColor;
    //vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    // lol I can change colors of this thing.
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuseTexture, TexCoords) ) * material.diffuseColor;

    //vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specularTexture, TexCoords))) * material.specularColor;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0f);
}
