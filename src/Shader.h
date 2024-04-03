#pragma once

#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <glm.hpp>

class Shader
{
public:
	// program ID
	unsigned int id;

	// constructor reads and builds the shader
	Shader(const char* vertexShaderPath, const char* fragmentPath);
	
	void activate();

	// utility functions
	std::string loadShaderSrc(const char* filename);
	GLuint compileShader(const char* filename, GLenum type);

	// uniform 
	void setMat4(const std::string& name, glm::mat4 value);

	void setInt(const std::string& name, int value);

	void setFloat(const std::string& name, float value);

};
#endif 

