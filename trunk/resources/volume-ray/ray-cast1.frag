#version 150 compatibility

vec4 zero = gl_ModelViewMatrixInverse * vec4(0.0,0.0,0.0,1.0);
in vec4 pi;

const mat4 Q = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,0.0);

const float shininess = 8.0;
const float specContrib = 0.25;
const float diffContrib = 0.45;
const float ambContrib = 0.3;
const float err = 0.0001;

bool entre_1e1(in float a)
{
	return (-1.0 <= a && a <= 1.0);
}

float tetra(in vec4 p)
{
	vec4 w0 = vec4(-1.0, -1.0, -1.0, 1.0);
	vec4 w1 = vec4( 1.0,  1.0, -1.0, 1.0);
	vec4 w2 = vec4( 1.0, -1.0,  1.0, 1.0);
	vec4 w3 = vec4(-1.0,  1.0,  1.0, 1.0);


	mat4 WW = mat4(w0, w1, w2, w3);
	mat4 WI = inverse(WW);

	mat4 sphere       = mat4(1.0,0.0,0.0,0.0, 0.0, 1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
	mat4 saddle       = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0,-1.0,0.0, 0.0,-0.5,0.0, 0.0);
	mat4 paraboloide  = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0, 1.0,0.0, 0.0,-0.5,0.0, 0.0);
	mat4 cone         = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.0);
	mat4 cylinder     = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
	mat4 para_2she    = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.25);

	mat4 Q0 = cone;
	mat4 Q1 = saddle;
	mat4 Q2 = sphere;
	mat4 Q3 = cone;

	vec4 r = (WI*p);
	return dot( vec4( dot(p,Q0*p) ,  dot(p,Q1*p), dot(p,Q2*p) , dot(p,Q3*p)) , r );
}

float distCubo(in vec4 p0, in vec4 d)
{
	float t;
	vec4 p;
	//x = -1
	t = (-1.0 - p0.x)/d.x;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.y) && entre_1e1(p.z) )
		{
			return t;
		}
	}
	//x = 1
	t = (1.0 - p0.x)/d.x;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.y) && entre_1e1(p.z) )
		{
			return t;
		}
	}
	//y = -1
	t = (-1.0 - p0.y)/d.y;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.x) && entre_1e1(p.z) )
		{
			return t;
		}
	}
	//y = 1
	t = (1.0 - p0.y)/d.y;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.x) && entre_1e1(p.z) )
		{
			return t;
		}
	}
	//z = -1
	t = (-1.0 - p0.z)/d.z;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.x) && entre_1e1(p.y) )
		{
			return t;
		}
	}
	//z = 1
	t = (1.0 - p0.z)/d.z;
	if(t - err > 1.0)
	{
		p = p0 + t*d;
		if( entre_1e1(p.x) && entre_1e1(p.y) )
		{
			return t;
		}
	}

	return -1.0;	
}

vec3 distQuad(in vec4 p0, in vec4 d)
{
	p0.w = 1.0;
	d.w = 0.0;
	vec3 ret = vec3(-1.0, -1.0, 1.0);
	float a = dot(d, Q * d);
	float b = dot(p0, Q * d);
	float c = dot(p0, Q * p0);

	float delta = b*b - a*c;

	if(delta < 0.0)
	{
		ret[2] = -1.0;
	}else
	{
		delta = sqrt(delta);
		ret[0] = (-b - delta)/a;
		ret[1] = (-b + delta)/a;
		if(ret[1] < ret[0])
		{
			float tmp = ret[0];
			ret[0] = ret[1];
			ret[1] = tmp;
		}
	}

	return ret;
}

float intensity(in vec3 inter, in vec3 v)
{
	v = normalize(-v);
	vec3 norm = (Q * vec4(inter, 1.0)).xyz;
	norm = normalize(norm);
	vec3 lv = (gl_ModelViewMatrixInverse * gl_LightSource[0].position).xyz;
	lv = normalize(lv - inter);

	float diff = max(dot(norm, lv), 0.0);
	lv = normalize(reflect(-lv, norm));
	float spec = pow(max(dot(v,lv),0.0), shininess);

	return diff*diffContrib +  ambContrib + spec*specContrib;
}

vec4 interp(in float iso, in float d1, in float d2, in vec4 p1, in vec4 p2)
{
	float t = (iso - d1)/(d2-d1);
	return (1.0 - t)*p1 + t*p2;
}

void main ()
{
	vec4 v = pi-zero;
	float iso = 0.0;
	float tc = distCubo(zero, v);
	if(tc < 0.0)
		discard;
	vec4 po = zero + tc*v;
	vec4 d = po-pi;

	int nPassos = 100;
	float dr = 1.0/(nPassos);
	
	vec4 ant = pi;
	float dAnt = tetra(ant);
	int i;
	for(i = 1 ; i < nPassos; ++i)
	{
		float t = i*dr;
		vec4 p = pi + t*d;
		float dP = tetra(p);
		if((dP > iso && dAnt < iso) || (dP < iso && dAnt > iso))
		{
			p = interp(iso, dP, dAnt, p, ant);
			float inten = intensity(p.xyz, v.xyz);
			gl_FragColor = vec4(gl_Color.xyz * inten, 1.0);
			break;
		}
		ant = p;
		dAnt = dP;
	}
	if(i == nPassos)
		discard;
	
	gl_FragColor = gl_Color;
}