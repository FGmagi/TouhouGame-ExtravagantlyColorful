#version 450 core
in vec2 texcoord;
in vec4 color;
in vec4 edge;
out vec4 col;

uniform sampler2D tex;
uniform ivec2 TexSize;
uniform vec4 outlinecolor;
uniform int bold;

float getAngel(float angel);
void main()
{    
    col = vec4(color.rgb,(1.0f - texture(tex, texcoord).r)*color.a);

    if (bold >= 1 && col.a != 1.0f){
        float count=0;
        count += getAngel(0.0f);
        count += getAngel(30.0f);
        count += getAngel(60.0f);
        count += getAngel(90.0f);
        count += getAngel(120.0f);
        count += getAngel(150.0f);
        count += getAngel(180.0f);
        count += getAngel(210.0f);
        count += getAngel(240.0f);
        count += getAngel(270.0f);
        count += getAngel(300.0f);
        count += getAngel(330.0f);
		col.a += count * color.a / 12.0f;
    }
}
// 判断在这个角度上距离为outlineSize那一点是不是透明
float getAngel(float angel){
    angel = angel * 0.01745329252; // 这个浮点数是 pi / 180，角度转弧度
	vec2 temp = vec2(texcoord.x + bold * cos(angel) / TexSize.x, texcoord.y + bold * sin(angel) / TexSize.y);
	temp.x = min(max(temp.x,edge.x),edge.z);
	temp.y = min(max(temp.y,edge.w),edge.y);
    return ((1.0f - texture2D(tex, temp).r));
}