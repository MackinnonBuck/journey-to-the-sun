#version 410 core
out vec4 color;
in vec2 frag_tex;
uniform vec3 campos;

uniform sampler2D tex;
uniform sampler2D tex2;

void main()
{
color.rgb = texture(tex, frag_tex).rgb;
color.rgb *= vec3(3.0, 0.8, 0.5);
color.r += 0.08;
color.a=1;
}
