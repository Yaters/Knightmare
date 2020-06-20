#include "Texture.h"


Texture2D::Texture2D()
	: Internal_Format(GL_RGBA), Image_Format(GL_RGBA),
	Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR), ID(0)
{
	
}


void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data) {

	this->Width = width;
	this->Height = height;

	glGenTextures(1, &this->ID);

	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::setParameters(GLuint wrapS, GLuint wrapT, GLuint minFilter, GLuint maxFilter) {
	this->Wrap_S = wrapS;
	this->Wrap_T = wrapT;
	this->Filter_Min = minFilter;
	this->Filter_Max = maxFilter;
}
void Texture2D::setFormat(GLuint internal, GLuint image) {
	this->Internal_Format = internal;
	this->Image_Format = image;
}

void Texture2D::Bind() const {
	glBindTexture(GL_TEXTURE_2D, this->ID);
}
void Texture2D::Unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::End() {
	glDeleteTextures(1, &this->ID);
}