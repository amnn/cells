#version 330 core

uniform usampler2DRect tex;
out vec4 color;

void main() {
    color = ((texture(tex,gl_FragCoord.xy).r + 1u)/10.0) * vec4(0.75, 0.56, 0.34, 1.0);
}
