#version 150 compatibility
#extension GL_ARB_geometry_shader4 : enable

const mat4 sphere       = mat4(1.0,0.0,0.0,0.0, 0.0, 1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 saddle       = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0,-1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 paraboloide  = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0, 1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 cone         = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.0);
const mat4 cylinder     = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 para_2she    = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.25);

const mat4 tetraIn = mat4(-1.0, -1.0, -1.0, 1.0,
                     1.0,  1.0, -1.0, 1.0,
                     1.0, -1.0,  1.0, 1.0,
                    -1.0,  1.0,  1.0, 1.0);

out mat4 Qi;
noperspective out mat4 Qo;
out vec4 pi;
noperspective out vec4 po;
out vec4 cori;
noperspective out vec4 coro;
noperspective out float woI;

mat4 Q[4];
vec4 cor[4];
vec4 osVertex[4];
vec4 psVertex[4];
vec4 ssVertex[4];
float w[4];

ivec4 sort(void)
{
	vec3 n = vec3(1.0,0.0,0.0);
	ivec4 idx = ivec4(0,1,2,3);
	vec4 coss;
	int t;

//<MaisAbaixo>
	t = 0;
	for(int i = 1; i < 4; ++i)
		if(ssVertex[i].y < ssVertex[t].y)
			t = i;

	idx[0] = t;
	idx[t] = 0;
//</MaisAbaixo>
	
	coss[0] = dot(n, normalize(vec3(ssVertex[0].xy,0.0) - vec3(ssVertex[idx[0]].xy,0.0)));
	coss[1] = dot(n, normalize(vec3(ssVertex[1].xy,0.0) - vec3(ssVertex[idx[0]].xy,0.0)));
	coss[2] = dot(n, normalize(vec3(ssVertex[2].xy,0.0) - vec3(ssVertex[idx[0]].xy,0.0)));
	coss[3] = dot(n, normalize(vec3(ssVertex[3].xy,0.0) - vec3(ssVertex[idx[0]].xy,0.0)));

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
	mat3 A = mat3(vec3(ssVertex[idx[0]].xy, 1.0),  //coluna
				  vec3(ssVertex[idx[1]].xy, 1.0),  //coluna
				  vec3(ssVertex[idx[2]].xy, 1.0)); //coluna
	vec3 P = vec3(p,1.0);

	return inverse(A)*P;
}

