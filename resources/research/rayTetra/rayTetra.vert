#version 120
#extension GL_ARB_gpu_shader5 : enable


const mat4 sphere       = mat4(1.0,0.0,0.0,0.0, 0.0, 1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 saddle       = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0,-1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 paraboloide  = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0,-0.5, 0.0,0.0, 1.0,0.0, 0.0,-0.5,0.0, 0.0);
const mat4 cone         = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.0);
const mat4 cylinder     = mat4(1.0,0.0,0.0,0.0, 0.0, 0.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0,-0.25);
const mat4 para_2she    = mat4(1.0,0.0,0.0,0.0, 0.0,-1.0,0.0, 0.0, 0.0,0.0, 1.0,0.0, 0.0, 0.0,0.0, 0.25);


// Receberei como atributo
mat4 tetraIn = mat4(-1.0, -1.0, -1.0, 1.0,
                     1.0,  1.0, -1.0, 1.0,
                     1.0, -1.0,  1.0, 1.0,
                    -1.0,  1.0,  1.0, 1.0);

//Receberei como atributos
varying mat4 Q0;
varying mat4 Q1;
varying mat4 Q2;
varying mat4 Q3;

varying mat4 osTetra;
varying mat4 psTetra;
varying mat4 cor;

mat4 trans0;
mat4 trans1;
mat4 trans2;
mat4 trans3;

void main ()
{
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

	Q0 = para_2she;
	Q1 = saddle;
	Q2 = cylinder;
	Q3 = cone;

	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
}