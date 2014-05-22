#version 150
#extension GL_EXT_geometry_shader4 : enable

vec2 v[4];

void drawLine(vec2 a, vec2 b)
{
	vec2 def = vec2(0.0, 1.0);
	gl_Position = vec4(a, def);
	EmitVertex();
	gl_Position = vec4(b, def);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	gl_FrontColor  = vec4(1.0);
	
	v[0] = vec2(-0.9, 0.9);
	v[1] = vec2(-0.5, -0.9);
	v[2] = vec2(0.5, 0.9);
	v[3] = vec2(0.9, -0.9);

	vec4 cor1 = vec4(1.0, 0.0, 0.0, 1.0);
	vec4 cor2 = vec4(1.0, 1.0, 1.0, 1.0);

	
	int nFrags = 20;
	
	bool impar = true;
	
	vec2 antF = v[0];

	for(int i = 1; i <= nFrags; i++)
	{
		float t = (float(i)/nFrags);

		vec2 temp = (1-t)*v[0] + t*v[1];
		vec2 temp2 = (1-t)*v[1] + t*v[2];
		vec2 temp3 = (1-t)*v[2] + t*v[3];

		vec2 ttemp = (1-t)*temp + t*temp2;
		vec2 ttemp2 = (1-t)*temp2 + t*temp3;

		vec2 tempF = (1-t)*ttemp + t*ttemp2;

		
		if(impar)	gl_FrontColor  = cor1;
		else	gl_FrontColor  = cor2;
		impar = !impar;
		

		drawLine(antF, tempF);
		

		antF = tempF;
	}

}