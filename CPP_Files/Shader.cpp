#include "Shader.h"

#include <iostream>

Shader::Shader() : ID(0) {}
Shader& Shader::Use() {
	glUseProgram(this->ID);
	return *this;
}
void Shader::End() {
	glDeleteProgram(this->ID);
}

void Shader::Compile(const GLchar* vCode, const GLchar* fCode, const GLchar* gCode) {
	int success;
	char infoLog[512];

	//Create each w/ ID
	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint geometry;
	if (gCode != nullptr) {
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		//I only output cause I doubt I'll use one
		std::cout << "There is a Geometry Shader" << std::endl;
	}

	this->ID = glCreateProgram();
	glAttachShader(this->ID, vertex);
	glAttachShader(this->ID, fragment);
	if (gCode != nullptr) {
		glAttachShader(this->ID, geometry);
	}
	glLinkProgram(this->ID); //Creates executable, so no longer need objects
	glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//delete objects
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (gCode != nullptr) {
		glDeleteShader(geometry);
	}
}
//Uniform Setters
void Shader::SetFloat(const GLchar* name, GLfloat value) {
	glUniform1f(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetInteger(const GLchar* name, GLint value) {
	glUniform1i(glGetUniformLocation(this->ID, name), value);
}
void Shader::SetVector2f(const GLchar* name, GLfloat x, GLfloat y) {
	glUniform2f(glGetUniformLocation(this->ID, name), x, y);
}
void Shader::SetVector2f(const GLchar* name, const glm::vec2& value) {
	glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
}
void Shader::SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) {
	glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
}
void Shader::SetVector3f(const GLchar* name, const glm::vec3& value) {
	glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
	glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
}
void Shader::SetVector4f(const GLchar* name, const glm::vec4& value) {
	glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const GLchar* name, const glm::mat4& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
