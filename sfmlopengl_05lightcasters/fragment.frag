#version 130

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform vec3 lightPos;
uniform vec3 lightSpotdir;
uniform float lightSpotCutOff;
uniform float lightSpotOuterCutOff;
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main() {
	float ambientStrength = 0.1f;

	vec3 norm = normalize(Normal);
	
	//point light
	vec3 lightDir = normalize(lightPos - FragPos);
	
	//directional light
	//vec3 lightDirection = vec3(-0.2f, -1.0f, -0.3f);
	//vec3 lightDir = normalize(-lightDirection);
	
	//spot light hard edge
	float theta = dot(lightDir, normalize(-lightSpotdir));
	
	//spot light soft edge
	float epsilon = lightSpotCutOff - lightSpotOuterCutOff;
	float intensity = clamp((theta - lightSpotOuterCutOff) / epsilon, 0.0, 1.0);
    

		vec3 ambient = ambientStrength * vec3(texture(diffuse, TexCoords)) * lightColor;
		
	  	//diffuse
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * vec3(texture(diffuse, TexCoords)) * lightColor;
		
		//specular
		float specularStrength = 1.0f;
		vec3 viewDir = normalize(viewPos - FragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
		vec3 specular = specularStrength * spec * vec3(texture(specular, TexCoords)) * lightColor;
		
		//attenuation
		float lightConstant = 1.0f;
		float lightLinear = 0.09f;
		float lightQuadratic = 0.032f;
		
		float distance = length(lightPos - FragPos);
		float attenuation = 1.0f / (lightConstant + lightLinear * distance + lightQuadratic * (distance * distance));  
		
		diffuse  *= intensity;
		specular *= intensity;
		
		ambient  *= attenuation; 
		diffuse  *= attenuation;
		specular *= attenuation;  

		//final light
		vec3 result = (ambient + diffuse + specular);
		color = vec4(result, 1.0f);

}
