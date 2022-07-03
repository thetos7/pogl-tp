#version 450

in vec2 uv;
in vec3 normal;

layout(location=0) out vec4 color;

uniform sampler2D snow_height;
uniform sampler2D snow_texture;
uniform sampler2D under_texture;

uniform vec3 sun_color;
uniform float sun_intensity;
uniform vec3 sun_direction;

uniform vec3 ambient_color;
uniform float ambient_intensity;


void main() {
    float sun_illumination = dot(normal, -sun_direction);
    sun_illumination = clamp(sun_illumination, 0.0, 1.0);
    vec3 ambient = ambient_color * ambient_intensity;
    vec3 sun = sun_color * sun_intensity;

    vec4 illumination = vec4(mix(ambient, sun, sun_illumination), 1);
    
    float height = texture(snow_height, uv).r;
    float tex_mix_fact = clamp(height * 5.0, 0.0, 1.0);
    
    vec4 snow_color = texture(snow_texture, uv);
    vec4 under_color = texture(under_texture, uv);
    vec4 tex_color = mix(under_color, snow_color, tex_mix_fact);

    color = tex_color * illumination;
}