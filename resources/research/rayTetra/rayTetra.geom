#version 150
#extension GL_ARB_geometry_shader4 : enable

in mat4 Q0[];
in mat4 Q1[];
in mat4 Q2[];
in mat4 Q3[];
in mat4 osTetra[];
in mat4 psTetra[];

out mat4 Qi;
out mat4 Qo;
out vec4 pos;

mat4 Q[4];
mat4 ssTetra;

ivec4 sort(void)
{
	vec3 n = vec3(1.0,0.0,0.0);
	ivec4 idx = ivec4(0,1,2,3);
	vec4 coss;
	int t;

//<MaisAbaixo>
	float above = psTetra[0][0].y;
	t = 0;
	for(int i = 1; i < 4; ++i)
	{
		if(psTetra[0][i].y < above)
		{
			above = psTetra[0][i].y;
			t = i;
		}
	}
	idx[0] = t;
	idx[t] = 0;
//</MaisAbaixo>
	
	coss[0] = dot(n, normalize(vec3(psTetra[0][0].xy,0.0) - vec3(psTetra[0][idx[0]].xy,0.0)));
	coss[1] = dot(n, normalize(vec3(psTetra[0][1].xy,0.0) - vec3(psTetra[0][idx[0]].xy,0.0)));
	coss[2] = dot(n, normalize(vec3(psTetra[0][2].xy,0.0) - vec3(psTetra[0][idx[0]].xy,0.0)));
	coss[3] = dot(n, normalize(vec3(psTetra[0][3].xy,0.0) - vec3(psTetra[0][idx[0]].xy,0.0)));

	if(coss[idx[2]] > coss[idx[1]] && coss[idx[2]] > coss[idx[3]])
	{
		t = idx[1];
		idx[1] = idx[2];
		idx[2] = t;
	}else if(coss[idx[3]] > coss[idx[1]] && coss[idx[3]] > coss[idx[2]])
	{
		t = idx[1];
		idx[1] = idx[3];
		idx[3] = t;
	} // else idx[1] ja eh o menor

	if(coss[idx[3]] > coss[idx[2]])
	{
		t = idx[2];
		idx[2] = idx[3];
		idx[3] = t;
	} // else idx[2] ja eh o menor

	return idx;

}

vec3 parametroInterpolacao(in ivec3 idx,in vec2 p)
{
	float areaTotal;
	vec3 area;
	vec3 p0 = vec3(psTetra[0][idx[0]].xy, 0.0);
	vec3 p1 = vec3(psTetra[0][idx[1]].xy, 0.0);
	vec3 p2 = vec3(psTetra[0][idx[2]].xy, 0.0);
	vec3 pp = vec3(p, 0.0);

	areaTotal = length(cross(p1-p0, p2-p0));// /2.0;

	area[0] = length(cross(p2-p1,pp-p1));// /2.0;
	area[1] = length(cross(p0-p2,pp-p2));// /2.0;
	area[2] = length(cross(p1-p0,pp-p0));// /2.0;

	area[0]/=areaTotal;
	area[1]/=areaTotal;
	area[2]/=areaTotal;

	return area;
}

vec2 intersection(in ivec4 idx)
{
	vec2 ret = vec2(0.0);
	vec2 a = ssTetra[idx[0]].xy;
	vec2 b = ssTetra[idx[2]].xy;
	vec2 c = ssTetra[idx[1]].xy;
	vec2 d = ssTetra[idx[3]].xy;

	float der = (b.y - a.y)/(b.x - a.x);
	float ydc = d.y-c.y;
	float xdc = d.x-c.x;

	ret.t = (a.y - c.y + der*(c.x-a.x)) / (ydc-der*xdc);
	ret.s = (c.x + ret.t*xdc - a.x)/ (b.x - a.x);

	return ret;
}


