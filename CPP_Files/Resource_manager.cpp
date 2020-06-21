#include "Resource_manager.h"



//(almost) ALL OF THESE ARE FRICKING STBImage!!!!!
#pragma warning (disable: 26451 6308 6011 6262 6387 28182)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// Instantiate static variables
std::map<std::string, Texture2D>    Resource_manager::Textures;
std::map<std::string, Shader>       Resource_manager::Shaders;
std::map<std::string, FontAtlas>	Resource_manager::Fonts;


Shader Resource_manager::LoadShader(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader Resource_manager::GetShader(std::string name) {
	return Shaders[name];
}


FontAtlas Resource_manager::LoadFont(std::string fontName) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//Create Atlas Texture reference
	FontAtlas atlas;
	

	std::cout << "****************************\n" << "Loading in Font: " << fontName << std::endl;
	// Load image
	int width, height, nrChannels;
	std::string path = "C:\\Users\\yajal\\Pictures\\Fonts\\" + fontName + "\\" + fontName + "font.png";
	unsigned char* image = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
	if (nrChannels != 1) {
		std::cout << std::endl << "Error, font png should only have one channel, not " << nrChannels << std::endl << std::endl;
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
		return atlas;
	}
	

	// Now generate texture
	atlas.Generate(width, height, image);
	// And finally free image data
	stbi_image_free(image);
	

	//Fills up the atlas glyph map using lua
	atlas.loadFont(fontName);
	Fonts[fontName] = atlas;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	std::cout << "Font "<< fontName << " loaded" << std::endl;
	return atlas;
}

FontAtlas& Resource_manager::GetFont(std::string name) {
	return Fonts[name];
}


Texture2D Resource_manager::LoadTexture(const GLchar* file, std::string name) {
	Textures[name] = loadTextureFromFile(file);
	return Textures[name];
}

Texture2D& Resource_manager::GetTexture(std::string name) {
	
	return Textures[name];
}

void Resource_manager::Clear() {
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		iter.second.End();
	// (Properly) delete all textures
	for (auto iter : Textures)
		iter.second.End();
	// (Properly) delete all fonts
	for (auto iter : Fonts)
		iter.second.End();
}

Shader Resource_manager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {

	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	
	try {

		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);

		vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		std::stringstream vShaderStream, fShaderStream;
		
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		
		// Convert stream into string
		vertexShaderFile.close();
		fragmentShaderFile.close();;
		
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		
		
		
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr) {
			std::cout << "Loading geometry shader" << std::endl;
			std::ifstream geometryShaderFile(gShaderFile);
			geometryShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
		char errmsg[512];
		strerror_s(errmsg, 512, errno);
		std::cout << "Problem was: " << errmsg << std::endl;
		
	}
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();
	const GLchar* gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}



Texture2D Resource_manager::loadTextureFromFile(const GLchar* file) {
	// Create Texture object
	Texture2D texture;

	// Load image
	int width, height, nrChannels;
	unsigned char* image = stbi_load(file, &width, &height, &nrChannels, 0);
	switch (nrChannels) {
	case(4): 
		texture.setFormat(GL_RGBA, GL_RGBA);
		break;
	case(3):
		texture.setFormat(GL_RGB, GL_RGB);
		break;
	case(2):
		texture.setFormat(GL_RG, GL_RG);
		break;
	case(1):
		texture.setFormat(GL_RED, GL_RED);
		break;
	}

	//unsigned char* image = SOIL_load_image(file, &width, &height, 0, alpha ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	//SOIL_free_image_data(image);
	stbi_image_free(image);
	return texture;
}

//Will be bad if something is set against default, but I don't feel like experimenting with push/pop
#pragma warning (default : 26451 6308 6011 6262 6387 28182)