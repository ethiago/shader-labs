
attribute float confidence;

uniform float scale;


void main()
{
	vec4 v = gl_Vertex;

	gl_FrontColor = vec4(confidence, 0.0, 0.0, 1.0);

	gl_Position = gl_ModelViewProjectionMatrix * v;
}