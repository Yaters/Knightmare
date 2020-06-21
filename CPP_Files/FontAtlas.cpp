#include "FontAtlas.h"
//Arithmetic overflow warning on the size of images (seriously, 4 bytes is enough)
//#pragma warning (disable : 26451)
//size_t to int warning on, again, size of image dat vector
//#pragma warning (disable : 4267)


/*

Copyright (c) 2010, Ben Wilhelm

All rights reserved.



Redistribution and use in source and binary forms, with or without

modification, are permitted provided that the following conditions are met:

	* Redistributions of source code must retain the above copyright

	  notice, this list of conditions and the following disclaimer.

	* Redistributions in binary form must reproduce the above copyright

	  notice, this list of conditions and the following disclaimer in the

	  documentation and/or other materials provided with the distribution.

	* Neither the name of the copyright holders' companies nor the

	  names of its contributors may be used to endorse or promote products

	  derived from this software without specific prior written permission.



THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND

ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED

WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE

DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY

DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES

(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;

LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND

ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT

(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS

SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

I modified the algorithm, but I'm pretty thankful for it existing. So yeah. Shout out to that dude^
*/



FontAtlas::FontAtlas() {
	setParameters(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER, GL_LINEAR, GL_LINEAR);
	setFormat(GL_RED, GL_RED);
	ID = 0;
}

void FontAtlas::loadFont(std::string fontName) {
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	std::string path = "C:\\Users\\yajal\\Pictures\\Fonts\\" + fontName + "\\" + fontName + "font.lua";
	
	if (luaL_loadfile(L, path.c_str()) || lua_pcall(L, 0, 0, 0)) {
		//Might want to do something with L for more info. Up to future you
		//Error message was pushed to top of stack	
		std::cout << "Cannot load Lua Font: " << lua_tostring(L, -1) << std::endl;
		lua_close(L);
		return;
	}

	//Each call pushes the variable onto the lua stack
	/*
	lua_getglobal(L, "padding");
	lua_getglobal(L, "height");
	lua_getglobal(L, "ascend");
	lua_getglobal(L, "distslope");
	*/

	//Might want to run checks on any of these numbers someday
	
	//Table of char tables, each char table->sx(int), sy(int), ex(int), ey(int), ox(float), oy(float), w(float)
	//o=negative, w = positive

	//lua_next will pop a key, then return key-value from table at index. that's why we push a nil.
	//I listen to my API :)
	lua_getglobal(L, "characters");
	lua_pushnil(L); // first key
	//int index = lua_gettop(L);

	//-1 = top of stock->...-something
	//1 = bottom of stock->...something

	//Returns 0 once nothing is left. Pops key and pushes the next pair from table at index
	//key = -2, value = -1 = table in this case
	while (lua_next(L, -2)) {

		GLchar ID = (unsigned char) lua_tointeger(L, -2); //get key

		//Start going through table values- characters = table of tables

		//Get the table at the top of the stack and push its sx/ex
		lua_getfield(L, -1, "sx");
		lua_getfield(L, -2, "ex");//rip: table is below sx now (so -2)
		GLfloat posX = lua_tonumber(L, -2); //xOrigin
		GLfloat sizeX = lua_tonumber(L, -1) - lua_tonumber(L, -2); //ex-sx = sizeX
		lua_pop(L, 2); //Pop the sx/ex from top

		lua_getfield(L, -1, "sy");
		lua_getfield(L, -2, "ey");
		GLfloat posY = lua_tonumber(L, -2); //yOrigin. 
		GLfloat sizeY =  lua_tonumber(L, -1) - lua_tonumber(L, -2); //ey-sy = sizeX
		lua_pop(L, 2); //Pop the sy/ey from top

		lua_getfield(L, -1, "ox");
		lua_getfield(L, -2, "oy");
		glm::vec2 bearing = glm::vec2(lua_tonumber(L, -2), lua_tonumber(L, -1));
		lua_pop(L, 2); //Pop the ox/oy from top

		lua_getfield(L, -1, "w");
		GLfloat advance = lua_tonumber(L, -1);
		lua_pop(L, 1); //Pop the w from the top

		//I removed ID (Covered by map
		FontGlyph glyph = {
			glm::vec2(sizeX / static_cast<GLfloat>(this->Width),sizeY / static_cast<GLfloat>(this->Height)), 
			bearing,
			glm::vec2(posX / static_cast<GLfloat>(this->Width), posY / static_cast<GLfloat>(this->Height)),
			advance
		};

		this->Glyphs.insert(std::pair<GLchar, FontGlyph>(ID, glyph) );
		lua_pop(L, 1); // stack restore (pop the table value. lua_next will pop key itself)
	}

	lua_close(L);
}

FontGlyph FontAtlas::getGlyph(GLchar ID) {
	return Glyphs[ID];
}



//#pragma warning (default : 26451)
//#pragma warning (default : 4267)