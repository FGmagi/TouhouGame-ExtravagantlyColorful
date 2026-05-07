#version 450 core   //色相HSL着色器（离线渲染）

in vec4 hsl;
in vec2 Coord;

uniform sampler2D Texture;

out vec4 Color;

vec3 RGB_to_HSL(float R,float G,float B);
vec3 HSL_to_RGB(float H,float S, float L);

void main(){
    Color = texture(Texture, Coord);
    Color.rgb = RGB_to_HSL(Color.r, Color.g, Color.b);
    Color.rgb = HSL_to_RGB(Color.r, Color.g, Color.b);
}

vec3 RGB_to_HSL(float R,float G,float B){
    float H,S,L,low,top,del,dr,dg,db;
    low = min(min(R,G),B);
    top = max(max(R,G),B);
    del = top - low;
    L = top + low;

    S = L*sign(1-L)+2*step(1,L);
    S = del/(S+sign(S)-1);
    L = 0.5*L;

    del = 1/(6*(del+sign(del)-1));
    dr = (top-R)*del+0.5;
    dg = (top-G)*del+0.5;
    db = (top-B)*del+0.5;

    H = (db-dg)*(1-step(0.000001,top-R));
    H += (1.0/3.0+dr-db)*(1-step(0.000001,top-G));
    H += (2.0/3.0+dg-dr)*(1-step(0.000001,top-B));

    H += step(0,-H)-step(1,H);
    H *= sign(del);
    return vec3(H,S,L);
}
float F(float v1,float v2,float vh){
    float back,t1,t2,t3;
    vh += step(0,-vh);
    vh -= step(1,vh);
    t1 = 1-step(1.0/6.0,vh);
    t2 = (1-step(1.0/2.0,vh))*(1-t1);
    t3 = (1-step(2.0/3.0,vh))*(1-t1-t2);
    back = t1*(v1+6*vh*(v2-v1));
    back += t2*v2;
    back += t3*(v1+6*(2.0/3.0-vh)*(v2-v1));
    back += v1*(1-t1-t2-t3);
    return back;
}
vec3 HSL_to_RGB(float H,float S, float L){
    float R,G,B,v1,v2;
    H = Color.r * hsl.r;
    S = Color.g * hsl.g;
    L = Color.b * hsl.b;
    H += step(0,-H);
    H -= step(1,H);

    v2 = L + L * S + step(0.5,L)*(S-2*S*L);
    v1 = 2 * L - v2;
    R = F(v1,v2,H+1.0/3.0)+1-step(0,S);
    G = F(v1,v2,H)+1-step(0,S);
    B = F(v1,v2,H-1.0/3.0)+1-step(0,S);
    return vec3(R,G,B);
}