#pragma once //One or the other. Not both
/*
#ifndef INFOBOX_H
#define INFOBOX_H
*/


#include "Texture.h"
#include "SpriteRender.h"
#include "GameObject.h"
#include <string>

class InfoBox : public GameObject {
private:
	GLfloat MaxHP;
	GLfloat MaxSP;
	GLfloat MaxMP;


public:
	InfoBox();
	InfoBox(Texture2D sprite, GLfloat MaxHP = 100.0f, GLfloat MaxSP = 100.0f, GLfloat MaxMP = 100.0f, glm::vec2 pos = glm::vec2(1245, 1045), glm::vec2 size = glm::vec2(350, 150));
	Texture2D sprite;

	std::string Name;
	void Draw(SpriteRender* renderer, GLfloat HP, GLfloat SP, GLfloat MP);
	void DrawEnemy(SpriteRender* renderer, GLfloat HP);
	//Try this, make an infoBox draw func that only draws HP bar over head. That removes MaxHP from Enemy.h. 
};

//#endif