
//
// texture.vert: Vertex shader for wobbling a texture
//
// author: Antonio Tejada
//
// Copyright (c) 2002: 3Dlabs, Inc.
//

varying float lightIntensity;
varying vec2 TexCoord;

const float specularContribution = 0.1;
const float diffuseContribution  = (1.0 - specularContribution);

void main(void) {
    vec3 LightPosition = gl_LightSource[0].position.xyz;
    vec4 pos        = gl_ModelViewMatrix * gl_Vertex;
    vec3 tnorm      = normalize(gl_NormalMatrix * gl_Normal);
    vec3 lightVec   = normalize(LightPosition - vec3(pos));
    vec3 reflectVec = reflect(lightVec, tnorm);
    vec3 viewVec    = normalize(vec3(pos));

    float spec = max(dot(reflectVec, viewVec), 0.0);
    spec = pow(spec, 16.0);

    lightIntensity = diffuseContribution * max(dot(lightVec, tnorm), 0.4) +
                     specularContribution * spec;

    TexCoord = gl_MultiTexCoord0.xy;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
