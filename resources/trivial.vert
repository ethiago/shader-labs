#version 140
 
uniform Transformation {
    mat4 projection_matrix;
    mat4 modelview_matrix;
};
 
in vec3 vertex;
 
void main() {
    gl_Position = projection_matrix * modelview_matrix * vec4(vertex, 3.0);
}

