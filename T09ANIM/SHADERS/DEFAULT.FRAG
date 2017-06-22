#version 420

uniform float GTime;
uniform float Time;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform mat4 MatrV;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Ph;

in vec3 DrawNormal;
in vec3 DrawPosition;
in vec2 DrawTexCoord;

uniform int IsTexture0;
layout(binding = 0) uniform sampler2D Texture0;

vec3 Shade( vec3 P, vec3 N )
{
  // Resut color
  vec3 color = vec3(0, 0, 0);

  vec3 V = -transpose(MatrV)[2].xyz;
  // Correct normal
  //N = faceforward(N, -V, N);
  //if (dot(N, V) >= 0.0001)
  //  N = -N;

  // Ambient
  //vec3 Ka = vec3(0.1, 0.1, 0.1);
  color += Ka;

  // Diffuse
  vec3 Kds = Kd;
  if (IsTexture0 == 1)
    Kds = texture2D(Texture0, vec2(0, 1) + vec2(1, -1) * DrawTexCoord).rgb;
  //return Kds;
  vec3 L = normalize(LightPos - P);
  float nl = dot(L, N);
  color += Kds * LightColor * max(nl, 0);

  // Specular
  vec3 R = reflect(V, N);
  float rl = dot(L, R);
  color += Ks * max(pow(rl, Ph), 0);

  return color;
}

void main( void )
{
  gl_FragColor = vec4(Shade(DrawPosition, DrawNormal), 1);
}