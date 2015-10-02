#version 130

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main() {
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * vec3(texture(diffuse, TexCoords)) * lightColor;

	vec3 norm = normalize(Normal);
	
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	
	vec3 diffuse = diff * vec3(texture(diffuse, TexCoords)) * lightColor;
	
	float specularStrength = 1.0f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * vec3(texture(specular, TexCoords)) * lightColor;

	vec3 result = (ambient + diffuse + specular);
	color = vec4(result, 1.0f);
}
