// copy post-processing shaders simply output a triangle that they read in, no transformations needed

in vec2 uv;

uniform sampler2D texture;

void main()
{ 
    gl_FragColor = texture(texture, uv);
}