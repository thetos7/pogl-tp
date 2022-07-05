#version 450

in vec2 vPosition;

uniform mat4 projection;
uniform mat4 model_transform;
uniform mat4 view_transform;

void main() {
    gl_Position = projection * view_transform * model_transform * vec4(vPosition.x, 0, vPosition.y, 1.0);
}