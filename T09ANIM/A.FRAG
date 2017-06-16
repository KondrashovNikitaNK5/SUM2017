#version 330

uniform float GTime;
uniform float Time;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform mat4 MatrV;

in vec3 DrawNormal;
in vec3 DrawPosition;

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
  vec3 Ka = vec3(0.1, 0.1, 0.1);
  color += Ka;

  // Diffuse
  vec3 Kd = vec3(0.8, 0.8, 0.8);
  vec3 L = normalize(LightPos - P);
  float nl = dot(L, N);
  color += Kd * LightColor * max(nl, 0);

  // Specular
  vec3 Ks = vec3(0.25, 0.35, 0.75);
  float Ph = 50;
  vec3 R = reflect(V, N);
  float rl = dot(L, R);
  color += Ks * max(pow(rl, Ph), 0);

  return color;
}

void main( void )
{
  gl_FragColor = vec4(Shade(DrawPosition, DrawNormal), 1);
}
