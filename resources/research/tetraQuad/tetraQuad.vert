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

varying mat4 osTetraIn;
varying mat4 psTetraIn;
varying mat4 corIn;

varying mat4 mvp;

mat4 trans0;
mat4 trans1;
mat4 trans2;
mat4 trans3;

void main ()
{
	mvp = gl_ModelViewProjectionMatrix;

	osTetraIn[0] = tetraIn[0];
	osTetraIn[1] = tetraIn[1];
	osTetraIn[2] = tetraIn[2];
	osTetraIn[3] = tetraIn[3];

	corIn[0] = vec4(0.0, 0.0, 0.0, 1.0);
	corIn[1] = vec4(1.0, 0.0, 0.0, 1.0);
	corIn[2] = vec4(0.0, 1.0, 0.0, 1.0);
	corIn[3] = vec4(0.0, 0.0, 1.0, 1.0);
	
	psTetraIn[0] = gl_ModelViewProjectionMatrix * tetraIn[0];
	psTetraIn[1] = gl_ModelViewProjectionMatrix * tetraIn[1];
	psTetraIn[2] = gl_ModelViewProjectionMatrix * tetraIn[2];
	psTetraIn[3] = gl_ModelViewProjectionMatrix * tetraIn[3];

	Q0 = cone;
	Q1 = saddle;
	Q2 = sphere;
	Q3 = cone;

	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
}