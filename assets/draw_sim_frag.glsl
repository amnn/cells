#version 330 core

uniform usampler2DArray terrain;
out vec4 color;

#define TERRAIN_SHADE vec4(0.75, 0.56, 0.34, 1.0)
#define ENERGY_SHADE  vec3(0.16, 0.56, 0.99)

void main() {
    vec4 texel = texelFetch(terrain, ivec3(gl_FragCoord.xy, 0), 0);
    color      = ((texel.r + 5u)/10.0) * TERRAIN_SHADE;

    if(texel.g > 0)
    {
        float amp = texel.g / 10.0;
        color.rgb = mix(color.rgb, ENERGY_SHADE * amp, amp);
    }
}
