#version 450 core   //开幕渐变专用着色器

uniform int Time;//时间
uniform float Middle;//中轴位置

out vec4 Color;//输出颜色

void main()
{
    float opacity = abs(float(gl_FragCoord.x - Middle) / Middle);//距中轴距离
    opacity = 1.0f + opacity - (float(Time) / 30.0f);//计算透明度
    opacity = min( max( opacity, 0.0f), 1.0f);//限制范围
    Color = vec4(0.0f, 0.0f, 0.0f, opacity);
}