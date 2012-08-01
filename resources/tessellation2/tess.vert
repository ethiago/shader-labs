#version 400 compatibility


out vec2 texCoordIn;

void main(void)
{		
    gl_Position = gl_Vertex;

	if(gl_Vertex.x < -0.9)
		texCoordIn = vec2(0.0, 0.0);
	else if(gl_Vertex.x > 0.9)
		texCoordIn = vec2(1.0, 1.0);
	else if(gl_Vertex.y < -0.9)
		texCoordIn = vec2(1.0, 0.0);
	else if(gl_Vertex.y > 0.9)
		texCoordIn = vec2(0.0, 1.0);
	else
		texCoordIn = vec2(0.5, 0.5);

}