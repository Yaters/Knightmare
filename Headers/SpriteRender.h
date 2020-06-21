#pragma once //One or the other. Not both
/*
#ifndef SPRITERENDER_H
#define SPRITERENDER_H
*/

#include "Shader.h"
#include "Texture.h"
#include "Resource_manager.h"
#include <iostream>
#include <string>
#include <map>

enum TextMode {
	REGULAR,
	OUTLINE,
	GLOW,
	SHADOW
};


class SpriteRender {
	public:
		SpriteRender(Shader &textureshader, Shader &quadshader, Shader &textshader);
		~SpriteRender();

		

		void DrawSprite(Texture2D& texture, glm::vec2 position = glm::vec2(0, 0),
						glm::vec2 size = glm::vec2(10, 10), 
						glm::vec2 direction = glm::vec2(0, -1),
						glm::vec4 color = glm::vec4(1.0f));
		void DrawSprite(Texture2D& texture, glm::vec2 position = glm::vec2(0, 0),
						glm::vec2 size = glm::vec2(10, 10),
						glm::vec4 color = glm::vec4(1.0f));

		

		void DrawQuad(glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
			glm::vec2 direction = glm::vec2(0, -1), glm::vec4 color = glm::vec4(1.0f));

		void DrawString(std::string text, std::string fontName, glm::vec2 position, GLfloat scale, glm::vec3 color);
		void DrawString(std::string text, std::string fontName, glm::vec2 position, GLfloat scale, glm::vec3 color, glm::vec3 outerColor, TextMode mode);
	private:
		Shader Textureshader;
		Shader Quadshader;
		Shader Textshader;

		GLuint quadVAO;

		GLuint wordVAO;
		GLuint wordVBO; //Needed because it's overriden to draw characters

		void initRenderData();
};

//#endif

