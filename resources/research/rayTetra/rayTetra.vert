const mat4 tQ = mat4(1.0,0.0,0.0,0.0, 0.0,1.0,0.0,0.0, 0.0,0.0,1.0,0.0, 0.0,0.0,0.0,-0.5);

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

// Receberei QIn e entao sera Q = QIn;
	Q0 = tQ;
	Q1 = tQ;
	Q2 = tQ;
	Q3 = tQ;

	gl_Position = gl_Vertex;
	gl_FrontColor = gl_Color;
}