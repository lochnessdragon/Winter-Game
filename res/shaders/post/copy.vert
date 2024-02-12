// copy post-processing shaders simply output a triangle that they read in, no transformations needed

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 vUv;

uniform mat4 modelMat;

out vec2 uv;

void main()
{
    gl_Position = modelMat * vec4(pos.x, pos.y, 0.0, 1.0); 
    uv = vUv;
}  