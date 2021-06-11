#version 410 core
out vec4 color;

in vec2 vertex_tex;

uniform bool bloom;
uniform float exposure;
uniform sampler2D screenTexture;
uniform sampler2D blurTexture;

void main()
{             
    const float gamma = 1.0;
    vec3 hdrColor = texture(screenTexture, vertex_tex).rgb;

    //color = vec4(hdrColor, 1.0);
    //return;

    vec3 bloomColor = texture(blurTexture, vertex_tex).rgb;

    if (bloom)
        hdrColor += bloomColor;

    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    result = pow(result, vec3(1.0 / gamma));
    color = vec4(result, 1.0);
}
