#pragma once //One or the other. Not both
/*
#ifndef TEXTURE_H
#define TEXTURE_H
*/

#include <glad/glad.h>
#include <iostream>

// Texture2D is able to store and configure a texture in OpenGL.
class Texture2D {
protected:
	
	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image

	
	
public:
	// Texture image dimensions
	GLuint Width = 0, Height = 0; // In pixels: maybe I'll add it back some other time

	// Holds the ID of the texture object- might want to make private
	GLuint ID;
	
	// Constructor (sets default texture modes)
	Texture2D();

	void setParameters(GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint maxFilter);
	void setFormat(GLuint internal, GLuint image);

	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);
	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
	void Unbind() const;
	/*
	void setWrap(GLuint wrap_S, GLuint wrap_T);
	void setFilter(GLuint MinFilter, GLuint MaxFilter);
	void setFormat(GLuint InternalFormat, GLuint ExternalFormat);
	*/
	void End();
};

//#endif

