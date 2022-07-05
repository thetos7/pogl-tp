#version 450

in vec3 vPosition;
in vec2 vUV;
in float vTexId;

uniform mat4 projection;
uniform mat4 model_transform;
uniform mat4 view_transform;

out vec2 uv;
out flat float texId;

void main() {
    gl_Position = projection * view_transform * model_transform * vec4(vPosition, 1.0);
    uv = vUV;
    texId = vTexId;
}