#version 450 core	//颜色叠加器

out vec2 coord;

vec2 get_position();
vec2 get_coord();
void main()
{
	coord = get_coord();
	gl_Position = vec4(get_position(),0.0f,1.0f);
};
//自动定位
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