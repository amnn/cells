#version 330 core

uniform usampler2DArray terrain;
out vec4 color;

void main() {
    vec4 texel = texelFetch(terrain, ivec3(gl_FragCoord.xy, 0), 0);
    color      = ((texel.r + 5u)/10.0) * vec4(0.75, 0.56, 0.34, 1.0);
}
