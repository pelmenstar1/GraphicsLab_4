#version 450 core

in vec3 vertexPos;
in vec3 normal;

uniform vec3 materialColor;
uniform vec3 lightPos;
uniform vec3 eyePos;

out vec3 color;

void main()
{
	// Bling-Phong lightning model

	float lightIntensity = 0.8f;
	float objectShininess = 64.0f;
	float specularStrength = 1.0f;
	//vec3 lightColor = vec3(0.0, 0.0, 1.0);

	// normaliZe vectors
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - vertexPos);
	vec3 viewDir = normalize(eyePos - vertexPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);

	vec3 ambient = vec3(0.1);

	// diffuse 
	vec3 diff = vec3(max(dot(norm, lightDir), 0.0) * lightIntensity);

	// specular 
	
    //vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), objectShininess);
    vec3 specular = vec3(specularStrength * spec);  

	// The light loses its intensity the futher it travels
	float lightDist = distance(lightPos, vertexPos);
	float attenuation = 1.0 / (1.0 + 0.01*lightDist + 0.01*lightDist*lightDist); //min(2.0f / lightDist, 1.0);

	diff *= attenuation;
	specular *= attenuation;

	color = (ambient + diff + specular) * materialColor;
}