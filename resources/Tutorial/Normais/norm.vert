uniform sampler2D sampler2d1;

varying vec4 eye;
varying vec3 N;


float height(in vec2 tc)
{
	float d = texture2D(sampler2d1, tc).r;
	d -= 0.4;
	if(d < 0.0)
		d = 0.0;
	return d;
}

vec3 norm(float s, float t, float delta)
{
 	float xAnt = height(vec2(s-delta, t));
	float xPos = height(vec2(s+delta, t));
	float yAnt = height(vec2(s, t-delta));
	float yPos = height(vec2(s, t+delta));

	vec3 x = vec3(1.0, 0.0, xPos-xAnt);
	vec3 y = vec3(0.0, 1.0, yPos-yAnt);
	
	return normalize(cross(x,y));
}

void main()
{
	vec2 texCoord = gl_MultiTexCoord0.st;
	float disp = height(texCoord);
	vec4 modelCoord = gl_Vertex;
	modelCoord.z += disp;

	vec4 eyeCoord = gl_ModelViewMatrix * modelCoord;

	eye = eyeCoord;

	gl_Position = gl_ProjectionMatrix * eyeCoord;
	
	vec3 n = norm(texCoord.s, texCoord.t, 0.02);
	N = normalize(gl_NormalMatrix * n);

}