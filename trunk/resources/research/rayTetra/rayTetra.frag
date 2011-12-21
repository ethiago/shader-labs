#version 120

varying mat4 Qi;
varying mat4 Qo;

varying vec4 pos;

const float ac = 0.3;
const float dc = 0.4;
const float sc = 0.3;

vec4  O = gl_ModelViewMatrixInverse * vec4(vec3(0.0), 1.0);
vec3 lp = vec3(gl_ModelViewMatrixInverse * gl_LightSource[0].position);

float shade(in vec3 p, in vec3 n, in vec3 v)
{
	vec3 lv = normalize(lp - p);
	float dif = max(dot(lv, n), 0.0);
	
	float spec = max(dot(reflect(-lv, n), v), 0.0);
	spec = pow(spec, 16.0);

	return dif*dc + spec*sc + ac;
}

void main ()
{
	vec4 d = pos - O;
	vec4 cor = gl_Color;

	float a = dot(d, Qi * d);
	float b = dot(O, Qi * d);
	float c = dot(O, Qi * O);

	float delta = b*b - a*c;

	if(delta < 0.0)	
		discard;
	delta = sqrt(delta);

	float t1 = (-b - delta) / a;
	float t2 = (-b + delta) / a;

	float t = 100000.0;
	float m = 1.0;
	cor = vec4(1.0,0.0,0.0,1.0);
	if(t1 >= 1.0 && t1 < t)
		t = t1;

	if(t2 >= 1.0 && t2 < t)
	{
		t = t2;
		m = -1.0;
		cor = vec4(0.0,1.0,0.0,1.0);
	}

	if(t > 2.0)
		discard;
	
	vec3 p =  O.xyz + t*d.xyz;
	vec3 n = normalize((mat3(Qi) * p));

	n = m*n;

	cor = vec4(n,1.0);

	gl_FragColor = cor * shade(p,n,normalize(-d.xyz));
//	gl_FragColor = cor;
	gl_FragColor.a = 1.0;	
}