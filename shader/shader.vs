#version 330 core    
layout (location = 0) in vec3 position; // The position variable has attribute position 0
layout (location = 1) in vec3 color;	  // The color variable has attribute position 1

out vec3 vertexColor;                   // Specify a color output to the fragment shader

void main()
{
  gl_Position = vec4(position, 1.0);    // See how we directly give a vec3 to vec4's constructor
  vertexColor = color;                  // Set ourColor to the input color we got from the vertex data
}
