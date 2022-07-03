#version 450

in vec3 vPosition;
in vec2 vUV;
in vec3 vNormal;

uniform mat4 projection;
uniform mat4 model_transform;
uniform mat4 view_transform;

uniform vec3 up;
uniform float scale;

uniform sampler2D snow_height;

out vec2 uv;
out vec3 normal;

void main() {
    uv = vUV;
    normal = vNormal;
    vec4 global_pos = model_transform * vec4(vPosition,1.0);
    float height = texture(snow_height, uv).r;
    global_pos.xyz /= global_pos.w;
    global_pos.xyz += scale * up * height;
    gl_Position = projection * view_transform * global_pos;
}
