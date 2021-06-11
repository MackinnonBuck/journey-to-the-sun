#version 410 core

out vec4 color;

in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;

uniform vec3 campos;
uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
	vec2 adjustedTexCoords = vertex_tex * 2 - vec2(1, 1);
	float alpha = 1 - length(adjustedTexCoords);

	if (alpha <= 0)
	{
		discard;
	}

	color = vec4(1, 1, 1, alpha);
}
