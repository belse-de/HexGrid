#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <cstdlib>

#include <iostream>

#include "Hexagon/libhex.hpp"

using namespace std;
using namespace glm;
using namespace Hexagon;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
GLfloat vertices[] = {
  // position           // color
   0.5f,  0.5f, 0.0f,   //+1.0f, +0.0f, +0.0f, // Top Right
   0.5f, -0.5f, 0.0f,   //+0.0f, +1.0f, +0.0f, // Bottom Right
  -0.5f, -0.5f, 0.0f,   //+0.0f, +0.0f, +1.0f, // Bottom Left
  -0.5f,  0.5f, 0.0f,   //+1.0f, +1.0f, +0.0f, // Top Left 
   0.0f,  0.0f, 0.0f,   //+0.0f, +1.0f, +1.0f, // Center
};
GLuint indices[] = {  // Note that we start from 0!
    0, 1, 2,   // First Triangle
    4, 3, 2    // Second Triangle
};  

static const GLchar* vertexShaderSource =
"#version 330 core\n"    
"layout (location = 0) in vec3 position; // The position variable has attribute position 0\n"
"\n"
"out vec4 vertexColor; // Specify a color output to the fragment shader\n"
"\n"
"void main()\n"
"{\n"
"  gl_Position = vec4(position, 1.0); // See how we directly give a vec3 to vec4's constructor\n"
"  vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // Set the output variable to a dark-red color\n"
"}\n";

static const GLchar* fragmentShaderSource =
"#version 330 core\n"
//"in vec4 vertexColor; // The input variable from the vertex shader (same name and same type)\n"
"\n"
"out vec4 color;\n"
"\n"
"uniform vec4 ourColor; // We set this variable in the OpenGL code.\n"
"\n"
"void main()\n"
"{\n"
//"  color = vertexColor;\n"
"  color = ourColor;\n"
"}\n";

int main(int argc, char **argv, char **env)
{
  cout << "Hello World!" << endl;
  
  glfwInit();
  atexit(glfwTerminate);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  
  glfwSetKeyCallback(window, key_callback);
  
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }
  
  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);
  
  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  cout << "Maximum nr of vertex attributes supported: " << nrAttributes << endl;
  
  
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
      exit(EXIT_FAILURE);
  }
  
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
      glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
      exit(EXIT_FAILURE);
  }
  
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
      exit(EXIT_FAILURE);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);  
  
  
  
  GLuint VAO; // Vertex Array Object
  glGenVertexArrays(1, &VAO);   
  
  GLuint VBO; // Vertex Buffer Object
  glGenBuffers(1, &VBO);
  
  GLuint EBO; // Element Array Buffer Object
  glGenBuffers(1, &EBO);
  
  // Bind Vertex Array Object
  glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copy our vertices array in a buffer for OpenGL to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //  Copy our index array in a element buffer for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // Then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0); 
  // Unbind the VAO (NOT the EBO)
  glBindVertexArray(0);
  


  // Program loop
  while(!glfwWindowShouldClose(window))
  {
    // Check and call events
    glfwPollEvents();
    
    // clear buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Rendering commands here  
    // Be sure to activate the shader
    glUseProgram(shaderProgram);

    
    // Update the uniform color
    GLfloat timeValue = glfwGetTime();
    GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
    
    
    // DRAW CODE
    glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    
    // Swap the buffers
    glfwSwapBuffers(window);
  }
    
  cout << "Good Bye." << endl;
  // Properly de-allocate all resources once they've outlived their purpose
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glfwTerminate();
  exit(EXIT_SUCCESS);
} 

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
  // When a user presses the escape key, we set the WindowShouldClose property to true, 
  // closing the application
  if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  	glfwSetWindowShouldClose(window, GL_TRUE);
  else if(key == GLFW_KEY_F && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else if(key == GLFW_KEY_L && action == GLFW_PRESS)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
