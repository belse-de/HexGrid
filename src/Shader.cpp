#include "Shader.hpp"
Shader::Shader(const char* path, GLenum type) : type(type)
{
  // 1. Retrieve the source code from filePath
  std::string code;
  std::ifstream shaderFile;
  // ensures ifstream objects can throw exceptions:
  shaderFile.exceptions(std::ifstream::badbit);
  try 
  {
    // Open files
    shaderFile.open(path);
    std::stringstream shaderStream;
    // Read file's buffer contents into streams
    shaderStream << shaderFile.rdbuf();		
    // close file handlers
    shaderFile.close();
    // Convert stream into GLchar array
    code = shaderStream.str();
  }
  catch(std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }
  
  const char* shaderCode = code.c_str();
  ShaderID = glCreateShader( type );
  glShaderSource(ShaderID, 1, &shaderCode, NULL);
  glCompileShader(ShaderID);
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
    glDeleteShader(ShaderID);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
              << path << "\n" 
              << infoLog << std::endl;
    throw "ERROR::SHADER::COMPILATION_FAILED";
  }
}

Shader::~Shader()
{
  glDeleteShader(ShaderID);
}

ShaderProgram::ShaderProgram(std::vector<GLuint> shaders)
//ShaderProgram::ShaderProgram(std::vector<Shader> shaders)
{      
  // Shader Program
  ProgramID = glCreateProgram();
  for( auto shader : shaders)
  {
    //std::cerr << ProgramID << " : " << shader.ShaderID << std::endl;
    //glAttachShader(ProgramID, );
    std::cerr << ProgramID << " : " << shader << std::endl;
    glAttachShader(ProgramID, shader);
  }
  
  glLinkProgram(ProgramID);
  // Print linking errors if any
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(ProgramID, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    throw "ERROR::SHADER::PROGRAM::LINKING_FAILED";
  }
}

ShaderProgram::~ShaderProgram()
{
  glUseProgram(0);
	glDeleteProgram(ProgramID);
}

void ShaderProgram::use() { glUseProgram(ProgramID); }  
