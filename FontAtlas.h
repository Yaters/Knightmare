#pragma once
#include "Texture.h"


#include <glm/vec2.hpp>

#include <map>
#include <string>


#include <cmath>
#include <cassert>
#include <iostream>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}




//Important Struct that encapsulates all of the data to render a character from the texture atlas
struct FontGlyph {
	glm::vec2 Size;
	glm::vec2 Bearing;
	glm::vec2 Position;
	GLfloat Advance;
};

//A Texture Atlas is an Atlas of font glyphs here
//The constructor takes the name of the font, and from it loads the png as a texture and a map of characters for the font
//It can create an unpacked texture map dynamically, though this isn't recommended
class FontAtlas : public Texture2D {
public:
	//Note, before using any font, it must have been loaded into the correct folder using FontAtlasMaker.exe
	//TODO: Make this copy the Texture2D default...ish
	FontAtlas();
	
	
	FontGlyph getGlyph(GLchar ID);
	void loadFont(std::string fontName);

private:
	
	std::map<GLchar, FontGlyph> Glyphs;
	//Might want to make a destructor
	//Also enforce param types!
};

