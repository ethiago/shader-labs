varying float lightIntensity;
varying vec2 TexCoord;

void main()
{
	float cos_45 = sqrt(2.0)/2.0;
	vec3 normal = gl_Normal;
	vec4 vert = gl_Vertex;
	TexCoord = gl_MultiTexCoord0.xy;

	if(vert.x > 0.0)
	{
		vert.x = cos_45;
	}
	else
	{ 
		vert.x = -cos_45;
	}

	if(vert.y > 0.0)
	{
		vert.y = cos_45;
	}
	else 
	{
		vert.y = -cos_45;
	}

	if(vert.z > 0.0)
	{
		vert.z = cos_45;
	}
	else 
	{	
		vert.z = -cos_45;
	}


	vec3 ePosition = vec3(gl_ModelViewMatrix * vert);
	vec3 lPosition = vec3(gl_ModelViewMatrix * gl_LightSource[0].position);
	vec3 tnorm     = normalize(gl_NormalMatrix * normal);
	vec3 lightVec  = normalize(gl_LightSource[0].position.xyz - ePosition);
	//vec3 lightVec  = normalize(lPosition - ePosition);
	float diffuse  = dot(lightVec,tnorm);
	vec4 color =   gl_LightSource[0].diffuse * gl_Color * diffuse;
	gl_FrontColor = color;
	gl_BackColor = color;

	TexCoord = gl_MultiTexCoord0.xy;
	lightIntensity = diffuse;
	gl_Position = gl_ModelViewProjectionMatrix * vert;
}
