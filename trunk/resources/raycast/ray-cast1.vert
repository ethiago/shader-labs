varying vec3 center;
varying vec3 vv;
varying vec3 lp2;

void main ()
{
	vec4 vert = gl_Vertex;
	vec3 norm = gl_Normal;
	vec3 color = gl_Color.xyz;
	vec3 lv;
	vec4 lp = gl_LightSource[0].position;
	lp2 = lp.xyz;
	float fat;

	vec4 c = vec4(0.0,0.0,0.0,1.0);

	vert = gl_ModelViewMatrix * vert;
	norm = normalize(gl_NormalMatrix * norm);
   center = (gl_ModelViewMatrix * c).xyz;

   vv = normalize(vert.xyz);

	lv = normalize(lp.xyz - vert.xyz);

	gl_FrontColor = gl_Color;

	gl_Position = gl_ProjectionMatrix * vert;
}