#version 330 core

// should be same name as the vertex shader
in vec2 TexCoord0;
out vec4 FragColor;
uniform sampler2D sampler;


void main(){
	// sampler indexes the tex whose index is assigned
	FragColor = texture2D(sampler, TexCoord0);
}