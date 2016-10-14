#pragma once
#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
  
#include <GL/glew.h> // Include glew to get all the required OpenGL headers

class Shader
{
  public:
    Shader(const char* path, GLenum type);
    virtual ~Shader();
    GLuint ShaderID;
    GLenum type;
};

class ShaderProgram
{
  public:
	  // The program ID
	  GLuint ProgramID;
	  // Constructor reads and builds the shader
	  //ShaderProgram(std::vector<Shader> shaders);
	  ShaderProgram(std::vector<GLuint> shaders);
	  virtual ~ShaderProgram();
	  // Use the program
	  void use();
};
  
#endif /* SHADER_HPP */
