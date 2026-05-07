#version 450 core   //搭配基础着色器.vert使用，range作用于纹理坐标

in vec3 temp;//中转变量
uniform sampler2D Texture;//输入纹理
out vec4 Color;//输出颜色

uniform vec4 u_color;
uniform vec4 u_range;
void main()
{
    Color = texture(Texture, temp.xy);
    Color.a *= temp.z;
	if(temp.x > u_range.x && temp.x < u_range.z && temp.y > u_range.y && temp.y < u_range.w){
		Color = vec4(Color.rgb*(1.0f-u_color.a) + u_color.rgb * u_color.a,Color.a);
		//Color = (Color * (1.0f-u_color.a)) + Color.a * u_color;
	}
}