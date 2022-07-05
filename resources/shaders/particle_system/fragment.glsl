#version 450

uniform sampler2DArray uTexture;
uniform float layer_count;

layout(location=0) out vec4 output_color;

in vec2 uv;
flat in float texId;

void main() {
    float layer = clamp(floor(texId + 0.5),0,layer_count - 1);
    vec4 tex_color = texture(uTexture, vec3(uv, layer));
    if(tex_color.a < 0.1)
        discard;
    output_color = tex_color;
}