#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 2) in vec2 vertTex;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
out vec2 frag_tex;
void main()
{
	gl_Position = P * V * M * vec4(vertPos, 1.0);
	frag_tex = vertTex;
}
