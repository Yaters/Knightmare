#pragma once //One or the other. Not both
/*
#ifndef SHADER_H
#define SHADER_H
*/


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
	// the program ID
	GLuint ID;

public:
	// constructor reads and builds the shader
	Shader();

	void Compile(const GLchar* vCode, const GLchar* fCode, const GLchar* gCode);

	// use/activate the shader
	Shader& Use();
	void End();

	// utility uniform functions
	void    SetFloat(const GLchar* name, GLfloat value);
	void    SetInteger(const GLchar* name, GLint value);
	void    SetVector2f(const GLchar* name, GLfloat x, GLfloat y);
	void    SetVector2f(const GLchar* name, const glm::vec2& value);
	void    SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
	void    SetVector3f(const GLchar* name, const glm::vec3& value);
	void    SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	void    SetVector4f(const GLchar* name, const glm::vec4& value);
	void    SetMatrix4(const GLchar* name, const glm::mat4& matrix);

};

//#endif