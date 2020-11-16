
#include "Game.h"


const glm::vec2 PLAYER_SIZE(60, 60);
GLfloat PLAYER_SPEED = 300.;

//Reference for future because I screwed this up like 4 times now and each time spent forever looking for the problem
//When you do if(condition) statement1; statement2; statement3; ONLY STATEMENT1 IS CONSIDERED FOR IF unless you put {}

Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), keys(), clicks(), mouseButtons(), Width(width), Height(height)
{
	soundEngine = irrklang::createIrrKlangDevice();
	Renderer = 0;
}

Game::~Game() {
	delete p1;
	delete enemy1;
	delete Renderer;
	soundEngine->drop();
}

void Game::Init() {
	char tmpBuffer[130];
	std::string dir = GetCurrentDir(tmpBuffer, 130);

	std::cout << "Directory: " << dir << "\n\n";

	std::cout << "****************************\n" << "Loading in Shaders (6)" << std::endl;

	Resource_manager::LoadShader((dir + "\\Shaders\\TextureVertex.txt").c_str(), (dir + "\\Shaders\\TextureFrag.txt").c_str(), nullptr, "TextureShader");
	Resource_manager::LoadShader((dir + "\\Shaders\\QuadVertex.txt").c_str(),    (dir + "\\Shaders\\QuadFrag.txt").c_str(),    nullptr, "QuadShader");
	Resource_manager::LoadShader((dir + "\\Shaders\\WordVertex.txt").c_str(), (dir + "\\Shaders\\WordFrag.txt").c_str(), nullptr, "TextShader");

	std::cout << "Finished loading shaders" << std::endl;

	//THis is cool, it changes the coordinate system to be x[0,Width], y[0,Height] with topleft 0,0
	glm::mat4 projection = glm::ortho(0.f, float(static_cast<GLfloat>(Width)),
		float(static_cast<GLfloat>(Height)), 0.f, -1.f, 1.f);

	//set Uniforms
	
	//OK this took forever to figure out black screen. Can't set uniforms unless shader is bound
	
	Resource_manager::GetShader("TextureShader").Use();
	Resource_manager::GetShader("TextureShader").SetInteger("image", 0);
	Resource_manager::GetShader("TextureShader").SetMatrix4("projection", projection);

	Resource_manager::GetShader("QuadShader").Use();
	Resource_manager::GetShader("QuadShader").SetMatrix4("projection", projection);

	Resource_manager::GetShader("TextShader").Use();
	Resource_manager::GetShader("TextShader").SetMatrix4("projection", projection);
	Resource_manager::GetShader("TextShader").SetInteger("text", 0);


	
	std::cout << "****************************\n" << "Loading in Textures (23)" << std::endl;

	//Load Textures
	Resource_manager::LoadTexture((dir + "\\Textures\\Sprite.png").c_str(), "Sprite");
	Resource_manager::LoadTexture((dir + "\\Textures\\InfoBox.png").c_str(), "InfoBox");
	Resource_manager::LoadTexture((dir + "\\Textures\\Button.png").c_str(), "Button");

	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\FanWarn.png").c_str(), "FanWarn");
	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\Fan.png").c_str(), "Fan");
	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\Bomb.png").c_str(), "Bomb");
	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\Shield.png").c_str(), "Shield");
	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\Throw.png").c_str(), "Throw");
	Resource_manager::LoadTexture((dir + "\\Textures\\Action\\Protect.png").c_str(), "Protect");

	Resource_manager::LoadTexture((dir + "\\Textures\\Screens\\Background2.png").c_str(), "BackGround");
	Resource_manager::LoadTexture((dir + "\\Textures\\Screens\\Title.png").c_str(), "TitleScreen");
	Resource_manager::LoadTexture((dir + "\\Textures\\Screens\\LoseScreen.png").c_str(), "LoseScreen");
	Resource_manager::LoadTexture((dir + "\\Textures\\Screens\\WinScreen.png").c_str(), "WinScreen");
	Resource_manager::LoadTexture((dir + "\\Textures\\Screens\\ActionSelect.png").c_str(), "SelectScreen");

	for (int i = 0; i < sizeof(Action::actionBoxes) / sizeof(std::string); i++) {
		Resource_manager::LoadTexture((dir + "\\Textures\\Boxes\\" + Action::actionBoxes[i] + ".png").c_str(), Action::actionBoxes[i]);
	}

	std::cout << "Finished loading textures" << std::endl;

	//haha the random var is just to remove the visual studio warning. Easier than writing the specific code thing at the top
	FontAtlas randVarDOntUse2158133_ = Resource_manager::LoadFont("Arial");

	//Make Renderer
	Shader sh1 = Resource_manager::GetShader("TextureShader");
	Shader sh2 = Resource_manager::GetShader("QuadShader");
	Shader sh3 = Resource_manager::GetShader("TextShader");

	
	//ANnnnnnd here is where i need to make my buttons
	this->menuButtons[0] = Button("Continue", glm::vec2(Width / 2, Height / 4), glm::vec2(Width/4, Height / 8), 12, TextMode::SHADOW);
	this->menuButtons[0].setTextColor(glm::vec3(0.540, 0.062, 0.109), glm::vec3(0.0f, 0.0f, 0.0f));
	this->menuButtons[0].onClick = [this]() {
		this->p1->color /= 0.4f;
		this->enemy1->color /= 0.4f;
		this->p1->color.a = 1.f;
		this->enemy1->color.a = 1.f;
		this->State = GAME_ACTIVE;
		this->music->setIsPaused(false);
	};
	this->menuButtons[1] = Button("Exit", glm::vec2(Width / 2, Height / 4), glm::vec2(Width / 4,5* Height / 8), 12, TextMode::SHADOW);
	this->menuButtons[1].setTextColor(glm::vec3(0.540, 0.062, 0.109), glm::vec3(0.0f, 0.0f, 0.0f));
	this->menuButtons[1].onClick = [this]() {
		this->State = GAME_EXIT;
	};
	this->titleButtons[0] = Button("Start Game", glm::vec2(Width / 2, Height / 4), glm::vec2(Width / 4, 5*Height / 8), 18, TextMode::GLOW);
	this->titleButtons[0].setTextColor(glm::vec3(0.540, 0.062, 0.109), glm::vec3(0.0f, 0.0f, 0.0f));
	this->titleButtons[0].onClick = [this]() {
		this->State = GAME_SELECT_ACTION;
	};
	this->selectButtons[0] = Button(">>", glm::vec2(Width / 2, Height*5 / 64), glm::vec2(Width  / 2, 43 * Height / 64), 6, TextMode::REGULAR);
	this->selectButtons[0].setTextColor(glm::vec3(0.0f, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f));
	this->selectButtons[0].onClick = [this]() {
		Action_List pActions[5];
		for (int i = 0; i < 5; i++) {
			pActions[i] = Action_List::Empty;
		}
		for (int i = 0; i < sizeof(actionSelect) / sizeof(ActionBox); i++) {
			int Index = actionSelect[i].getActionIndex();
			if (Index != -1) {
				pActions[Index] = actionSelect[i].getAction();
			}
		}
		p1->setMoves(pActions);

		this->State = GAME_ACTIVE;
		//File, loop :), start paused, track (return ISound)
		this->music = soundEngine->play2D("Music/BattleMono.mp3", true, false, true);
		this->music->setVolume(0.3);
	};
	this->selectButtons[1] = Button("<<", glm::vec2(Width / 2, Height*5 / 64), glm::vec2(0, 43 * Height / 64), 6, TextMode::REGULAR);
	this->selectButtons[1].setTextColor(glm::vec3(0.0f, 0.0, 0.0), glm::vec3(0.0f, 0.0f, 0.0f));
	this->selectButtons[1].onClick = [this]() {
		this->State = GAME_START;
		//Start music
		this->music = soundEngine->play2D("Music/TitleMono.mp3", true, false, true);
		this->music->setVolume(0.25);
	};

	this->endButtons[0] = Button("Return To Title", glm::vec2(Width / 2, Height / 8), glm::vec2(Width / 4, 5 * Height / 8), 12, TextMode::SHADOW);
	this->endButtons[0].setTextColor(glm::vec3(0.540, 0.062, 0.109), glm::vec3(0.0f, 0.0f, 0.0f));
	this->endButtons[0].onClick = [this]() {
		this->State = GAME_START;
		//Start music
		this->music = soundEngine->play2D("Music/TitleMono.mp3", true, false, true);
		this->music->setVolume(0.25);
		//Reset logic
		delete p1;
		delete enemy1;
		p1 = new Player(Resource_manager::GetTexture("Sprite"), PLAYER_SPEED, glm::vec2(Width / 2.0, Height - 100), glm::vec4(0.6, 0.6, 1, 1), PLAYER_SIZE, glm::vec2(0.0, -1.0));
		enemy1 = new Enemy(Resource_manager::GetTexture("Sprite"), (float)PLAYER_SPEED / 3.0f, glm::vec2(Width / 2.0, 100), glm::vec4(1, 0.6, 0.6, 1), PLAYER_SIZE, glm::vec2(0.0, 1.0));
	};

	//ExitButton
	this->endButtons[1] = Button("Exit Game", glm::vec2(Width / 2, Height / 8), glm::vec2(Width / 4, 6.5 * Height / 8), 12, TextMode::SHADOW);
	this->endButtons[1].setTextColor(glm::vec3(0.540, 0.062, 0.109), glm::vec3(0.0f, 0.0f, 0.0f));
	this->endButtons[1].onClick = [this]() {
		this->State = GAME_EXIT;
	};

	
	glm::vec2 actionBoxesSize = glm::vec2(200, 133);

	GLint boxPerRow = (Width - 100) / ((int) actionBoxesSize.x + 100); //boxes that can fit per row

	ActionBox::setBottomStats(1.5f, glm::vec2(100.f + actionBoxesSize.x, 0.f), glm::vec2(100.f, Height - 100.f - actionBoxesSize.y));
	for (int i = 0; i < sizeof(actionSelect) / sizeof(ActionBox); i++) {
		
		GLint row = i / boxPerRow;
		GLint col = i % boxPerRow;

		actionSelect[i] = ActionBox(Action::actionBoxes[i], actionBoxesSize, glm::vec2( (actionBoxesSize.x+100)*col + 100, (actionBoxesSize.y+100) * row + 100),(Action_List) i );
		actionSelect[i].initClick();
	}
	
	
	
	//Deleted in destructor
	p1 = new Player(Resource_manager::GetTexture("Sprite"), PLAYER_SPEED, glm::vec2(Width / 2.0, Height - 100), glm::vec4(0.6, 0.6, 1, 1), PLAYER_SIZE, glm::vec2(0.0, -1.0));
	//InfoBox enemyBox(Resource_manager::GetTexture("InfoBox"), 100., 100., 100., glm::vec2(5, 5), glm::vec2(350, 150));
	enemy1 = new Enemy(Resource_manager::GetTexture("Sprite"), (float) PLAYER_SPEED / 3.0f, glm::vec2(Width / 2.0, 100), glm::vec4(1, 0.6, 0.6, 1), PLAYER_SIZE, glm::vec2(0.0, 1.0));
	Renderer = new SpriteRender(sh1, sh2, sh3);	

	background.pos = glm::vec2(0, 0);
	background.size = glm::vec2(Width, Height);

	//Start music
	//Weird bug, in irrklang stereo audio causes a MASSIVE crunch sound
	//Almost blew out my speakers on that
	//Solution is to convert to mono, I used audacity
	this->music = soundEngine->play2D("Music/TitleMono.mp3", true, false, true);
	this->music->setVolume(0.25);
}