void main()
{
	ivec4 idx;

	Q[0] = Q0[0];
	Q[1] = Q1[0];
	Q[2] = Q2[0];
	Q[3] = Q3[0];

	ssTetra[0] = psTetra[0][0] / psTetra[0][0].w;
	ssTetra[1] = psTetra[0][1] / psTetra[0][1].w;
	ssTetra[2] = psTetra[0][2] / psTetra[0][2].w;
	ssTetra[3] = psTetra[0][3] / psTetra[0][3].w;
		
	idx = sort();
	
	vec2 s,t;	
	vec2 p = intersection(idx);
	s[0] = 1.0 - p.s;
	s[1] = p.s;
	t[0] = 1.0 - p.t;
	t[1] = p.t;

	vec4 center;
	mat4 Qic;
	mat4 Qoc;
	vec4 posc;

	if(s[0] > 0.0 && s[0] < 1.0 && t[0] < 1.0 && t[0] > 0.0)
	{
		float z0 = s[0]*ssTetra[idx[0]].z + s[1]*ssTetra[idx[2]].z;
		float z1 = t[0]*ssTetra[idx[1]].z + t[1]*ssTetra[idx[3]].z;
		if(z0 < z1)
		{
			center = s[0]*psTetra[0][idx[0]] + s[1]*psTetra[0][idx[2]];
			Qic =    s[0]*Q[idx[0]]          + s[1]*Q[idx[2]];
			Qoc =    t[0]*Q[idx[1]]          + t[1]*Q[idx[3]];
			posc =   s[0]*osTetra[0][idx[0]] + s[1]*osTetra[0][idx[2]];
		}else
		{
			center = t[0]*psTetra[0][idx[1]] + t[1]*psTetra[0][idx[3]];
			Qic =    t[0]*Q[idx[1]]          + t[1]*Q[idx[3]];
			Qoc =    s[0]*Q[idx[0]]          + s[1]*Q[idx[2]];
			posc =   t[0]*osTetra[0][idx[1]] + t[1]*osTetra[0][idx[3]];
		}

		gl_FrontColor = vec4(1.0);
		for(int i = 0; i < 4; i++)
		{
			int iplus1 = (i+1)%4;
			gl_Position = center;
			Qi = Qic;
			Qo = Qoc;
			pos = posc;
			EmitVertex();
			gl_Position = psTetra[0][idx[i]];
			Qi = Q[idx[i]];
			Qo = Q[idx[i]];
			pos = osTetra[0][idx[i]];
 			EmitVertex();
			gl_Position = psTetra[0][idx[iplus1]]; 
			Qi = Q[idx[iplus1]];
			Qo = Q[idx[iplus1]];
			pos = osTetra[0][idx[iplus1]];
			EmitVertex();
			EndPrimitive();
		}
	}else
	{
		int t = idx[2];
		idx[2] = idx[3];
		idx[3] = t;
		// idx[0,1,2] formam um triangulo CCW

		vec3 pI = parametroInterpolacao(idx.xyz, psTetra[0][idx[3]].xy);

		float z0 = pI[0]*psTetra[0][idx[0]].z + pI[1]*psTetra[0][idx[1]].z + pI[2]*psTetra[0][idx[2]].z; //z do triangulo
		float z1 = psTetra[0][idx[3]].z;
		if(z0 < z1) // triangulo na frente
		{
			center = pI[0]*psTetra[0][idx[0]] + pI[1]*psTetra[0][idx[1]] + pI[2]*psTetra[0][idx[2]]; 
			Qic = pI[0]*Q[idx[0]] + pI[1]*Q[idx[1]] + pI[2]*Q[idx[2]];
			Qoc = Q[idx[3]];
			posc = pI[0]*osTetra[0][idx[0]] + pI[1]*osTetra[0][idx[1]] + pI[2]*osTetra[0][idx[2]];
		}else // vertice na frente
		{
			center = psTetra[0][idx[3]];
			Qic = Q[idx[3]];
			Qoc = pI[0]*Q[idx[0]] + pI[1]*Q[idx[1]] + pI[2]*Q[idx[2]];
			posc = osTetra[0][idx[3]];
		}


		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = psTetra[0][idx[0]];
		Qi = Q[idx[0]];
		Qo = Q[idx[0]];
		pos = osTetra[0][idx[0]];
 		EmitVertex();
		gl_Position = psTetra[0][idx[1]]; 
		Qi = Q[idx[1]];
		Qo = Q[idx[1]];
		pos = osTetra[0][idx[1]];
		EmitVertex();
		EndPrimitive();

		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = psTetra[0][idx[1]];
		Qi = Q[idx[1]];
		Qo = Q[idx[1]];
		pos = osTetra[0][idx[1]];
 		EmitVertex();
		gl_Position = psTetra[0][idx[2]]; 
		Qi = Q[idx[2]];
		Qo = Q[idx[2]];
		pos = osTetra[0][idx[2]];
		EmitVertex();
		EndPrimitive();

		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = psTetra[0][idx[2]];
		Qi = Q[idx[2]];
		Qo = Q[idx[2]];
		pos = osTetra[0][idx[2]];
 		EmitVertex();
		gl_Position = psTetra[0][idx[0]]; 
		Qi = Q[idx[0]];
		Qo = Q[idx[0]];
		pos = osTetra[0][idx[0]];
		EmitVertex();
		EndPrimitive();
	}
}