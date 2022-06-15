#version 450

layout(location=0) out vec4 output_color;

uniform vec4 obj_color;

void main() {
    output_color = obj_color;
}