vec2 intersection(in ivec4 idx)
{
	vec2 ret = vec2(0.0);
	vec2 a = ssVertex[idx[0]].xy;
	vec2 b = ssVertex[idx[2]].xy;
	vec2 c = ssVertex[idx[1]].xy;
	vec2 d = ssVertex[idx[3]].xy;

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
	vec4 ct;
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
		float z0 = s[0]*ssVertex[idx[0]].z + s[1]*ssVertex[idx[2]].z;
		float z1 = t[0]*ssVertex[idx[1]].z + t[1]*ssVertex[idx[3]].z;
		float wI02 = s[0]/w[idx[0]] + s[1]/w[idx[2]];
		float wI13 = t[0]/w[idx[1]] + t[1]/w[idx[3]];
		if(z0 < z1)
		{
			center = (s[0]*ssVertex[idx[0]] + s[1]*ssVertex[idx[2]])/wI02;
			ct = (t[0]*ssVertex[idx[1]] + t[1]*ssVertex[idx[3]])/wI13;
			
			pic =    (s[0]*osVertex[idx[0]]/w[idx[0]] + s[1]*osVertex[idx[2]]/w[idx[2]])/wI02;
			Qic =    (s[0]*Q[idx[0]]/w[idx[0]]        + s[1]*Q[idx[2]]/w[idx[2]])/wI02;
			coric =  (s[0]*cor[idx[0]]/w[idx[0]]      + s[1]*cor[idx[2]]/w[idx[2]])/wI02;

			poc =    (t[0]*osVertex[idx[1]]/w[idx[1]] + t[1]*osVertex[idx[3]]/w[idx[3]])/wI13;
			Qoc =    (t[0]*Q[idx[1]]/w[idx[1]]        + t[1]*Q[idx[3]]/w[idx[3]])/wI13;
			coroc =  (t[0]*cor[idx[1]]/w[idx[1]]      + t[1]*cor[idx[3]]/w[idx[3]])/wI13;
				
		}else
		{
			center = (t[0]*ssVertex[idx[1]] + t[1]*ssVertex[idx[3]])/wI13;
			ct = (s[0]*ssVertex[idx[0]] + s[1]*ssVertex[idx[2]])/wI02;
			
			pic =    (t[0]*osVertex[idx[1]]/w[idx[1]] + t[1]*osVertex[idx[3]]/w[idx[3]])/wI13;
			Qic =    (t[0]*Q[idx[1]]/w[idx[1]]        + t[1]*Q[idx[3]]/w[idx[3]])/wI13;
			coric =  (t[0]*cor[idx[1]]/w[idx[1]]      + t[1]*cor[idx[3]]/w[idx[3]])/wI13;
			
			poc =    (s[0]*osVertex[idx[0]]/w[idx[0]] + s[1]*osVertex[idx[2]]/w[idx[2]])/wI02;
			Qoc =    (s[0]*Q[idx[0]]/w[idx[0]]        + s[1]*Q[idx[2]]/w[idx[2]])/wI02;
			coroc =  (s[0]*cor[idx[0]]/w[idx[0]]      + s[1]*cor[idx[2]]/w[idx[2]])/wI02;
		}
		nTriangulos = 4;
	}else
	{
		int t = idx[2];
		idx[2] = idx[3];
		idx[3] = t;
		// idx[0,1,2] formam um triangulo CCW

		vec3 pI = parametroInterpolacao2(idx.xyz, ssVertex[idx[3]].xy);
		float wI012 = pI[0]/w[idx[0]] + pI[1]/w[idx[1]] + pI[2]/w[idx[2]];

		float z0 = pI[0]*ssVertex[idx[0]].z + pI[1]*ssVertex[idx[1]].z + pI[2]*ssVertex[idx[2]].z; //z do triangulo
		float z1 = ssVertex[idx[3]].z;
		if(z0 < z1) // triangulo na frente
		{
			center = (pI[0]*ssVertex[idx[0]] + pI[1]*ssVertex[idx[1]] + pI[2]*ssVertex[idx[2]])/wI012;
			ct = psVertex[idx[3]];
			
			pic =    (pI[0]*osVertex[idx[0]]/w[idx[0]] + pI[1]*osVertex[idx[1]]/w[idx[1]] + pI[2]*osVertex[idx[2]]/w[idx[2]])/wI012;
			Qic =    (pI[0]*Q[idx[0]]/w[idx[0]]        + pI[1]*Q[idx[1]]/w[idx[1]]        + pI[2]*Q[idx[2]]/w[idx[2]])/wI012;
			coric =  (pI[0]*cor[idx[0]]/w[idx[0]]      + pI[1]*cor[idx[1]]/w[idx[1]]      + pI[2]*cor[idx[2]]/w[idx[2]])/wI012;

	
			poc =    osVertex[idx[3]];
			Qoc =    Q[idx[3]];
			coroc =  cor[idx[3]];

		}else // vertice na frente
		{
			center = psVertex[idx[3]];
			ct = (pI[0]*ssVertex[idx[0]] + pI[1]*ssVertex[idx[1]] + pI[2]*ssVertex[idx[2]])/wI012;

			pic =    osVertex[idx[3]];
			Qic =    Q[idx[3]];
			coric =  cor[idx[3]];

			poc =    (pI[0]*osVertex[idx[0]]/w[idx[0]] + pI[1]*osVertex[idx[1]]/w[idx[1]] + pI[2]*osVertex[idx[2]]/w[idx[2]])/wI012;
			Qoc =    (pI[0]*Q[idx[0]]/w[idx[0]]        + pI[1]*Q[idx[1]]/w[idx[1]]        + pI[2]*Q[idx[2]]/w[idx[2]])/wI012;
			coroc =  (pI[0]*cor[idx[0]]/w[idx[0]]      + pI[1]*cor[idx[1]]/w[idx[1]]      + pI[2]*cor[idx[2]]/w[idx[2]])/wI012;
		}

		nTriangulos = 3;
	}

	gl_FrontColor = vec4(0.5);
	for(int i = 0; i < nTriangulos; i++)
	{
		int iplus1 = (i+1)%nTriangulos;
		gl_Position = center;
		Qi = Qic;
		pi = pic;
		cori = coric;
		
		Qo = Qoc/ct.w;
		po = poc/ct.w;
		coro = coroc/ct.w;
		woI = 1.0/ct.w;
		EmitVertex();
	
		gl_Position = psVertex[idx[i]];
		Qi = Q[idx[i]];
		pi = osVertex[idx[i]];
		cori = cor[idx[i]];
		
		Qo = Q[idx[i]]/w[idx[i]];		
		po = osVertex[idx[i]]/w[idx[i]];
		coro = cor[idx[i]]/w[idx[i]];
		woI = 1.0/w[idx[i]];
		EmitVertex();
	
		gl_Position = psVertex[idx[iplus1]]; 
		Qi = Q[idx[iplus1]];
		pi = osVertex[idx[iplus1]];
		cori = cor[idx[iplus1]];

		Qo = Q[idx[iplus1]]/w[idx[iplus1]];
		po = osVertex[idx[iplus1]]/w[idx[iplus1]];
		coro = cor[idx[iplus1]]/w[idx[iplus1]];
		woI = 1.0/w[idx[iplus1]];
		EmitVertex();
	
		EndPrimitive();
	}
}

void main()
{
	Q[0] = cone;
	Q[1] = saddle;
	Q[2] = sphere;
	Q[3] = cone;

	cor[0] = vec4(0.0, 1.0, 1.0, 1.0);
	cor[1] = vec4(1.0, 0.0, 0.0, 1.0);
	cor[2] = vec4(0.0, 1.0, 0.0, 1.0);
	cor[3] = vec4(0.0, 0.0, 1.0, 1.0);

	osVertex[0] = tetraIn[0];
	osVertex[1] = tetraIn[1];
	osVertex[2] = tetraIn[2];
	osVertex[3] = tetraIn[3];

	psVertex[0] = gl_ModelViewProjectionMatrix * osVertex[0];
	psVertex[1] = gl_ModelViewProjectionMatrix * osVertex[1];
	psVertex[2] = gl_ModelViewProjectionMatrix * osVertex[2];
	psVertex[3] = gl_ModelViewProjectionMatrix * osVertex[3];

	w[0] = psVertex[0].w;
	w[1] = psVertex[1].w;
	w[2] = psVertex[2].w;
	w[3] = psVertex[3].w;

	ssVertex[0] = psVertex[0]/w[0];
	ssVertex[1] = psVertex[1]/w[1];
	ssVertex[2] = psVertex[2]/w[2];
	ssVertex[3] = psVertex[3]/w[3];
		
	processaTetraedro();
}