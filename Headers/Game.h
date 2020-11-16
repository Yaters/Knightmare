#pragma once //One or the other. Not both
/*
#ifndef GAME_H
#define GAME_H
*/
#include <glad/glad.h>

#include "Enemy.h"
#include "SpriteRender.h"
#include "Resource_manager.h"
#include "Button.h"
#include "ActionBox.h"
#include <irrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")
//^ Had to look that up, tells the linker wher to look for stuff, can do in project properties too


#include <direct.h>
#define GetCurrentDir _getcwd
#include <string.h>

enum GameState {
	GAME_START,
	GAME_SELECT_ACTION,
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSE,
	GAME_EXIT
};



// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.


class Game {
public:
	// Game state
	GameState              State;

	//User input variables
	GLboolean              keys[1024];
	GLboolean              clicks[1024];
	GLboolean              mouseButtons[8];

	Button				   menuButtons[10]; //Buttons shown on menu screen
	Button				   titleButtons[3]; //Buttons shown on title screen
	Button				   selectButtons[2]; //Action Boxes shown on select screen
	Button				   endButtons[2];	//Buttons shown on win/lose screen
	ActionBox			   actionSelect[9]; //Action Boxes shown on select screen
	
	GLuint                 Width, Height;

	glm::vec2			   CursorPos = glm::vec2(0, 0);

	Player* p1 = NULL;
	Enemy* enemy1 = NULL;

	//Drawer
	SpriteRender* Renderer;

	//Sound maker
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISound* music;

	GameObject background;

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();

	// Initialize game state (load all shaders/textures/levels)
	void Init();

	// GameLoop
	void ProcessInput(GLfloat dt);
	void DoCollisions(GLfloat dt);
	void Update(GLfloat dt);
	void Render(GLfloat dt);

	void DamageUpdate(Player* player, Enemy* enemy, GLfloat dt); //This used to be in player, but seems better here

	
};

//#endif

