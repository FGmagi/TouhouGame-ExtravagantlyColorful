#version 450 core
layout (location = 0) in vec4 pos;
layout (location = 1) in vec4 tex;
layout (location = 2) in vec4 col;
uniform int italic;
uniform int slender;
out vec2 texcoord;
out vec4 edge;
out vec4 color;

void main() {
    vec2 a,b;
    if (gl_VertexID % 4 == 0){//上
      a = pos.xy;
      b = tex.xy;
      if (italic == 1) a.x += 0.025f;
    }
    if (gl_VertexID % 4 == 1){//下
      a = pos.xw;
      b = tex.xw;
    }
    if (gl_VertexID % 4 == 2){//上
      a = pos.zy;
      b = tex.zy;
      if (italic == 1) a.x += 0.025f;
      if (slender == 1) a.x -= 0.007f;
   }
    if (gl_VertexID % 4 == 3){//下
      a = pos.zw;
      b = tex.zw;
      if (slender == 1) a.x -= 0.007f;
    }
    gl_Position = vec4(a, 0.0, 1.0);
    texcoord = vec2(b);
    color = col;
	edge = tex;
}