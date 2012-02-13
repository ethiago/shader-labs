#version 120
#extension GL_ARB_gpu_shader5 : enable

// Receberei como atributo
mat4 tetraIn = mat4(-1.0, -1.0, -1.0, 1.0,
                     1.0,  1.0, -1.0, 1.0,
                     1.0, -1.0,  1.0, 1.0,
                    -1.0,  1.0,  1.0, 1.0);

//Receberei como atributos
varying vec4 dAtt;

varying mat4 osTetra;
varying mat4 psTetra;
varying mat4 cor;
varying vec4 d;

void main ()
{
	dAtt[0] = 0.3;
	dAtt[1] = 0.5;
	dAtt[2] = 0.7;
	dAtt[3] = 0.9;

	osTetra[0] = tetraIn[0];
	osTetra[1] = tetraIn[1];
	osTetra[2] = tetraIn[2];
	osTetra[3] = tetraIn[3];

	cor[0] = vec4(0.0, 0.0, 0.0, 1.0);
	cor[1] = vec4(1.0, 0.0, 0.0, 1.0);
	cor[2] = vec4(0.0, 1.0, 0.0, 1.0);
	cor[3] = vec4(0.0, 0.0, 1.0, 1.0);
	
	psTetra[0] = gl_ModelViewProjectionMatrix * tetraIn[0];
	psTetra[1] = gl_ModelViewProjectionMatrix * tetraIn[1];
	psTetra[2] = gl_ModelViewProjectionMatrix * tetraIn[2];
	psTetra[3] = gl_ModelViewProjectionMatrix * tetraIn[3];

	d[0] = dAtt[0];
	d[1] = dAtt[1];
	d[2] = dAtt[2];
	d[3] = dAtt[3];

	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
}