#version 150
#extension GL_ARB_geometry_shader4 : enable

const mat4 sphere       = mat4(1.0,0.0,0.0,0.0, 0.0, 1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 saddle       = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0,-1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 paraboloide  = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0, 1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 cone         = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.0);
const mat4 cylinder     = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 para_2she    = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.25);

in mat4 Q0[];
in mat4 Q1[];
in mat4 Q2[];
in mat4 Q3[];
in mat4 osTetraIn[];
in mat4 psTetraIn[];
in mat4 corIn[];

in mat4 mvp[];

out mat4 Qi;
out mat4 Qo;
out vec4 pi;
out vec4 po;
out vec4 cori;
out vec4 coro;

mat4 Q[4];
vec4 cor[4];
vec4 osVertex[4];
vec4 psVertex[4];

ivec4 sort(void)
{
	vec3 n = vec3(1.0,0.0,0.0);
	ivec4 idx = ivec4(0,1,2,3);
	vec4 coss;
	int t;

//<MaisAbaixo>
	t = 0;
	for(int i = 1; i < 4; ++i)
		if(psVertex[i].y < psVertex[t].y)
			t = i;

	idx[0] = t;
	idx[t] = 0;
//</MaisAbaixo>
	
	coss[0] = dot(n, normalize(vec3(psVertex[0].xy,0.0) - vec3(psVertex[idx[0]].xy,0.0)));
	coss[1] = dot(n, normalize(vec3(psVertex[1].xy,0.0) - vec3(psVertex[idx[0]].xy,0.0)));
	coss[2] = dot(n, normalize(vec3(psVertex[2].xy,0.0) - vec3(psVertex[idx[0]].xy,0.0)));
	coss[3] = dot(n, normalize(vec3(psVertex[3].xy,0.0) - vec3(psVertex[idx[0]].xy,0.0)));

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
	mat3 A = mat3(vec3(psVertex[idx[0]].xy, 1.0),  //coluna
				  vec3(psVertex[idx[1]].xy, 1.0),  //coluna
				  vec3(psVertex[idx[2]].xy, 1.0)); //coluna
	vec3 P = vec3(p,1.0);

	return inverse(A)*P;
}

vec2 intersection(in ivec4 idx)
{
	vec2 ret = vec2(0.0);
	vec2 a = psVertex[idx[0]].xy;
	vec2 b = psVertex[idx[2]].xy;
	vec2 c = psVertex[idx[1]].xy;
	vec2 d = psVertex[idx[3]].xy;

	float der = (b.y - a.y)/(b.x - a.x);
	float ydc = d.y-c.y;
	float xdc = d.x-c.x;

	ret.t = (a.y - c.y + der*(c.x-a.x)) / (ydc-der*xdc);
	ret.s = (c.x + ret.t*xdc - a.x)/ (b.x - a.x);

	return ret;
}

void processaTetraedro()
{
	ivec4 idx;
	vec2 s,t;	
	vec4 center;
	mat4 Qic;
	mat4 Qoc;
	vec4 pic;
	vec4 poc;
	vec4 coric;
	vec4 coroc;
	int nTriangulos;

	idx = sort();
	
	vec2 p = intersection(idx);
	s[0] = 1.0 - p.s;
	s[1] = p.s;
	t[0] = 1.0 - p.t;
	t[1] = p.t;

	if(s[0] > 0.0 && s[0] < 1.0 && t[0] < 1.0 && t[0] > 0.0)
	{
		float z0 = s[0]*psVertex[idx[0]].z + s[1]*psVertex[idx[2]].z;
		float z1 = t[0]*psVertex[idx[1]].z + t[1]*psVertex[idx[3]].z;
		if(z0 < z1)
		{
			center = s[0]*psVertex[idx[0]] + s[1]*psVertex[idx[2]];
			
			pic =    s[0]*osVertex[idx[0]] + s[1]*osVertex[idx[2]];
			Qic =    s[0]*Q[idx[0]]          + s[1]*Q[idx[2]];
			coric =  s[0]*cor[idx[0]]     + s[1]*cor[idx[2]];

			poc =    t[0]*osVertex[idx[1]] + t[1]*osVertex[idx[3]];
			Qoc =    t[0]*Q[idx[1]]          + t[1]*Q[idx[3]];
			coroc =  t[0]*cor[idx[1]]     + t[1]*cor[idx[3]];
			
		}else
		{
			center = t[0]*psVertex[idx[1]] + t[1]*psVertex[idx[3]];
			
			pic =    t[0]*osVertex[idx[1]] + t[1]*osVertex[idx[3]];
			Qic =    t[0]*Q[idx[1]]          + t[1]*Q[idx[3]];
			coric =  t[0]*cor[idx[1]]     + t[1]*cor[idx[3]];
			
			poc =    s[0]*osVertex[idx[0]] + s[1]*osVertex[idx[2]];
			Qoc =    s[0]*Q[idx[0]]          + s[1]*Q[idx[2]];
			coroc =  s[0]*cor[idx[0]]     + s[1]*cor[idx[2]];
		}
		nTriangulos = 4;
	}else
	{
		int t = idx[2];
		idx[2] = idx[3];
		idx[3] = t;
		// idx[0,1,2] formam um triangulo CCW

		vec3 pI = parametroInterpolacao2(idx.xyz, psVertex[idx[3]].xy);

		float z0 = pI[0]*psVertex[idx[0]].z + pI[1]*psVertex[idx[1]].z + pI[2]*psVertex[idx[2]].z; //z do triangulo
		float z1 = psVertex[idx[3]].z;
		if(z0 < z1) // triangulo na frente
		{
			center = pI[0]*psVertex[idx[0]] + pI[1]*psVertex[idx[1]] + pI[2]*psVertex[idx[2]]; 
			
			pic =    pI[0]*osVertex[idx[0]] + pI[1]*osVertex[idx[1]] + pI[2]*osVertex[idx[2]];
			Qic =    pI[0]*Q[idx[0]]          + pI[1]*Q[idx[1]]          + pI[2]*Q[idx[2]];
			coric =  pI[0]*cor[idx[0]]     + pI[1]*cor[idx[1]]     + pI[2]*cor[idx[2]];
	
			poc =    osVertex[idx[3]];
			Qoc =    Q[idx[3]];
			coroc =  cor[idx[3]];
		}else // vertice na frente
		{
			center = psVertex[idx[3]];

			pic =    osVertex[idx[3]];
			Qic =    Q[idx[3]];
			coric =  cor[idx[3]];

			poc =    pI[0]*osVertex[idx[0]] + pI[1]*osVertex[idx[1]] + pI[2]*osVertex[idx[2]];
			Qoc =    pI[0]*Q[idx[0]]          + pI[1]*Q[idx[1]]          + pI[2]*Q[idx[2]];
			coroc =  pI[0]*cor[idx[0]]     + pI[1]*cor[idx[1]]     + pI[2]*cor[idx[2]];
		}

		nTriangulos = 3;
	}

	gl_FrontColor = vec4(1.0);
	for(int i = 0; i < nTriangulos; i++)
	{
		int iplus1 = (i+1)%nTriangulos;
		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pi = pic;
		po = poc;
		cori = coric;
		coro = coroc;
		EmitVertex();
	
		gl_Position = psVertex[idx[i]];
		Qi = Q[idx[i]];
		Qo = Q[idx[i]];
		pi = osVertex[idx[i]];
		po = osVertex[idx[i]];
		cori = cor[idx[i]];
		coro = cor[idx[i]];
		EmitVertex();
	
		gl_Position = psVertex[idx[iplus1]]; 
		Qi = Q[idx[iplus1]];
		Qo = Q[idx[iplus1]];
		pi = osVertex[idx[iplus1]];
		po = osVertex[idx[iplus1]];
		cori = cor[idx[iplus1]];
		coro = cor[idx[iplus1]];
		EmitVertex();
	
		EndPrimitive();
	}
}

