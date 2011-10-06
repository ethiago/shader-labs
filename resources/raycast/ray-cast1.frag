varying vec3 center;
varying vec3 vv;
varying vec3 lp2;

const float r = 1.0;
const float shininess = 100.0;

void main ()
{
	vec3 vv2 = normalize(vv);
	float b = dot(vv2, center);
	float c = dot(center, center) - r*r;

	float delta = b*b - c;

	if(delta < 0.0)
		discard;
	
	float t = (-b - sqrt(delta));
	vec3 inter = t*vv2;
	vec3 norm = normalize(inter - center);
	vec3 lv = normalize(lp2 - inter);

	float diff = dot(norm, lv);
	lv = normalize(reflect(lv, norm));
	float spec = pow(max(dot(vv,lv),0.0), shininess);

	
	gl_FragColor = vec4(gl_Color.xyz * (diff ), 1.0);
}