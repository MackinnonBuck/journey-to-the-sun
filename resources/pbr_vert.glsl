#version 330 core

layout(location = 0) in vec4 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec3 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//uniform mat4 LS;

//out vec4 fPosLS;
out vec3 Normal;
out vec3 WorldPos;
out vec2 TexCoords;

void main()
{
	TexCoords = vertTex.xy;
	gl_Position = P * V * M * vertPos;
	Normal = (M * vec4(vertNor, 0.0)).xyz;
	WorldPos = vec3(M * vec4(vertPos.xyz, 1.0));

	//vec3 fPos = (M * vec4(vertPos.xyz, 1.0)).xyz;
	//fPosLS = LS * vec4(fPos, 1.0);
}
