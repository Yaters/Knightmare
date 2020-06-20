#pragma once //One or the other. Not both
/*
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
*/

#include "Resource_manager.h"
#include <glm/glm.hpp>

class GameObject {
public:
	GameObject(glm::vec2 pos, glm::vec2 size, glm::vec2 dir);
	GameObject();
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 dir;
};


//#endif

