uniform sampler2D sampler2d0;

varying vec2 texCoord;
varying vec3 n;
varying vec3 lv;

void main()
{
	vec4 cor = texture2D(sampler2d0, texCoord);

	float diff = max(dot(normalize(n),normalize(lv)),0.0);

	gl_FragColor = cor*0.2 + cor*diff;
	gl_FragColor.a = 1.0;
}