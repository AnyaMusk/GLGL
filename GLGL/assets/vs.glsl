#version 330 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;

uniform mat4 matrix;

out vec2 TexCoord0;

//vec4 colors[3] = vec4[3](vec4(1, 0, 0, 1), vec4(0, 1, 0, 1), vec4(0, 0, 1, 1));

void main(){
	//gl_Position = vec4(0.5 *Position.x, 0.5 * Position.y, Position.z, 1.0);
	gl_Position = matrix * vec4(Position, 1.0);
	//gl_Position = vec4(Position, 1.0);

	// gl_VertexID is runs per each vertex by gpu and it returns vertex id assigned by gpu as per your indexes i suppose
	//Color = colors[gl_VertexID];
	TexCoord0 = TexCoord;
}