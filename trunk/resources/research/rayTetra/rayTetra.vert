#version 120
#extension GL_ARB_gpu_shader5 : enable


const mat4 tQ = mat4(1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,-0.25);

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

	psTetra[0] = gl_ModelViewProjectionMatrix * tetraIn[0];
	psTetra[1] = gl_ModelViewProjectionMatrix * tetraIn[1];
	psTetra[2] = gl_ModelViewProjectionMatrix * tetraIn[2];
	psTetra[3] = gl_ModelViewProjectionMatrix * tetraIn[3];

	trans0 = mat4(1.0, 0.0, 0.0,-1.0,
				  0.0, 1.0, 0.0,-1.0,
				  0.0, 0.0, 1.0,-1.0,
				  0.0, 0.0, 0.0, 1.0);
	trans1 = mat4(1.0, 0.0, 0.0, 1.0,
				  0.0, 1.0, 0.0, 1.0,
				  0.0, 0.0, 1.0,-1.0,
				  0.0, 0.0, 0.0, 1.0);
	trans2 = mat4(1.0, 0.0, 0.0, 1.0,
				  0.0, 1.0, 0.0,-1.0,
				  0.0, 0.0, 1.0, 1.0,
				  0.0, 0.0, 0.0, 1.0);
	trans3 = mat4(1.0, 0.0, 0.0,-1.0,
				  0.0, 1.0, 0.0, 1.0,
				  0.0, 0.0, 1.0, 1.0,
				  0.0, 0.0, 0.0, 1.0);

	trans0 = inverse(trans0);
	trans1 = inverse(trans1);
	trans2 = inverse(trans2);
	trans3 = inverse(trans3);

	
	Q0 = trans0*tQ*transpose(trans0);
	Q1 = trans1*tQ*transpose(trans1);
	Q2 = trans2*tQ*transpose(trans2);
	Q3 = trans3*tQ*transpose(trans3);

//*
	Q0 = tQ;
	Q1 = tQ;
	Q2 = tQ;
	Q3 = tQ;
//*/
	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
}