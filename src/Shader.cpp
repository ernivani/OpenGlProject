#include "Shader.h"

Shader::Shader(const char* vertexShaderPath, const char* fragmentPath) {

	int sucess;
	char infoLog[512];

	GLuint vertexShader = compileShader(vertexShaderPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = compileShader(fragmentPath, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);

	glGetProgramiv(id, GL_LINK_STATUS, &sucess);
	if (!sucess) {
		glGetProgramInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void Shader::activate() {
	glUseProgram(id);
}


std::string Shader::loadShaderSrc(const char* filename) {

	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open file " << filename << std::endl;
	}

	file.close();

	return ret;
}

GLuint Shader::compileShader(const char* filename, GLenum type) {
	int sucess;
	char infoLog[512];

	GLuint ret = glCreateShader(type);
	std::string shaderSrc = Shader::loadShaderSrc(filename);
	const GLchar* shaderChar = shaderSrc.c_str();
	glShaderSource(ret, 1, &shaderChar, NULL);
	glCompileShader(ret);

	glGetShaderiv(ret, GL_COMPILE_STATUS, &sucess);
	if (!sucess) {
		glGetShaderInfoLog(ret, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	return ret;

	
}

// uniform 
void Shader::setMat4(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	
}

void Shader::setInt(const std::string& name, int value) {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}