#version 120

varying mat4 Qi;
varying mat4 Qo;

varying vec4 pi;
varying vec4 po;

const float ac = 0.3;
const float dc = 0.4;
const float sc = 0.3;
const float err = 0.00001;

vec3 lp = vec3(gl_ModelViewMatrixInverse * gl_LightSource[0].position);

float a,b,c,d;

float f(float t)
{
	return a*(t*t*t) + b*(t*t) + c*t + d;
}

float df(float t)
{
	return 3*a*(t*t) + 2*b*(t) + c;
}

float shade(in vec3 p, in vec3 n, in vec3 v)
{
	vec3 lv = normalize(lp - p);
	float dif = max(dot(lv, n), 0.0);
	
	float spec = max(dot(reflect(-lv, n), v), 0.0);
	spec = pow(spec, 16.0);

	return dif*dc + spec*sc + ac;
}

float achar1oTCubica()
{
	float t = 0.0;
	float t0 = 0.0;
	do{
		t0 = t;
		float passo = f(t0)/df(t0);
		//if(abs(passo) > 0.5)
		//	passo = 0.1*(passo/abs(passo));

		t = t0 - passo;
	}while(abs(t-t0) > err);
	return t;
}

float acharProxTCubica_Briot_Ruffini(float t0)
{
	float ra = a;
	float rb = ra*t0 + b;
	float rc = rb*t0 + c;

	float delta = rb*rb - 4*ra*rc;
	float t = -1.0;
	if(delta >= 0.0)
	{
		delta = sqrt(delta);
		t = (-rb - delta)/2.0*ra;
		if(t < 0.0)
			t = (-rb + delta)/2.0*ra;
	}
	return t;
}

float casoCubica()
{
	float t = achar1oTCubica();
	float t1 = acharProxTCubica_Briot_Ruffini(t);

	if((t < 0.0 || t > 1.0) && (t1 < 0.0 || t1 > 1.0))
		t = -13.5;
	else if(t < 0.0 || t > 1.0)
		t = t1;
	else if(t1 >= 0.0 && t1 <= 1.0)
		t = min(t,t1);
	
	return t;
}

float casoQuadrica()
{
	float qa = b;
	float qb = c;
	float qc = d;

	float delta = qb*qb - 4*qa*qc;
	float t =  -12.5;

	if(delta > 0.0)
	{
		delta = sqrt(delta);
		t = (-qb - delta)/2*qa;
		if(t < 0.0)
			t = (-qb + delta)/2*qa;
	}

	return t;
}

float casoLinear()
{
	return -d/c;
}


void main ()
{
	vec4 cor = vec4(1.0);

	vec4 dir = po - pi;
	
	float ddi = dot(dir, Qi * dir);
	float pdi = dot(pi, Qi * dir);
	float ppi = dot(pi, Qi * pi);

	float ddo = dot(dir, Qo * dir);
	float pdo = dot(pi, Qo * dir);
	float ppo = dot(pi, Qo * pi);

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
	
	if(t < 0.0 || t > 1.0)
		discard;
	
	vec3 p =  pi.xyz + t*dir.xyz;
	mat4 Q = (1.0 - t)*Qi + t*Qo;
	vec3 n = normalize((mat3(Q) * p));

	gl_FragColor = cor * shade(p,n,normalize(-dir.xyz));
	gl_FragColor.a = 1.0;	
}