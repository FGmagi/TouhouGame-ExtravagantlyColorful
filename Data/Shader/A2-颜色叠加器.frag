#version 450 core   //颜色叠加器

uniform vec4 RGBA;
/*in vec2 coord;
uniform sampler2D Texture;*/

out vec4 Color;//输出颜色

float fuck(float n);
void main(){
    /*Color = texture(Texture, coord);
    Color.r = fuck(Color.r + RGB.r);
    Color.g = fuck(Color.g + RGB.g);
    Color.b = fuck(Color.b + RGB.b);*/
    Color =  RGBA;
}
float fuck (float n){
    n = min(n,1.0f);
    n = max(n,-1.0f);
    return n;
}