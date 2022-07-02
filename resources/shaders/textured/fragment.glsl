#version 450

in vec2 uv;

layout(location=0) out vec4 color;

uniform sampler2D uTexture;

void main() {
    color = texture(uTexture, uv);
}