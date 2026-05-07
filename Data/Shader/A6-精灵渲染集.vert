#version 450 core	//基础着色器

layout (location = 0) in mat4 Position;//位置+纹理
layout (location = 4) in float Opacity;//透明度

out vec3 temp; //前二位纹理坐标，后一位透明度

void main()
{
	gl_Position = vec4(Position[gl_VertexID % 4].xy,0.0f, 1.0f);
	temp = vec3(Position[gl_VertexID % 4].zw,Opacity);
}