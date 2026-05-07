#version 450 core	//色相HSL着色器

uniform vec3 HSL;

out vec4 hsl;
out vec2 Coord;

vec2 get_position();
vec2 get_coord();
void main()
{
	hsl = vec4(HSL,1.0f);
	gl_Position = vec4(get_position(),0.0f,1.0f);
	Coord = get_coord();
}
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