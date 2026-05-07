#version 450 core	//画面渲染器 合成到最终帧

out vec2 temp;//纹理坐标

vec2 get_position();
vec2 get_coord();

void main()
{
	gl_Position = vec4(get_position(),0.0f,1.0f);
	temp = get_coord();
}
vec2 get_position() {
	float x = (gl_VertexID&0x2)-1;
	float y = (gl_VertexID-1&0x2)-1;
	return vec2(x,y);
};
vec2 get_coord() {
	float x = gl_VertexID>>1;
	float y = (gl_VertexID-1&0x2)>>1;
	return vec2(x,y);
}