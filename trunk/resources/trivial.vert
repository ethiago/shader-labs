void main() {
    vec4 v = gl_ModelViewProjectionMatrix * gl_Vertex;
    v.x *= 1.6;
    gl_Position = v;
}