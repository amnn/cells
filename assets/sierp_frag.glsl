#version 330 core

in  vec2 texUV;
out vec3 color;

void main() {

	color = (int(texUV.x) & int(texUV.y)) == 0 ? vec3(0) : vec3(1);

}