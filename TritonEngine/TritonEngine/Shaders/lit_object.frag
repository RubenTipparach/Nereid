#version 330 core
out vec4 color;

//uniform float ambientStrength;  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;  

uniform vec3 viewPos; //this will be using world space, view space will have to modify light pos

in vec3 Normal;  
in vec3 FragPos;  

void main()
{
	float specularStrength = 2f;

	// normal calculation
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	// ambient
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	// dot product
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
    
	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);

    //color = vec4(lightColor * objectColor, 1.0f);
}