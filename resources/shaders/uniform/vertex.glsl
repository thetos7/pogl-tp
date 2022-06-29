#version 450

in vec3 vPosition;

uniform mat4 projection_matrix;
uniform mat4 model_transform_matrix;
uniform mat4 view_transform_matrix;


void main() {
    gl_Position = projection_matrix * view_transform_matrix * model_transform_matrix * vec4(vPosition,1.0);
}