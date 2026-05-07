#version 450 core   //处理黑色背景遮罩动画

in vec3 temp;//中转变量
uniform sampler2D Texture;//输入纹理
out vec4 Color;//输出颜色

void main()
{
	Color = texture(Texture, temp.xy);
	Color.a *= temp.z;
	Color.a *= min(1.0f,Color.r+Color.g+Color.b);
}