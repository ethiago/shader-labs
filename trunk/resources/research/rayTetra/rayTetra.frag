#version 120
#extension GL_ARB_gpu_shader_fp64 : enable

const mat4 tQ = mat4(1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,-0.4);

varying mat4 Qi;
varying mat4 Qo;

varying vec4 pi;
varying vec4 po;

const float ac = 0.3;
const float dc = 0.4;
const float sc = 0.3;
const float err = 0.0000001;

vec3 lp = vec3(gl_ModelViewMatrixInverse * gl_LightSource[0].position);
vec4 O = gl_ModelViewMatrixInverse * vec4(0.0,0.0,0.0,1.0);

float a,b,c,d;

vec4 cor;

float f(float t)
{
	return a*(t*t*t) + b*(t*t) + c*t + d;
}

float df(float t)
{
	return 3*a*(t*t) + 2*b*(t) + c;
}

bool acharRaiz(float a, float b, float c, out vec2 raizes)
{
	raizes = vec2(1.0);
	float delta = b*b - 4.0*a*c;
	
	if(delta < 0.0)
		return false;

	delta = sqrt(delta);
	raizes[0] = (-b - delta)/(2.0*a);
	raizes[1] = (-b + delta)/(2.0*a);
	return true;
}

float shade(in vec3 p, in vec3 n, in vec3 v)
{
	vec3 lv = normalize(lp - p);
	float dif = max(dot(lv, n), 0.0);
	
	float spec = max(dot(reflect(-lv, n), v), 0.0);
	spec = pow(spec, 16.0);

	return dif*dc + spec*sc + ac;
}

float acharNewton(float t0)
{
	float t = t0;
	do{
		t0 = t;
		t = t0 - f(t0)/df(t0);
	}while(abs(t-t0) > err);
	return t;
}

vec2 acharMaxMinLocal()
{
	vec2 r;
	if(!acharRaiz(3.0*a, 2.0*b, c, r))
		discard;

	return r;
}

bool valido(float t)
{
	return (t >= 0.0 && t <= 1.0);
}

float buscaLinear(float x0, float fx0, float x1, float fx1)
{
	return x0 + ((abs(fx0)*(x1-x0))/abs(fx0-fx1));
}

float acharTCubicaBuscaBinaria()
{
	vec2 mm = acharMaxMinLocal();
	vec2 fx  = vec2(f(0.0), f(1.0));
	vec2 fm  = vec2(f(mm[0]), f(mm[1]));

	if(valido(mm[0]))
	{
		if(sign(fx[0]) * sign(fm[0]) <= 0)
			return acharNewton(buscaLinear(0.0, fx[0], mm[0], fm[0]));
		if(valido(mm[1]))
		{
			if(sign(fm[0]) * sign(fm[1]) <= 0)
				return acharNewton(buscaLinear(mm[0], fm[0], mm[1], fm[1]));
		}
		if(sign(fm[0]) * sign(fx[1]) <= 0)
			return acharNewton(buscaLinear(mm[0], fm[0], 1.0, fx[1]));
	}

	if(valido(mm[1]))
	{
		if(sign(fx[0]) * sign(fm[1]) <= 0)
			return acharNewton(buscaLinear(0.0, fx[0], mm[1], fm[1]));
  
		if(sign(fm[1]) * sign(fx[1]) <= 0)
			return acharNewton(buscaLinear(mm[1], fm[1], 1.0, fx[1]));
	}

	if(sign(fx[0]) * sign(fx[1]) <= 0)
		return acharNewton(buscaLinear(0.0, fx[0], 1.0, fx[1]));

	return -1.0;
}

float casoCubica()
{
	cor = vec4(0.0,1.0,0.0,1.0);

	return acharTCubicaBuscaBinaria();
}

float casoQuadrica()
{
	float qa = b;
	float qb = c;
	float qc = d;
	vec2 r;

	if(!acharRaiz(qa, qb, qc, r))
		discard;

	if(r[0] < 0.0)
		return r[1];
	else
		return r[0];
}

float casoLinear()
{
	return -d/c;
}

float _2(float v)
{
	return v*v;
}

void main ()
{
	cor = vec4(1.0);

	vec4 dir = po - pi;

	float ddi = dot(dir, Qi * dir);
	float pdi = dot(pi, Qi * dir);
	float ppi = dot(pi, Qi * pi);

	float ddo = dot(dir, Qo * dir);
	float pdo = dot(pi, Qo * dir);
	float ppo = dot(pi, Qo * pi);

/*
	a = ddo - ddi;
	b = 2*(pdo - pdi) + ddi;
	c = 2*pdi + ppo - ppi;
	d = ppi;
*/

	a = _2(dir.z)*dir.z;
	b = _2(dir.x) + _2(dir.y) - _2(dir.z) + 3*_2(dir.z)*pi.z;
	c = 2*(dir.x*pi.y + dir.x*pi.y - dir.z*pi.z) + 3*dir.z*_2(pi.z);
	d = _2(pi.x) + _2(pi.y) - _2(pi.z) + _2(pi.z)*pi.z;


	float t = -1.0;
	
	if(abs(a) > err) //a != 0
		t = casoCubica();
	else if(abs(b) > err) // b != 0
		t = casoQuadrica();
	else if(abs(c) > err)
		t = casoLinear();
	
	if(!valido(t))
		discard;
  	
	vec3 p =  pi.xyz + t*dir.xyz;
	mat4 Q = (1.0 - t)*Qi + t*Qo;
	//vec3 n = normalize((mat3(tQ) * p));
	vec3 n = normalize(vec3(2*p.x, 2*p.y, 3*_2(p.z)-2*p.z));

	gl_FragColor = cor * shade(p,n,normalize(-dir.xyz));
	gl_FragColor.a = 1.0;	
}