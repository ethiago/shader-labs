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

vec2 inter(in ivec4 idx)
{
	vec2 ret = vec2(0.0);
	vec2 a = psTetra[0][idx[0]].xy;
	vec2 b = psTetra[0][idx[2]].xy;
	vec2 c = psTetra[0][idx[1]].xy;
	vec2 d = psTetra[0][idx[3]].xy;

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
	vec2 p;

	Q[0] = Q0[0];
	Q[1] = Q1[0];
	Q[2] = Q2[0];
	Q[3] = Q3[0];
		
	idx = sort();

	gl_FrontColor = vec4(1.0);
	
	p = inter(idx);

	vec4 center;
	mat4 Qic;
	mat4 Qoc;
	vec4 posc;

	if(p.s > 0.0 && p.s < 1.0 && p.t < 1.0 && p.t > 0.0)
	{
		

		float z0 = psTetra[0][idx.x].z + p.s*(psTetra[0][idx.z].z - psTetra[0][idx.x].z);
		float z1 = psTetra[0][idx.y].z + p.t*(psTetra[0][idx.w].z - psTetra[0][idx.y].z);
		if(z0 < z1)
		{
			center = psTetra[0][idx.x] + p.s*(psTetra[0][idx.z] - psTetra[0][idx.x]);
			Qic = Q[idx.x] + p.s*(Q[idx.z] - Q[idx.x]);
			Qoc = Q[idx.y] + p.t*(Q[idx.w] - Q[idx.y]);
			posc = osTetra[0][idx.x] + p.s*(osTetra[0][idx.z] - osTetra[0][idx.x]);
		}else
		{
			center = psTetra[0][idx.y] + p.t*(psTetra[0][idx.w] - psTetra[0][idx.y]);
			Qic = Q[idx.y] + p.t*(Q[idx.w] - Q[idx.y]);
			Qoc = Q[idx.x] + p.s*(Q[idx.z] - Q[idx.x]);
			posc = osTetra[0][idx.y] + p.t*(osTetra[0][idx.w] - osTetra[0][idx.y]);
		}

//lineStrip
/*
		gl_FrontColor = vec4(1.0,0.0,0.0,1.0);
		gl_Position = center; EmitVertex();
		gl_Position = psTetra[0][idx.x]; EmitVertex();
		EndPrimitive();
		gl_Position = center; EmitVertex();
		gl_Position = psTetra[0][idx.y]; EmitVertex();
		EndPrimitive();
		gl_Position = center; EmitVertex();
		gl_Position = psTetra[0][idx.z]; EmitVertex();
		EndPrimitive();
		gl_Position = center; EmitVertex();
		gl_Position = psTetra[0][idx.w]; EmitVertex();
		EndPrimitive();

		gl_FrontColor = vec4(1.0);
		gl_Position = psTetra[0][idx.x]; EmitVertex();
		gl_Position = psTetra[0][idx.y]; EmitVertex();
		EndPrimitive();
		gl_Position = psTetra[0][idx.x]; EmitVertex();
		gl_Position = psTetra[0][idx.z]; EmitVertex();
		EndPrimitive();
		gl_Position = psTetra[0][idx.x]; EmitVertex();
		gl_Position = psTetra[0][idx.w]; EmitVertex();
		EndPrimitive();
		gl_Position = psTetra[0][idx.y]; EmitVertex();
		gl_Position = psTetra[0][idx.z]; EmitVertex();
		EndPrimitive();
		gl_Position = psTetra[0][idx.y]; EmitVertex();
		gl_Position = psTetra[0][idx.w]; EmitVertex();
		EndPrimitive();
		gl_Position = psTetra[0][idx.z]; EmitVertex();
		gl_Position = psTetra[0][idx.w]; EmitVertex();
		EndPrimitive();
*/

//Final Triangle Strip

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
		gl_Position = psTetra[0][idx[3]]; 
		Qi = Q[idx[3]];
		Qo = Q[idx[3]];
		pos = osTetra[0][idx[3]];
		EmitVertex();
		EndPrimitive();		


		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = psTetra[0][idx[3]];
		Qi = Q[idx[3]];
		Qo = Q[idx[3]];
		pos = osTetra[0][idx[3]];
 		EmitVertex();
		gl_Position = psTetra[0][idx[0]]; 
		Qi = Q[idx[0]];
		Qo = Q[idx[0]];
		pos = osTetra[0][idx[0]];
		EmitVertex();
		EndPrimitive();
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