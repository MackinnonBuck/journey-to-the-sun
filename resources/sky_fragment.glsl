#version 410 core
out vec4 color;
in vec2 frag_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
color.rgb = texture(tex, frag_tex).rgb;
color.rgb *= 0.5;
//color.rg = frag_tex;
//color.b = 0;
color.a=1;
}
