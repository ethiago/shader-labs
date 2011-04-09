void main() {
    vec4 v = gl_ModelViewProjectionMatrix * gl_Vertex;
    v.x *= 2;
    gl_Position = v;
}

