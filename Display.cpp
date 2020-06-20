
#include "Game.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

Game Knightmare(SCR_WIDTH, SCR_HEIGHT);



int main(int argc, char* argv[]) {
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Knightmare", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	glfwSetCursor(window, cursor);

	// glad: load all OpenGL function pointers 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glCullFace(GL_CW);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	//Load info for shaders
	Knightmare.Init();
	glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

	GLfloat deltaTime = 0.0, lastTime = 0.0;

	Knightmare.State = GAME_START;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		if (Knightmare.State == GAME_EXIT) glfwSetWindowShouldClose(window, GL_TRUE);
		
		GLfloat currentFrame = (float) glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;

		//Input/Update
		glfwPollEvents();
		//Knightmare.ProcessInput(deltaTime);
		Knightmare.Update(deltaTime);
		glClear(GL_COLOR_BUFFER_BIT);
		//render
		Knightmare.Render(deltaTime);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		//V-Sync (mess around to see its effects!) Comment out when checking performance
		//glfwSwapInterval(1);
		//wglSwapIntervalEXT(1);
		glfwSwapBuffers(window);
		
	}
	//Delete glStuff
	//glDeleteBuffers(NumBuffers, Buffers);
	//glDeleteVertexArrays(NumVAOs, VAOs);
	//shader.end();
	
	Resource_manager::Clear();
	glfwDestroyCursor(cursor);
	glfwDestroyWindow(window);
	glfwTerminate(); // glfw: terminate, clearing all previously allocated GLFW resources.
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		//glfwSetWindowShouldClose(window, GL_TRUE);
	
	if (key >= 0 && key < 1024) {

		if (action == GLFW_PRESS) {
			//If it goes from false to true it registers as a click. Clearing once used is left to game process input
			if(!Knightmare.keys[key]) Knightmare.clicks[key] = GL_TRUE;

			Knightmare.keys[key] = GL_TRUE;
		}	
		else if (action == GLFW_RELEASE)
			Knightmare.keys[key] = GL_FALSE;
	
	}	

	
	
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	Knightmare.CursorPos = glm::vec2(xpos, ypos);	
}

// glfw: whenever the window size changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	// note that width and height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	
	if (button >= 0 && button < 8) {

		if (action == GLFW_PRESS) {
			//If it goes from false to true it registers as a click. Clearing once used is left to game process input
			if (!Knightmare.mouseButtons[button]) Knightmare.mouseButtons[button] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE)
			Knightmare.mouseButtons[button] = GL_FALSE;

	}
}