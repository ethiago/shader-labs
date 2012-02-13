#version 150
#extension GL_ARB_geometry_shader4 : enable

in mat4 osTetra[];
in mat4 psTetra[];
in mat4 cor[];
in vec4 d[];

out float Di;
out float Do;
out vec4 pi;
out vec4 po;
out vec4 cori;
out vec4 coro;

const float iso = 0.2;

ivec4 sort(void)
{
	vec3 n = vec3(1.0,0.0,0.0);
	ivec4 idx = ivec4(0,1,2,3);
	vec4 coss;
	int t;

//<MaisAbaixo>
	t = 0;
	for(int i = 1; i < 4; ++i)
		if(psTetra[0][i].y < psTetra[0][t].y)
			t = i;

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

vec3 parametroInterpolacao2(in ivec3 idx,in vec2 p)
{
	mat3 A = mat3(vec3(psTetra[0][idx[0]].xy, 1.0),  //coluna
				  vec3(psTetra[0][idx[1]].xy, 1.0),  //coluna
				  vec3(psTetra[0][idx[2]].xy, 1.0)); //coluna
	vec3 P = vec3(p,1.0);

	return inverse(A)*P;
}
/*  Quero verificar se o algorítimo de baixo e mais rapido que o de cima
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
*/
vec2 intersection(in ivec4 idx)
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

bool contemIso()
{
	if((d[0][0] > iso && d[0][1] > iso && d[0][2] > iso && d[0][3] > iso) ||
       (d[0][0] < iso && d[0][1] < iso && d[0][2] < iso && d[0][3] < iso))
		return false;
	else
		return true;
}

void main()
{
	ivec4 idx;
	vec2 s,t;	
	vec4 center;
	float Dic;
	float Doc;
	vec4 pic;
	vec4 poc;
	vec4 coric;
	vec4 coroc;
	int nTriangulos;

	//if(!contemIso())
	//	discard;
		
	idx = sort();
	
	vec2 p = intersection(idx);
	s[0] = 1.0 - p.s;
	s[1] = p.s;
	t[0] = 1.0 - p.t;
	t[1] = p.t;

	if(s[0] > 0.0 && s[0] < 1.0 && t[0] < 1.0 && t[0] > 0.0)
	{
		float z0 = s[0]*psTetra[0][idx[0]].z + s[1]*psTetra[0][idx[2]].z;
		float z1 = t[0]*psTetra[0][idx[1]].z + t[1]*psTetra[0][idx[3]].z;
		if(z0 < z1)
		{
			center = s[0]*psTetra[0][idx[0]] + s[1]*psTetra[0][idx[2]];
			
			pic =    s[0]*osTetra[0][idx[0]] + s[1]*osTetra[0][idx[2]];
			Dic =    s[0]*d[0][idx[0]]       + s[1]*d[0][idx[2]];
			coric =  s[0]*cor[0][idx[0]]     + s[1]*cor[0][idx[2]];

			poc =    t[0]*osTetra[0][idx[1]] + t[1]*osTetra[0][idx[3]];
			Doc =    t[0]*d[0][idx[1]]       + t[1]*d[0][idx[3]];
			coroc =  t[0]*cor[0][idx[1]]     + t[1]*cor[0][idx[3]];
			
		}else
		{
			center = t[0]*psTetra[0][idx[1]] + t[1]*psTetra[0][idx[3]];
			
			pic =    t[0]*osTetra[0][idx[1]] + t[1]*osTetra[0][idx[3]];
			Dic =    t[0]*d[0][idx[1]]       + t[1]*d[0][idx[3]];
			coric =  t[0]*cor[0][idx[1]]     + t[1]*cor[0][idx[3]];
			
			poc =    s[0]*osTetra[0][idx[0]] + s[1]*osTetra[0][idx[2]];
			Doc =    s[0]*d[0][idx[0]]       + s[1]*d[0][idx[2]];
			coroc =  s[0]*cor[0][idx[0]]     + t[1]*cor[0][idx[2]];
		}
		nTriangulos = 4;
	}else
	{
		int t = idx[2];
		idx[2] = idx[3];
		idx[3] = t;
		// idx[0,1,2] formam um triangulo CCW

		vec3 pI = parametroInterpolacao2(idx.xyz, psTetra[0][idx[3]].xy);

		float z0 = pI[0]*psTetra[0][idx[0]].z + pI[1]*psTetra[0][idx[1]].z + pI[2]*psTetra[0][idx[2]].z; //z do triangulo
		float z1 = psTetra[0][idx[3]].z;
		if(z0 < z1) // triangulo na frente
		{
			center = pI[0]*psTetra[0][idx[0]] + pI[1]*psTetra[0][idx[1]] + pI[2]*psTetra[0][idx[2]]; 
			
			pic =    pI[0]*osTetra[0][idx[0]] + pI[1]*osTetra[0][idx[1]] + pI[2]*osTetra[0][idx[2]];
			Dic =    pI[0]*d[0][idx[0]]       + pI[1]*d[0][idx[1]]       + pI[2]*d[0][idx[2]];
			coric =  pI[0]*cor[0][idx[0]]     + pI[1]*cor[0][idx[1]]     + pI[2]*cor[0][idx[2]];
	
			poc =    osTetra[0][idx[3]];
			Doc =    d[0][idx[3]];
			coroc =  cor[0][idx[3]];
		}else // vertice na frente
		{
			center = psTetra[0][idx[3]];

			pic =    osTetra[0][idx[3]];
			Dic =    d[0][idx[3]];
			coric =  cor[0][idx[3]];

			poc =    pI[0]*osTetra[0][idx[0]] + pI[1]*osTetra[0][idx[1]] + pI[2]*osTetra[0][idx[2]];
			Doc =    pI[0]*d[0][idx[0]]       + pI[1]*d[0][idx[1]]       + pI[2]*d[0][idx[2]];
			coroc =  pI[0]*cor[0][idx[0]]     + pI[1]*cor[0][idx[1]]     + pI[2]*cor[0][idx[2]];
		}

		nTriangulos = 3;
	}

	gl_FrontColor = vec4(1.0);
	for(int i = 0; i < nTriangulos; i++)
	{
		int iplus1 = (i+1)%nTriangulos;
		gl_Position = center;
		Di = Dic;
		Do = Doc;
		pi = pic;
		po = poc;
		cori = coric;
		coro = coroc;
		EmitVertex();
	
		gl_Position = psTetra[0][idx[i]];
		Di = d[0][idx[i]];
		Do = d[0][idx[i]];
		pi = osTetra[0][idx[i]];
		po = osTetra[0][idx[i]];
		cori = cor[0][idx[i]];
		coro = cor[0][idx[i]];
		EmitVertex();
	
		gl_Position = psTetra[0][idx[iplus1]]; 
		Di = d[0][idx[iplus1]];
		Do = d[0][idx[iplus1]];
		pi = osTetra[0][idx[iplus1]];
		po = osTetra[0][idx[iplus1]];
		cori = cor[0][idx[iplus1]];
		coro = cor[0][idx[iplus1]];
		EmitVertex();
	
		EndPrimitive();
	}
}