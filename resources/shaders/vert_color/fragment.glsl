#version 450

layout(location=0) out vec4 output_color;

uniform vec4 obj_color;
in vec3 vert_color;

void main() {
    output_color = vec4(vert_color, 1.0);
}