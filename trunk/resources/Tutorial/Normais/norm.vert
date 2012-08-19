uniform sampler2D sampler2d1;

varying vec2 texCoordIn;
varying vec3 nIn;
varying vec3 lvIn;

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

	vec3 x = vec3((xPos-xAnt)/(2.0*delta), 0.0, 0.0);
	vec3 y = vec3(0.0, (yPos-yAnt)/(2.0*delta), 0.0);
	
	return (cross(x,y));
}

void main()
{
	texCoordIn = gl_MultiTexCoord0.st;
	float disp = height(texCoordIn);
	vec4 modelCoord = gl_Vertex;
	modelCoord.z += disp;

	vec4 eyeCoord = gl_ModelViewMatrix * modelCoord;

	lvIn = normalize((gl_LightSource[0].position - eyeCoord).xyz);
	
	gl_Position = gl_ProjectionMatrix * eyeCoord;
	
	nIn = norm(texCoordIn.s, texCoordIn.t, 0.02);
	nIn = normalize(gl_NormalMatrix * nIn);
}