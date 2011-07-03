
//
// granite.frag: Fragment shader for producing a granite effect
//
// author: Randi Rost
//
// Copyright (c) 2002: 3Dlabs, Inc.
//

varying float LightIntensity; 
varying vec3  MCposition;

uniform sampler3D Noise;
uniform float NoiseScale;
uniform vec3 Offset;

void main(void)
{
    vec4  noisevec  = texture3D(Noise, NoiseScale * (MCposition + Offset));
    float intensity = min(1.0, noisevec[3] * 18.0);
    vec3  color     = vec3 (intensity * LightIntensity);
    color = clamp(color, 0.0, 1.0);
    gl_FragColor    = vec4 (color, 1.0);
}