//ORDER GOES: INIT, INPUT, UPDATE, RENDER

void Game::Update(GLfloat dt) {
	if (State == GAME_ACTIVE) {
		//Update p1 dir
		glm::vec2 temp = CursorPos - p1->pos;
		//Always remember to normalize dir vector
		p1->SetDir(temp / glm::length(temp));

		ProcessInput(dt);

		//Update damage based on actions
		DamageUpdate(p1, enemy1, dt);

		DoCollisions(dt); //Contains players within circle
	} else {
		ProcessInput(dt);
	}
	
}

void Game::DoCollisions(GLfloat dt) {
	//Keeps characters in background
	Collider::CircleContainCircle(background, *p1);
	Collider::CircleContainCircle(background, *enemy1);

}

void Game::ProcessInput(GLfloat dt) {
	if (State == GAME_START) {
		//Check for clicks
		if (this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			for (int i = 0; i < sizeof(titleButtons) / sizeof(Button); i++) {
				if (titleButtons[i].isSelected) {
					titleButtons[i].onClick();
					this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = GL_FALSE;
					//std::cout << "\nDoing on click for the " << i << " button\n";
				}
			}
		}
		if (this->clicks[GLFW_KEY_ESCAPE]) {
			State = GAME_EXIT;
		}
	} else if (State == GAME_SELECT_ACTION) {

		//Check for clicks
		if (this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			for (int i = 0; i < sizeof(actionSelect) / sizeof(ActionBox); i++) {
				
				if (actionSelect[i].isSelected) {
					actionSelect[i].onClick();
					this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = GL_FALSE;
					//std::cout << "\nDoing on click for the " << i << " button\n";
				}
			}
			for (int i = 0; i < sizeof(selectButtons) / sizeof(Button); i++) {
				if (selectButtons[i].isSelected) {
					selectButtons[i].onClick();
					this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = GL_FALSE;
					//std::cout << "\nDoing on click for the " << i << " button\n";
				}
			}
		}
		if (this->clicks[GLFW_KEY_ESCAPE]) {
			State = GAME_EXIT;
		}
	} else if (State == GAME_ACTIVE) {

		if (this->clicks[GLFW_KEY_ESCAPE]) {
			this->clicks[GLFW_KEY_ESCAPE] = GL_FALSE;
			this->State = GAME_MENU;
			this->p1->color *= 0.4f;
			this->enemy1->color *= 0.4f;
			this->p1->color.a = 1.f;
			this->enemy1->color.a = 1.f;
			this->music->setIsPaused(true);
		}

		//Muahahahahaha I tricked you all by putting the input processing inside the player class! Huehuehuehuehuehuehue
		//Honestly it just sort of hides the mess of if else click weird stuff that was input. yeah...

		//Update position and actions based on input
		p1->Update(Renderer, enemy1, dt, keys, clicks);
		Collider::CircleExcludeCircle(*enemy1, *p1);

		//Update enemy Pos
		enemy1->Update(Renderer, p1, dt);
		Collider::CircleExcludeCircle(*p1, *enemy1);
	}  else if (State == GAME_MENU) {
		//Check for clicks
		if (this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			for (int i = 0; i < sizeof(menuButtons) / sizeof(Button); i++) {
				if (menuButtons[i].isSelected) {
					menuButtons[i].onClick();
					this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = GL_FALSE;
				}
			}
		}
		if (this->clicks[GLFW_KEY_ESCAPE]) {
			this->clicks[GLFW_KEY_ESCAPE] = GL_FALSE; 
			menuButtons[0].onClick(); //Resumes game
			this->music->setIsPaused(false);
		}

	} else {
		//WIN or LOSE screen. Similar to menu in functionality
		if (this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
			for (int i = 0; i < sizeof(endButtons) / sizeof(Button); i++) {
				if (endButtons[i].isSelected) {
					endButtons[i].onClick();
					this->mouseButtons[GLFW_MOUSE_BUTTON_LEFT] = GL_FALSE;
				}
			}
		}
	}
}




