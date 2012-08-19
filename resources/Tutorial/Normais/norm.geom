#extension GL_EXT_geometry_shader4 : enable

in vec2 texCoordIn[];
in vec3 nIn[];
in vec3 lvIn[];

varying out vec2 texCoord;
varying out vec3 n;
varying out vec3 lv;

void main()
{
	


	for(int i = 0; i < 3; ++i)
	{
		gl_Position = gl_PositionIn[i];
		texCoord = texCoordIn[i];
		n = nIn[i];
		lv = lvIn[i];
		EmitVertex();
	}
	EndPrimitive();	
}