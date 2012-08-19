uniform sampler2D sampler2d0;

varying vec2 texCoord;
varying float diff;
varying float spec;

void main()
{
	vec4 cor = texture2D(sampler2d0, texCoord);

	gl_FragColor = cor*0.2 + cor*diff + vec4(spec);
	gl_FragColor.a = 1.0;
}