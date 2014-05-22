uniform sampler2D sampler2d0;
uniform sampler2D sampler2d1;
uniform sampler2D sampler2d2;
uniform float time;
void main()
{
	vec2 tc = gl_MultiTexCoord0.st;
	vec4 c1 = texture2D(sampler2d0,tc);
	vec4 c2 = texture2D(sampler2d1,tc);
	
	gl_FrontColor = c1*(1.0-time) + c2*time;
	gl_Position = gl_ModelViewProjectionMatrix*vec4(gl_Vertex.xyz*time,1.0);
}