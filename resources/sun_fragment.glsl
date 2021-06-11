#version 330 core
in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

out vec4 fragCol;

void main()
{
	vec3 baseColor = vec3(1.0, 1.0, 0.5) * 5;
	baseColor *= vec3(2.5, 0.25, 0.15);
	fragCol = vec4(baseColor, 1.0);
}

