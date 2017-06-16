#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTextCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform mat4 MatrWVP;
uniform mat4 MatrW;
uniform float Time;

out vec3 DrawNormal;
out vec3 DrawPosition;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawPosition = vec3(MatrW * vec4(InPosition, 1));
  DrawNormal = mat3(inverse(transpose(MatrW))) * InNormal;
}