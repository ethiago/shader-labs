#version 150 compatibility
#extension GL_ARB_gpu_shader_fp64 : enable

in mat4 Qi;
noperspective in mat4 Qo;
mat4 Qo2;

in vec4 pi;
noperspective in vec4 po;
vec4 po2;

in vec4 cori;
noperspective in vec4 coro;
vec4 coro2;

noperspective in float woI;

const float ac = 0.3;
const float dc = 0.4;
const float sc = 0.3;
const float err = 0.00001;
const float err2 = 0.001;

vec3 lp = vec3(gl_ModelViewMatrixInverse * gl_LightSource[0].position);
vec4 O = gl_ModelViewMatrixInverse * vec4(0.0,0.0,0.0,1.0);

float a,b,c,d;

vec4 cor;

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
	po2 = po/woI;
	po2 /= po2.w;
	Qo2 = Qo/woI;
	coro2 = coro/woI;


	cor = vec4(1.0);
	vec4 dir = po2 - pi;

	float ddi = dot(dir, Qi * dir);
	float pdi = dot(pi, Qi * dir);
	float ppi = dot(pi, Qi * pi);

	float ddo = dot(dir, Qo2 * dir);
	float pdo = dot(pi, Qo2 * dir);
	float ppo = dot(pi, Qo2 * pi);


    a = ddo - ddi;
	b = 2*(pdo - pdi) + ddi;
	c = 2*pdi + ppo - ppi;
	d = ppi;

	float t = -1.0;		
	
	if(abs(a) > err) //a != 0
		t = casoCubica();
	else if(abs(b) > err) // b != 0
		t = casoQuadrica();
	else if(abs(c) > err)
		t = casoLinear();

	if(!valido(t))
		discard;

	vec4 p =  pi + t*dir;
	p.w = 1.0;

	mat4 Q = (1.0 - t)*Qi + t*Qo2;
 
	vec3 n = normalize((Q * p).xyz);

	cor = (1.0 - t)*cori + t*coro2;
	gl_FragColor = cor * shade(p.xyz, n ,normalize(-dir.xyz));
	gl_FragColor.a = 1.0;

//<Controlling the Viewport> http://www.opengl.org/registry/doc/glspec41.core.20100725.pdf   pag 137
	vec4 pd = gl_ModelViewProjectionMatrix * p; 
  gl_FragDepth = ((gl_DepthRange.far - gl_DepthRange.near) * pd.z/pd.w + gl_DepthRange.near + gl_DepthRange.far)/2.0 ;
//</Controlling the Viewport>

}