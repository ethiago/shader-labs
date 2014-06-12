uniform float shine;

void main()
{
	gl_FragColor = gl_Color;
	gl_FragColor.g = shine;
}