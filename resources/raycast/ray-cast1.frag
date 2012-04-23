
varying vec3 vv;

const float r = 1.0;
const float shininess = 8.0;
vec3 c1 = vec3(0.2,0.2,0.2);
vec3 c2 = vec3(-0.2,-0.2,-0.2);
const float specContrib = 0.25;
const float diffContrib = 0.45;
const float ambContrib = 0.3;

vec3 distSphere(in vec3 center, in vec3 vv)
{
    vec3 ret = vec3(-1.0, -1.0, 1.0);
	float a = dot(vv,vv);
	float b = dot(vv, center);
	float c = dot(center, center) - r*r;

	float delta = b*b - a*c;

	if(delta < 0.0)
	{
		ret[2] = -1.0;
	}else
	{
		delta = sqrt(delta);
		ret[0] = (b - delta)/(a);
		ret[1] = (b + delta)/(a);
	}

	return ret;
}

float intensity(in vec3 inter, in vec3 center, in vec3 vv)
{
	vv = normalize(vv);
	vec3 norm = normalize(inter - center);
	vec3 lv = normalize(gl_LightSource[0].position.xyz - inter);

	float diff = max(dot(norm, lv), 0.0);
	lv = normalize(reflect(lv, norm));
	float spec = pow(max(dot(vv,lv),0.0), shininess);

	return diff*diffContrib + spec*specContrib + ambContrib;
}

void main ()
{
	float t = -1.0;
	vec3 c = vec3(0.0);
	
	c1 = (gl_ModelViewMatrix * vec4(c1, 1.0)).xyz;
	c2 = (gl_ModelViewMatrix * vec4(c2, 1.0)).xyz;

	vec3 t1 = distSphere(c1, vv);
	vec3 t2 = distSphere(c2, vv);
	
 //Uniao
	if(t1[2] < 0.0 && t2[2] < 0.0)
		discard;

	if(t2[2] < 0.0)
	{
		t = t1[0];
		c = c1;
	}else if (t1[2] < 0.0)
	{
		t = t2[0];
		c = c2;
	}else if(t1[0] < t2[0])
	{
		t = t1[0];
		c = c1;
	}else
	{
		t = t2[0];
		c = c2;
	}

// intersecao
/*	if(t1[2] > 0.0 && t2[2] > 0.0)
	{
		if(t2[0] < t1[0])
		{
			vec3 tmp = t2;
			t2 = t1;
			t1 = tmp;
			tmp = c2;
			c2 = c1;
			c1 = tmp;
		}

		if(t2[0] < t1[1])
		{
			t = t2[0];
			c = c2;
		}else
			discard;
	}else discard;
*/

	float inten = intensity(t*vv,c, vv);
	
	
	gl_FragColor = vec4(gl_Color.xyz * inten, 1.0);
	//gl_FragColor = vec4(1.0);
}