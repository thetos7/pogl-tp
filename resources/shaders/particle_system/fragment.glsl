#version 450

uniform sampler2D uTexture;

layout(location=0) out vec4 output_color;

in vec2 uv;

void main() {
    vec4 tex_color = texture(uTexture, uv);
    if(tex_color.a < 0.1)
        discard;
    output_color = tex_color;
}