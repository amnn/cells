#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec4 vPos;

out vec3 fragCol;

void main() {
	
	gl_Position = MVP * vPos;

}