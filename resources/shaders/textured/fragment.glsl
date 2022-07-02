#version 450

in vec2 uv;

layout(location=0) out vec4 color;

uniform sampler2D uTexture;

void main() {
    vec4 tex_color = texture(uTexture, uv);
    if(tex_color.a < 0.1)
        discard;
    color = tex_color;
}