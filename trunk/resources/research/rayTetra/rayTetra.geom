#extension GL_ARB_geometry_shader4 : enable

varying out mat4 Qi;
varying out mat4 Qo;

varying out vec4 pos;

const mat4 Q = mat4(1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,-0.5);

mat4 QIn[4];

mat4 tetraIn = mat4(-1.0, -1.0, -1.0, 1.0,
                   1.0,  1.0, -1.0, 1.0,
                   1.0, -1.0,  1.0, 1.0,
                  -1.0,  1.0,  1.0, 1.0);

mat4 tetra;


bool equalThanZero(float v)
{
	if(abs(v) < 0.000001)
		return true;

	return false;
}


void calcula(inout ivec4 idx)
{
	vec3 n = vec3(1.0,0.0,0.0);
	int i;
	float co = 2.0;
	for(i = 0; i < 4; ++i)
	{
		if(i != idx.x)
		{
			vec3 v = normalize(vec3(tetra[i].xy,0.0) - vec3(tetra[idx.x].xy,0.0));
			float c = dot(v,n);
			if(c < co)
			{
				co = c;
				idx.y = i;
			}
		}
	}

	co = -2.0;
	for(i = 0; i < 4; ++i)
	{
		if(i != idx.x && i != idx.y)
		{
			vec3 v = normalize(vec3(tetra[i].xy,0.0) - vec3(tetra[idx.x].xy,0.0));
			float c = dot(v,n);
			if(c > co)
			{
				co = c;
				idx.w = i;
			}
		}
	}

	for(i = 0; i < 4; ++i)
	{
		if(i != idx.x && i != idx.y && i != idx.w)
		{
			idx.z = i;
		}
	}
}

vec2 inter(in ivec4 idx)
{
	vec2 ret = vec2(0.0);
	vec2 a = tetra[idx.x].xy;
	vec2 b = tetra[idx.z].xy;
	vec2 c = tetra[idx.y].xy;
	vec2 d = tetra[idx.w].xy;

	float der = (b.y - a.y)/(b.x - a.x);
	float ydc = d.y-c.y;
	float xdc = d.x-c.x;

	ret.s = (a.y - c.y + der*(c.x-a.x)) / (ydc-der*xdc);
	ret.t = (c.x + ret.s*xdc - a.x)/ (b.x - a.x);

	return ret;
}

void main()
{
	gl_FrontColor = vec4(1.0);
	int ie = 0;
	ivec4 idx = ivec4(0);

	tetra[0] = gl_ModelViewProjectionMatrix * tetraIn[0];
	tetra[1] = gl_ModelViewProjectionMatrix * tetraIn[1];
	tetra[2] = gl_ModelViewProjectionMatrix * tetraIn[2];
	tetra[3] = gl_ModelViewProjectionMatrix * tetraIn[3];

	QIn[0] = Q;
	QIn[1] = Q;
	QIn[2] = Q;
	QIn[3] = Q;

	vec4 ve = tetra[0];
	ie = 0;
	int i = 0;
	for(i = 1; i < 4; ++i)
	{
		if(tetra[i].y < ve.y)
		{
			ve = tetra[i];
			ie = i;
		}
	}

	idx.x = ie;

	calcula(idx);
	
	gl_FrontColor = vec4(1.0);
	
	vec2 p = inter(idx);

	if(p.s > 0.0 && p.s < 1.0 && p.t < 1.0 && p.t > 0.0)
	{
		vec4 center;
		mat4 Qic;
		mat4 Qoc;
		vec4 posc;

		float z0 = tetra[idx.x].z + p.t*(tetra[idx.z].z - tetra[idx.x].z);
		float z1 = tetra[idx.y].z + p.s*(tetra[idx.w].z - tetra[idx.y].z);
		if(z0 < z1)
		{
			center = tetra[idx.x] + p.t*(tetra[idx.z] - tetra[idx.x]);
			Qic = QIn[idx.x] + p.t*(QIn[idx.z] - QIn[idx.x]);
			Qoc = QIn[idx.y] + p.s*(QIn[idx.w] - QIn[idx.y]);
			posc = tetraIn[idx.x] + p.t*(tetraIn[idx.z] - tetraIn[idx.x]);
		}else
		{
			center = tetra[idx.y] + p.s*(tetra[idx.w] - tetra[idx.y]);
			Qic = QIn[idx.y] + p.s*(QIn[idx.w] - QIn[idx.y]);
			Qoc = QIn[idx.x] + p.t*(QIn[idx.z] - QIn[idx.x]);
			posc = tetraIn[idx.y] + p.s*(tetraIn[idx.w] - tetraIn[idx.y]);
		}

		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = tetra[idx.x];
		Qi = QIn[idx.x];
		Qo = QIn[idx.x];
		pos = tetraIn[idx.x];
 		EmitVertex();
		gl_Position = tetra[idx.y]; 
		Qi = QIn[idx.y];
		Qo = QIn[idx.y];
		pos = tetraIn[idx.y];
		EmitVertex();
		EndPrimitive();


		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = tetra[idx.y];
		Qi = QIn[idx.y];
		Qo = QIn[idx.y];
		pos = tetraIn[idx.y];
 		EmitVertex();
		gl_Position = tetra[idx.z]; 
		Qi = QIn[idx.z];
		Qo = QIn[idx.z];
		pos = tetraIn[idx.z];
		EmitVertex();
		EndPrimitive();


		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = tetra[idx.z];
		Qi = QIn[idx.z];
		Qo = QIn[idx.z];
		pos = tetraIn[idx.z];
 		EmitVertex();
		gl_Position = tetra[idx.w]; 
		Qi = QIn[idx.w];
		Qo = QIn[idx.w];
		pos = tetraIn[idx.w];
		EmitVertex();
		EndPrimitive();		


		gl_Position = center;
		Qi = Qic;
		Qo = Qoc;
		pos = posc;
		EmitVertex();
		gl_Position = tetra[idx.w];
		Qi = QIn[idx.w];
		Qo = QIn[idx.w];
		pos = tetraIn[idx.w];
 		EmitVertex();
		gl_Position = tetra[idx.x]; 
		Qi = QIn[idx.x];
		Qo = QIn[idx.x];
		pos = tetraIn[idx.x];
		EmitVertex();
		EndPrimitive();
	}
}