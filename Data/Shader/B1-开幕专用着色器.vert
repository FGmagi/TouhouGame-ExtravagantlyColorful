#version 450 core	//开幕渐变专用着色器

vec2 get_position();
void main()
{
	gl_Position = vec4(get_position(),0.0f,1.0f);
}
vec2 get_position() {
	float x = (gl_VertexID&0x2)-1;
	float y = (gl_VertexID-1&0x2)-1;
	return vec2(x,y);
};