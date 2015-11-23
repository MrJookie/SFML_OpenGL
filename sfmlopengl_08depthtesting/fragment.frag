#version 130

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuse;
uniform sampler2D specular;

float near = 1.0; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main() {
	vec3 lightColor = vec3(1.0f, 0.5f, 1.0f);
	
	float ambientStrength = 1.0f;
	vec3 ambient = ambientStrength * vec3(texture(diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(texture(diffuse, TexCoords));
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * vec3(texture(specular, TexCoords));  

	vec3 result = (ambient + diffuse + specular);

	/*
	if((int(gl_FragCoord.x) % 30) == 0 && (int(gl_FragCoord.y) % 30) == 0)
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	else
	*/
    
    color = vec4(result, 1.0f);
    
    /*
    float depth = LinearizeDepth(gl_FragCoord.z) / far;
    color = vec4(vec3(depth), 1.0f);
    */
}