void Game::Render(GLfloat dt) {

	if (State == GAME_START) {
		Renderer->DrawSprite(Resource_manager::GetTexture("TitleScreen"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(1));
		for (int i = 0; i < sizeof(titleButtons) / sizeof(Button); i++) {
			titleButtons[i].Draw(this->CursorPos, Renderer);
		}

	} else if (State == GAME_SELECT_ACTION) {
		
		Renderer->DrawSprite(Resource_manager::GetTexture("SelectScreen"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(1));
		for (int i = 0; i < sizeof(actionSelect) / sizeof(ActionBox); i++) {
			actionSelect[i].Draw(this->CursorPos, Renderer);
		}
		for (int i = 0; i < sizeof(selectButtons) / sizeof(Button); i++) {
			selectButtons[i].Draw(this->CursorPos, Renderer);
		}


	} else if (State == GAME_ACTIVE) {
		//Texture, Position, Size, Direction (+y=down), Color
		Renderer->DrawSprite(Resource_manager::GetTexture("BackGround"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(1));
		//Drawing updates player's position and Actions (MP/HP)
		p1->Draw(Renderer, dt);
		enemy1->Draw(Renderer, dt);
		//NOTE: STRING POS DEFINES BOTTOM LEFT TODO: Fix that
		
		//Renderer->DrawString("quetzolcotal 2.0 man! potato. cabbage. quilts!", "Arial", glm::vec2(200, 1000), 20.0, glm::vec3(0.7, 0.3, 0.3));
		
		//Renderer->DrawString("Hello World 2.0! potato. cabbage. quilts!", glm::vec2(25, 50), 1.0f, glm::vec3(0.3, 0.7, 0.9));
		
	} else if (State == GAME_MENU) {
		//Texture, Position, Size, Rotate, Color
		Renderer->DrawSprite(Resource_manager::GetTexture("BackGround"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(0.1, 0.1, 0.1, 1));
		//Drawing updates player's position and Actions (MP/HP)
		
		p1->Draw(Renderer, 0);
		enemy1->Draw(Renderer, 0);
		
		for (int i = 0; i < sizeof(menuButtons) / sizeof(Button); i++) {
			menuButtons[i].Draw(this->CursorPos, Renderer);
		}
		
		//buttons[0].Draw(this->CursorPos, Renderer);
		//buttons[1].Draw(this->CursorPos, Renderer);
	} else if (State == GAME_WIN) {
		Renderer->DrawSprite(Resource_manager::GetTexture("WinScreen"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(1.0, 1.0, 1.0, 1));
		for (int i = 0; i < sizeof(endButtons) / sizeof(Button); i++) {
			endButtons[i].Draw(this->CursorPos, Renderer);
		}
	} else {
		Renderer->DrawSprite(Resource_manager::GetTexture("LoseScreen"), background.pos, background.size, glm::vec2(0, -1), glm::vec4(1.0, 1.0, 1.0, 1));
		for (int i = 0; i < sizeof(endButtons) / sizeof(Button); i++) {
			endButtons[i].Draw(this->CursorPos, Renderer);
		}
	}
	
}



//Ok, Idk what to do with the collider class. It really only needs one instance for every player and enemy, but singleton is 'bad design'
//Though I did use a singleton for Resource Manager, but that's more... pervasive in where it's needed
//Function that changes HP of both players based on actions. Each switch case is basically copy of the other but for different chars
void Game::DamageUpdate(Player* P, Enemy* E, GLfloat dt) {

	Action::DamageUpdate(P, E, dt);

	if (P->HP < 0) {
		State = GAME_LOSE;
		soundEngine->stopAllSounds();
	}
	else if (E->HP < 0) {
		State = GAME_WIN;
		soundEngine->stopAllSounds();
	}

}

//TODO: buttons, pause, start screen, etc.