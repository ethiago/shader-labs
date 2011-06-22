void main() {
    vec4 v = gl_Vertex;
    v.x *= 1.7;
    gl_Position = gl_ModelViewProjectionMatrix * v;
}