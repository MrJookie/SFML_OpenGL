#version 130

attribute vec3 position;
attribute vec2 texCoord;

varying vec2 f_texcoord;
varying vec3 f_color;

uniform mat4 mvp;

void main() {
	f_texcoord = texCoord;
	
	//gl_Position = vec4(position, 1.0f);
	gl_Position = mvp * vec4(position, 1.0);
}
