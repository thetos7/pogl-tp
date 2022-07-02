#version 450

in vec3 vPosition;

uniform mat4 projection;
uniform mat4 model_transform;
uniform mat4 view_transform;


void main() {
    gl_Position = projection * view_transform * model_transform * vec4(vPosition,1.0);
}