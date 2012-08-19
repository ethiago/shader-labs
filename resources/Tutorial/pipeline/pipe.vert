vec4 calculaCor(in vec4 eyeCoord)
{
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec4 lv = normalize(gl_LightSource[0].position - eyeCoord);
	float diff = max(dot(n,lv.xyz),0.0);

	float spec = 0.0;
	if(diff > 0.0)
	{
		vec3 r = reflect(-lv.xyz,n);
		vec3 v = normalize(-eyeCoord.xyz);
		spec = max(dot(v, r), 0.0);
		spec = pow(spec, 16.0);
	}

	return gl_Color*0.2 + gl_Color*diff + vec4(spec);
}

void main()
{
	vec4 modelCoord = gl_Vertex;
	vec4 eyeCoord = gl_ModelViewMatrix * modelCoord;
	gl_Position = gl_ProjectionMatrix * eyeCoord;

	gl_FrontColor = calculaCor(eyeCoord);
	gl_FrontColor.a = 1.0;
}
