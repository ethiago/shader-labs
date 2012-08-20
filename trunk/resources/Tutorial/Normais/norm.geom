#extension GL_EXT_geometry_shader4 : enable

in vec4 eye[];
in vec3 N[];

void main()
{

	vec3 v1 = vec3(eye[1] - eye[0]);
	vec3 v2 = vec3(eye[2] - eye[0]);
	vec3 n = cross(v1,v2).xyz;
	n = normalize(n);
	vec4 p = eye[0]+eye[1]+eye[2];

	gl_FrontColor = vec4(n,1.0);
	gl_Position = gl_ProjectionMatrix * p;
	EmitVertex();
	gl_Position = gl_ProjectionMatrix * (p + vec4(n*0.1,1.0));
	EmitVertex();
	EndPrimitive();
}