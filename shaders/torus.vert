#version 450 core

layout(location = 0) in vec3 aVert;
layout(location = 1) in vec3 aNorm;

uniform mat4 viewMat;
uniform mat4 projMat;
uniform mat4 normalMat;
uniform mat4 modelMat;

uniform vec3 lightPos;

out vec3 vertexPos;
out vec3 normal;

void main()
{
	vertexPos = vec3(modelMat * vec4(aVert, 1.0));
    normal = (normalMat * vec4(aNorm, 1.0)).xyz;
    
    gl_Position = projMat * viewMat * vec4(vertexPos, 1.0);
}