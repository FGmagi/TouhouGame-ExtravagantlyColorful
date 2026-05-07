#version 450 core	//矩形着色器

layout (location = 0) in vec2 Position;//位置
layout (location = 1) in vec3 Color;//颜色
layout (location = 2) in float Opacity;//透明度

out vec4 temp;//输出颜色

void main()
{
	gl_Position = vec4(Position,0.0f, 1.0f);
	temp = vec4(Color,Opacity);
}