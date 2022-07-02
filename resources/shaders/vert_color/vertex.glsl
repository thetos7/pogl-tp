#version 450

in vec3 vPosition;
in vec3 vColor;

uniform mat4 projection;
uniform mat4 model_transform;
uniform mat4 view_transform;

out vec3 vert_color;

void main() {
    vert_color = vColor;
    gl_Position = projection * view_transform * model_transform * vec4(vPosition,1.0);
}