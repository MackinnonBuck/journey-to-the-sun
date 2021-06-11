#version 330 core
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform sampler2D tex;

out vec4 fragCol;

void main()
{
	vec4 tcol = texture(tex, vertex_tex);
	fragCol = tcol;
}


