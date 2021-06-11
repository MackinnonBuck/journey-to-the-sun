#version 410 core

out vec4 color;

in vec3 vertex_normal;
in vec3 vertex_pos;
in vec2 vertex_tex;
in float vertex_age;

uniform vec3 campos;
uniform ivec2 tileOffset;
uniform ivec2 nextTileOffset;
uniform float lerpAmount;
uniform sampler2D tex;
uniform sampler2D tex2;

const int X_TILE_COUNT = 4;
const int Y_TILE_COUNT = 4;
const vec2 TEX_COORD_SCALE = vec2(1.0f / X_TILE_COUNT, 1.0f / Y_TILE_COUNT);

void main()
{
	vec4 tcol = texture(tex, TEX_COORD_SCALE * (vertex_tex + tileOffset));
	vec4 next_tcol = texture(tex, TEX_COORD_SCALE * (vertex_tex + nextTileOffset));

	tcol.a *= (1.0f - lerpAmount);
	next_tcol.a *= lerpAmount;

	tcol.rgb *= tcol.a;
	next_tcol.rgb *= next_tcol.a;

	color = tcol + next_tcol;
	color.rgb *= 3;
	color.a *= 0.5;
	color.a /= (vertex_age * 50 + 1);

	if (color.a < 0.025f)
	{
		discard;
	}
}
