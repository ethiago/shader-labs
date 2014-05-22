varying vec4 pi;

void main ()
{
	pi = gl_Vertex;
	gl_FrontColor = gl_Color;
	gl_Position = ftransform();
}