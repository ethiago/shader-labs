#version 400 compatibility

out vec3 N;

void main(void)
{		
    gl_Position = gl_Vertex;
	N = gl_Normal;
}