#version 330 core

uniform usampler2DRect tex;

out vec4 color;

void main() {

	color = vec4( texture( tex, gl_FragCoord.xy ).rrr, 1 ); 

}