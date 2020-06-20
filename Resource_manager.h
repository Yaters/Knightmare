#pragma once //One or the other. Not both
/*
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
*/

#include <map>
#include <string>

#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>

#include "Texture.h"
#include "Shader.h"

#include "FontAtlas.h"




class Resource_manager {
public:
	
	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader   LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name);
	// Retrieves a stored sader
	static Shader   GetShader(std::string name);
	// Loads (and generates) a texture from file
	static Texture2D LoadTexture(const GLchar* file, std::string name);
	// Retrieves a stored texture
	static Texture2D& GetTexture(std::string name);
	//Loads (and generates) a bitmap texture based on the font in fontPath
	static FontAtlas LoadFont(std::string fontName);
	// Retrieves a stored font
	static FontAtlas& GetFont(std::string name);
	// Properly de-allocates all loaded resources
	static void Clear();


private:
	// Resource storage
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture2D> Textures;
	static std::map<std::string, FontAtlas> Fonts;
	//static std::map<GLchar, Char> Characters;

	// Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	Resource_manager() { }
	// Loads and generates a shader from file
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	// Loads a single texture from file
	//Used to include alpha boolean
	static Texture2D loadTextureFromFile(const GLchar* file);
};

//#endif


