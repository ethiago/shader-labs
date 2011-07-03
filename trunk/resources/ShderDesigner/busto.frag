uniform sampler2D diffuse1;
uniform sampler2D diffuse2;

void main()
{
  vec4 dif = texture2D(diffuse1, gl_TexCoord[0].st);
  vec4 dif2 = texture2D(diffuse2, gl_TexCoord[0].st);
  gl_FragColor = vec4(dif2.a);
}