void geraLinhasTetraedro()
{
	gl_FrontColor = vec4(1.0);
	for(int i = 0; i < 4; i++)
	{
		
		for(int j = i+1; j < 4; j++ )
		{
			gl_Position = psVertex[i];
			EmitVertex();
		
			gl_Position = psVertex[j];
			EmitVertex();
		
			EndPrimitive();
		}
	}
}

void main()
{
	Q[0] = Q0[0];
	Q[1] = Q1[0];
	Q[2] = Q2[0];
	Q[3] = Q3[0];

	cor[0] = corIn[0][0];
	cor[1] = corIn[0][1];
	cor[2] = corIn[0][2];
	cor[3] = corIn[0][3];

	osVertex[0] = osTetraIn[0][0];
	osVertex[1] = osTetraIn[0][1];
	osVertex[2] = osTetraIn[0][2];
	osVertex[3] = osTetraIn[0][3];

	psVertex[0] = psTetraIn[0][0];
	psVertex[1] = psTetraIn[0][1];
	psVertex[2] = psTetraIn[0][2];
	psVertex[3] = psTetraIn[0][3];
		
	//processaTetraedro();
	//geraLinhasTetraedro();

	cor[3] = vec4(1.0);
	osVertex[3] = vec4(2.0, -2.0, -2.0, 1.0);
	psVertex[3] = mvp[0] * osVertex[3];
	Q[3] = cylinder;

	//processaTetraedro();
	//geraLinhasTetraedro();

	cor[3] = corIn[0][3];
	osVertex[3] = osTetraIn[0][3];
	psVertex[3] = psTetraIn[0][3];
	Q[3] = Q3[0];

	cor[0] = vec4(1.0);
	osVertex[0] = vec4(2.0, 2.0, 2.0, 1.0);
	psVertex[0] = mvp[0] * osVertex[0];
	Q[0] = cylinder;

	//geraLinhasTetraedro();
	//processaTetraedro();

	cor[0] = corIn[0][0];
	osVertex[0] = osTetraIn[0][0];
	psVertex[0] = psTetraIn[0][0];
	Q[0] = Q0[0];

	cor[2] = vec4(1.0);
	osVertex[2] = vec4(-2.0, 2.0, -2.0, 1.0);
	psVertex[2] = mvp[0] * osVertex[2];
	Q[2] = cylinder;

	//geraLinhasTetraedro();
	//processaTetraedro();

	cor[2] = corIn[0][2];
	osVertex[2] = osTetraIn[0][2];
	psVertex[2] = psTetraIn[0][2];
	Q[2] = Q2[0];

	cor[1] = vec4(1.0);
	osVertex[1] = vec4(-2.0, -2.0, 2.0, 1.0);
	psVertex[1] = mvp[0] * osVertex[1];
	Q[1] = cylinder;

	//geraLinhasTetraedro();
	processaTetraedro();
}