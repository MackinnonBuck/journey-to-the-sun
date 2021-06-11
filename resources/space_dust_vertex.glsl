#version 410 core
layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec3 vertNor;
layout(location = 2) in vec2 vertTex;
layout(location = 3) in vec4 instancePos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec3 campos;

out vec3 vertex_pos;
out vec3 vertex_normal;
out vec2 vertex_tex;

const float PARTICLE_SPAN = 200.0;
const float PARTICLE_RANGE = PARTICLE_SPAN * 0.5;

void main()
{
	vec3 actualCampos = -campos;
	vec3 wrappedPos = instancePos.xyz;

	while (wrappedPos.x - actualCampos.x > PARTICLE_RANGE)
	{
		wrappedPos.x -= PARTICLE_SPAN;
	}

	while (wrappedPos.x - actualCampos.x < -PARTICLE_RANGE)
	{
		wrappedPos.x += PARTICLE_SPAN;
	}

	while (wrappedPos.y - actualCampos.y > PARTICLE_RANGE)
	{
		wrappedPos.y -= PARTICLE_SPAN;
	}

	while (wrappedPos.y - actualCampos.y < -PARTICLE_RANGE)
	{
		wrappedPos.y += PARTICLE_SPAN;
	}

	while (wrappedPos.z - actualCampos.z > PARTICLE_RANGE)
	{
		wrappedPos.z -= PARTICLE_SPAN;
	}

	while (wrappedPos.z - actualCampos.z < -PARTICLE_RANGE)
	{
		wrappedPos.z += PARTICLE_SPAN;
	}

	vertex_normal = vec4(M * vec4(vertNor,0.0)).xyz;
	vec4 pos = M * vec4(vertPos,1.0) + vec4(wrappedPos, 0.0);
	gl_Position = P * V * pos;
	vertex_tex = vertTex;	
}
