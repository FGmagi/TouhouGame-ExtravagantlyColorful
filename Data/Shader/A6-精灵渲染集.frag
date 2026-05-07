#version 450 core   //基础着色器

in vec3 temp;//中转变量
uniform sampler2D Texture;//输入纹理
out vec4 Color;//输出颜色

void main()
{
    Color = texture(Texture, temp.xy);
    Color.a *= temp.z;
}