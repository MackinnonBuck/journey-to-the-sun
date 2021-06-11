#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec4 instancePos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float totalTime;
out vec3 vertex_pos;
out vec3 vertex_normal;
out vec2 vertex_tex;
out float vertex_age;

void main()
{
	vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;
	vertex_age = totalTime - instancePos.w;
	vec3 newVertPos = vertPos / (1 + vertex_age * 5);
	//vec3 newVertPos = vertPos * (0.5 + vertex_age * 15);
	vec4 pos = M * vec4(newVertPos,1.0) + vec4(instancePos.xyz, 0.0);
	pos.x += vertex_age * 10;
	pos.y += vertex_age * 5;
	pos.z -= vertex_age * 2.5;
	gl_Position = P * V * pos;
	vertex_tex = vertTex;	
}
