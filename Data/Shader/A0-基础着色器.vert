#version 450 core	//基础着色器

layout (location = 0) in vec2 Position;//位置
layout (location = 1) in vec2 TexCoord;//纹理坐标
layout (location = 2) in float Opacity;//透明度

out vec3 temp; //前二位纹理坐标，后一位透明度

void main()
{
	gl_Position = vec4(Position,0.0f, 1.0f);
	temp = vec3(TexCoord,Opacity);
}