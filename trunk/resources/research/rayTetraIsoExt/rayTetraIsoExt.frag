#version 120
#extension GL_ARB_gpu_shader_fp64 : enable

const float iso = 0.8;

varying float Di;
varying float Do;

varying vec4 pi;
varying vec4 po;

varying vec4 cori;
varying vec4 coro;

const float ac = 0.3;
const float dc = 0.4;
const float sc = 0.3;
const float err = 0.00001;
const float err2 = 0.001;

vec3 lp = vec3(gl_ModelViewMatrixInverse * gl_LightSource[0].position);
vec4 O = gl_ModelViewMatrixInverse * vec4(0.0,0.0,0.0,1.0);

float a,b,c,d;

vec4 cor;

//<CORRETO>
float casoLinear()
{
	return -d/c;
}

float _2(float v)
{
	return v*v;
}

float _3(float v)
{
	return v*v*v;
}

float shade(in vec3 p, in vec3 n, in vec3 v)
{
	//n = faceforward(n,n,v);
	vec3 lv = normalize(lp - p);
	float dif = max(dot(lv, n), 0.0);
	
	float spec = max(dot(reflect(-lv, n), v), 0.0);
	spec = pow(spec, 16.0);

	return dif*dc + spec*sc + ac;
}

bool acharRaiz(in float a, in float b, in float c, out vec2 raizes)
{
	raizes = vec2(-1.0);
	
	float delta = b*b - (4.0*a*c);
	
	if(delta < 0.0)
		return false;
	
	delta = sqrt(delta);
	
	raizes[0] = (-b - delta)/(2.0*a);
	raizes[1] = (-b + delta)/(2.0*a);
	
	if(raizes[1] < raizes[0])
	{
		float t = raizes[0];
		raizes[0] = raizes[1];
		raizes[1] = t;
	}

	return true;
}

bool valido(in float t)
{
	return (t >= 0.0 && t <= 1.0);
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
//</CORRETO>

//<NAOVERIFICADO>
float f(in float t)
{
	return a*(t*t*t) + b*(t*t) + c*t + d;
}

float df(in float t)
{
	return 3*a*(t*t) + 2*b*(t) + c;
}

float acharNewton(in float t0)
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
		r = vec2(-1.0);

	return r;
}

float buscaBinaria(in float x0,in float fx0,in float x1,in float fx1)
{
	float t = (x0+x1)/2.0;
	float ft;

	do{
		ft = f(t);
		if(sign(fx0) * sign(ft) <= 0)
		{
			x1 = t;
			fx1 = ft;
		}
		else
		{
			x0 = t;
			fx0 = ft;
		}
		t = (x0+x1)/2.0;
	}while(t-x0 > err2);

	return t;	
}

float acharTCubicaSeparacaoDominio()
{
	vec2 mm = acharMaxMinLocal();
	vec2 fx  = vec2(f(0.0), f(1.0));
	vec2 fm  = vec2(f(mm[0]), f(mm[1]));

	if(valido(mm[0]))
	{
		if(sign(fx[0]) * sign(fm[0]) <= 0)
			return acharNewton(buscaBinaria(0.0, fx[0], mm[0], fm[0])); 
			
		if(valido(mm[1]))
		{
			if(sign(fm[0]) * sign(fm[1]) <= 0)
				return acharNewton(buscaBinaria(mm[0], fm[0], mm[1], fm[1]));
		}else
		{		
			if(sign(fm[0]) * sign(fx[1]) <= 0)
				return acharNewton(buscaBinaria(mm[0], fm[0], 1.0, fx[1]));
		}
	}

	if(valido(mm[1]))
	{
		if(sign(fx[0]) * sign(fm[1]) <= 0)
			return acharNewton(buscaBinaria(0.0, fx[0], mm[1], fm[1]));
  
		if(sign(fm[1]) * sign(fx[1]) <= 0)
			return acharNewton(buscaBinaria(mm[1], fm[1], 1.0, fx[1]));
	}else
	{
		if(sign(fx[0]) * sign(fx[1]) <= 0)
			return acharNewton(buscaBinaria(0.0, fx[0], 1.0, fx[1]));
	}

	return -1.0;
}

float casoCubica()
{
	return acharTCubicaSeparacaoDominio();
}

void main ()
{
	float t;
	cor = vec4(1.0);

	if((Di > iso && Do > iso) || (Di < iso && Do < iso))
	{
		cor = vec4(1.0);
	}else
	{
		t = (iso - Di)/(Do - Di);

		vec3 p =  pi.xyz + t*(po-pi).xyz;

		cor = (1.0 - t)*cori + t*coro;
	}

	
	//gl_FragColor = cor * shade(p,n,normalize(-dir.xyz));
	gl_FragColor = cor;
	gl_FragColor.a = 1.0;	
}