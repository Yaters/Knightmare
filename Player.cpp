#include "Player.h"

Player::Player(Texture2D PlaySprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir): 
	Character(PlaySprite, Speed, Pos, Color, Size, Dir) {
	
	info = InfoBox(Resource_manager::GetTexture("InfoBox"));
	initBoxPlacement(1600, 1200);
	action = new Action();
}
Player::Player(Texture2D PlaySprite, GLfloat Speed, glm::vec2 Pos, glm::vec4 Color, glm::vec2 Size, glm::vec2 Dir, InfoBox Info) :
	Character(PlaySprite, Speed, Pos, Color, Size, Dir), info(Info) {
	//Yaaaaaaa aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa... I'm tired. WOulD thIs MakE Me mOnEY In A cOMpanY?
	//^ Me copy pasting constructors for something somewhere ^
	initBoxPlacement(1600, 1200);
}




//Might someday need to be a enemy* array with a list of all the enemies in the game

//AAAAAAAAAAAAAAAAAAA This code is horrible, and not updateable, and spaghetti ish, and I'm still learning OK??????


void Player::Update(SpriteRender* renderer, Character* enemy, GLfloat dt, GLboolean(&keys)[1024], GLboolean(&clicks)[1024]) {
	UpdateVel(keys, clicks);
	//Update player position
	pos += velocity * speed * dt; //Collision handled by Game.cpp
}

//Draws player+info box based on current state
void Player::Draw(SpriteRender* renderer, GLfloat dt) {

	//Problem, different actions should be over or under player(like fan should be under, shield should be over
	//Might be better to make everything over, even with many players. Maybe in game draw all player stuff then do Action Update?
	info.Draw(renderer, HP, SP, MP);
	renderer->DrawSprite(sprite, pos, size, dir, color);

	for (int i = 0; i < sizeof(moves) / sizeof(Action_List); i++) {
		//If this isnt an empty list
		if (moves[i] != Action_List::Empty) {
			renderer->DrawSprite(Resource_manager::GetTexture(Action::actionBoxes[moves[i]]), 
								 boxesPos[i], boxesSize, 
								 glm::vec4(1.f, 1.f, 1.f, 1.f) * color.b);
		}
		
	}

	//Action logic
	if (this->SP <= 100) this->SP += 3 * dt;
	if (this->MP <= 100) this->MP += 3 * dt;
	this->action->Draw(this, renderer, dt); //Can't put this in Update or else it draws under
}



void Player::initBoxPlacement(GLint WIDTH, GLint HEIGHT) {
	GLfloat actionRatio = 2.f / 3.f; //Ratio of width to height on an actionBox

	this->boxesSize = glm::vec2(WIDTH / 12.f, WIDTH * actionRatio / 12.f);

	boxesPos[0] = glm::vec2(0,			 HEIGHT - boxesSize.y * 3); //Row 1
	boxesPos[1] = glm::vec2(boxesSize.x, HEIGHT - boxesSize.y * 3);

	boxesPos[2] = glm::vec2(0,			 HEIGHT - boxesSize.y * 2); //Row 2
	boxesPos[3] = glm::vec2(boxesSize.x, HEIGHT - boxesSize.y * 2);

	boxesPos[4] = glm::vec2(boxesSize.x / 2.f, HEIGHT - boxesSize.y);

}







/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* YOU AREN'T MISSING MUCH BEYOND THIS POINT,
* JUST VELOCITY UPDATE WITH INPUT
* TURN BACK NOW
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */











//Ok lemme explain. This makes it so conflicting directions dont cause a stop. Last key clicked becomes the input
//Also it processes the clicks, making them false for next glfwPollEvents loop

void Player::UpdateVel(GLboolean (&keys) [1024], GLboolean(&clicks)[1024]) {
	//TODO: Work out this jumble
	if (clicks[GLFW_KEY_W]) {
		//WHY IS IT NOT WORKING FOR ONLY W/D LIKE WTF MAN WTF WTF WTF WTF
		//Update: It's a flipping keyboard problem I think
		keys[GLFW_KEY_S] = GL_FALSE;
		clicks[GLFW_KEY_W] = GL_FALSE;
	}
	if (clicks[GLFW_KEY_S]) {
		keys[GLFW_KEY_W] = GL_FALSE;
		clicks[GLFW_KEY_S] = GL_FALSE;
	}
	if (clicks[GLFW_KEY_A]) {
		keys[GLFW_KEY_D] = GL_FALSE;
		clicks[GLFW_KEY_A] = GL_FALSE;
	}
	if (clicks[GLFW_KEY_D]) {
		keys[GLFW_KEY_A] = GL_FALSE;
		clicks[GLFW_KEY_D] = GL_FALSE;
	}


	//Update player pos (else if is for the 0.0000001s efficiency update... it matters to me alright!)
	//Maybe someday in the far future normalize velocity, so that going diagonal isn't faster.
	if (keys[GLFW_KEY_W]) velocity.y = -1;
	else if (keys[GLFW_KEY_S]) velocity.y = 1;
	else velocity.y = 0;

	//A/D and W/S are processed seperately
	if (keys[GLFW_KEY_A]) velocity.x = -1;
	else if (keys[GLFW_KEY_D]) velocity.x = 1;
	else velocity.x = 0;



	//Cuts off action if you start a new one
	//See if a key is clicked
	for (int i = 1; i <= 5; i++) {
		if (clicks[i + 48] == GL_TRUE) { //48 is key_0, I checked
			//std::cout << std::endl << "Action Done" << std::endl;

			if (moves[i - 1] != Action_List::Empty) {
				delete action;
				action = new Action(pos, dir, moves[i - 1], size); //Set key pressed to be action
			}

			clicks[i + 48] = GL_FALSE; 
		}
	}

}


void Player::setMoves(Action_List moves[5]) {
	//Basic setter, for loop isnt needed but that doesnt really matter
	for (int i = 0; i < 5; i++) {
		this->moves[i] = moves[i];
	}
}





//TO TRY: Particles, make these actions... sparkle! Like fan, I could totally do a wave of fire
