#version 410 core
out vec4 color;
in vec2 frag_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{

color.rgb = texture(tex, frag_tex).rgb;
//color.rg = frag_tex;
//color.b = 0;
color.rgb *= vec3(1.5, 0.5, 0.25);
color.r += 0.1;
color.a=1;
}
