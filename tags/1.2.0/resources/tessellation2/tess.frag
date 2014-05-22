#version 400 compatibility

in vec3 cor;

void main()
{
	gl_FragColor = vec4(cor, 1.0);
}