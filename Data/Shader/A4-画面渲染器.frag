#version 450 core   //画面渲染器（实时渲染）

in vec2 temp;//纹理坐标

uniform sampler2D Texture;//输入纹理

out vec4 Color;//输出颜色

void main(){
    Color = texture(Texture, temp);
    //Color = vec4(0.8f,0.8f,0.8f,1.0f);
}
