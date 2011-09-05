void main()
{
	vec4 vert = gl_Vertex;
	float scale = 1.0;
	vert.x *= scale;
	vert.y *= scale;
	vert.z *= scale;
	vec4 cor2;

	if(vert.x < 0.0)
	{
		if(vert.y < 0.0) cor2 = vec4(1, 0, 0, 0);
		else cor2 = vec4(0, 1, 0, 0);
	}
	else
	{
		if(vert.y < 0.0) cor2 = vec4(0, 0, 1, 0);
		else cor2 = vec4(0, 1, 1, 0);
	} 
	
 	gl_Position = gl_ModelViewProjectionMatrix * vert;
	vec4 tmp = vec4(2,2,2,1);

	vec3 ePosition = vec3(gl_ModelViewMatrix * gl_Vertex);
	vec3 lPosition = vec3(gl_ModelViewMatrix * tmp);
	vec3 tnorm     = normalize(gl_NormalMatrix * gl_Normal);
	vec3 lightVec  = normalize(tmp.xyz - ePosition);
	//vec3 lightVec  = normalize(lPosition - ePosition);
	float diffuse  = dot(lightVec,tnorm);
	vec4 color =   gl_LightSource[0].diffuse * cor2 * diffuse;
	gl_FrontColor = color;
	//gl_BackColor = color;
}
