#include "SpriteRender.h"






SpriteRender::SpriteRender(Shader& textureshader, Shader& quadshader, Shader& textshader) {
	this->Textureshader = textureshader;
	this->Quadshader = quadshader;
	this->Textshader = textshader;
	this->initRenderData();
}

SpriteRender::~SpriteRender() {
	glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRender::initRenderData() {
	//CLOCKWISE
	GLfloat vertices[] = {
		//Vertex	//TexCoords
		0.0, 1.0,	0.0, 1.0,
		1.0, 0.0,	1.0, 0.0,
		0.0, 0.0,	0.0, 0.0,

		0.0, 1.0,	0.0, 1.0,
		1.0, 1.0,	1.0, 1.0,
		1.0, 0.0,	1.0, 0.0
	};
	//Buffer
	GLuint VBO;
	//Create VAO
	glGenVertexArrays(1, &this->quadVAO);
	//Bind VAO and get to work
	glBindVertexArray(this->quadVAO);

	//Fill Buffer
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//location, number, type, normalize, stride (in bytes), offset
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (GLvoid*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Initialize text shader buffer
	glGenVertexArrays(1, &this->wordVAO);
	glGenBuffers(1, &this->wordVBO);
	glBindVertexArray(this->wordVAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->wordVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


}

void SpriteRender::DrawSprite(Texture2D&  texture, glm::vec2 position, glm::vec2 size, glm::vec2 dir, glm::vec4 color) {
	
	this->Textureshader.Use();

	//glm return trans * og, so imagine these multiplications are done in reverse for classic matrix math
	glm::mat4 model = glm::mat4(1.0);
	//Move to position
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//Move quad back to where it was
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	/* I GOT IT, HELL YEAH BABY
	* The reason x and y are switched is because I give in coordinates of vector I want it rotated towards, not unit circle theta vector:
	* That means to get desired result, we need to transform (dir.x,dir.y)->(dir.y, -dir.x). Try some circle pairs yourself to see!
	*You could also see that transformation as rotate by dir then back 90 degrees, (because up = (1,0) but that = 90 degree turn)
	* The -dir.y (before switch) comes from the fact that screen coordinates mean down is positive y
	*/
	model *= glm::mat4(-dir.y,   dir.x, 0, 0,
					   -dir.x,  -dir.y, 0, 0,
					   0,       0,      1, 0,
					   0,       0,      0, 1);
	//move quad to center at origin (0,0)
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	//Scale model to size
	model = glm::scale(model, glm::vec3(size, 1.0f));
	//Fill uniforms
	this->Textureshader.SetMatrix4("model", model);
	this->Textureshader.SetVector4f("spriteColor", color);

	//Fill texture
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	//Draw at last
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	texture.Unbind();

	glBindVertexArray(0);
}
void SpriteRender::DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 size, glm::vec4 color) {

	this->Textureshader.Use();

	//glm return trans * og, so imagine these multiplications are done in reverse for classic matrix math
	glm::mat4 model = glm::mat4(1.0);
	//Move to position
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//Scale model to size
	model = glm::scale(model, glm::vec3(size, 1.0f));

	//Fill uniforms
	this->Textureshader.SetMatrix4("model", model);
	this->Textureshader.SetVector4f("spriteColor", color);

	//Fill texture
	glActiveTexture(GL_TEXTURE0);
	texture.Bind();

	//Draw at last
	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	texture.Unbind();

	glBindVertexArray(0);
}

void SpriteRender::DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec2 dir, glm::vec4 color) {

	this->Quadshader.Use();

	//glm does column major, so imagine these multiplications are done in reverse for classic matrix math
	glm::mat4 model = glm::mat4(1.0);
	//Move to position
	model = glm::translate(model, glm::vec3(position, 0.0f));
	//Move quad back to where it was
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	//See above rant on why not just normal rotate matrix
	model *= glm::mat4( -dir.y, dir.x, 0, 0,
						-dir.x,  -dir.y, 0, 0,
						0,		0,	   1, 0,
						0,      0,     0, 1);
	//move quad to center at origin (0,0)
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));
	//Scale model to size
	model = glm::scale(model, glm::vec3(size, 1.0f));
	//Fill uniforms
	this->Quadshader.SetMatrix4("model", model);
	this->Quadshader.SetVector4f("quadColor", color);


	glBindVertexArray(this->quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}


//TODO: Fix the include mess

//TO NOTE: Defines string at the bottom left instead of the top left like the rest. Can't fix. (would need max char height in string)
void SpriteRender::DrawString(std::string text, std::string fontName, glm::vec2 position, GLfloat scale, glm::vec3 color) {
	scale /= 10; //And here is where I makes scale completely arbitrary to font size. Losers. //TODO: Say it ain't so
	this->Textshader.Use();
	this->Textshader.SetVector3f("textColor", color);

	GLfloat x = position.x; //pen point moves forward

	glActiveTexture(GL_TEXTURE0); //Now we just need to bind the right texture to it and set the sampler2D uniform
	Resource_manager::GetFont(fontName).Bind();
	glBindVertexArray(this->wordVAO);


	std::string::const_iterator c;
	FontGlyph ch;
	
	GLuint width = Resource_manager::GetFont(fontName).Width; //Tex width/height
	GLuint height = Resource_manager::GetFont(fontName).Height;

	for (c = text.begin(); c != text.end(); c++) {
		
		 ch = Resource_manager::GetFont(fontName).getGlyph((unsigned char) *c);
		 

		 
		//Freetype = bottomleft origin

		GLfloat xpos = x + ch.Bearing.x * scale; //Move xbearing from origin on the x
		GLfloat ypos = position.y  + ch.Bearing.y * scale; // Move down from baseline so bottom rests at real bottom
		
		GLfloat w = ch.Size.x * width * scale; //Adjust size to scale
		GLfloat h = ch.Size.y * height * scale;

		// Update VBO for each character- this is why the shader was made dynamic... because we are updating the VBO every single character
		//CLOCKWISE
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   ch.Position.x,			   ch.Position.y + ch.Size.y}, //TopLeft- was 0,1
			{ xpos + w, ypos ,      ch.Position.x + ch.Size.x, ch.Position.y}, //BottomRight- was 1,0
			{ xpos,     ypos,       ch.Position.x,			   ch.Position.y}, //Bottomleft- was 0,0

			{ xpos,     ypos + h,   ch.Position.x,			   ch.Position.y + ch.Size.y}, //Topleft- was 0,1
			{ xpos + w, ypos + h,   ch.Position.x + ch.Size.x, ch.Position.y + ch.Size.y},  //Topright- was 1,1
			{ xpos + w, ypos,		ch.Position.x + ch.Size.x, ch.Position.y} //Bottomright- was 1,0
		}; 
		

		//Here's an idea! pass these changes along to the shader instead of doing this every time!
		glBindBuffer(GL_ARRAY_BUFFER, this->wordVBO); 
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //Update the values in the buffer according to new pos.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Now advance cursors for next glyph
		x += ch.Advance * scale;
	}
	glBindVertexArray(0);
	Resource_manager::GetFont(fontName).Unbind();
}




void SpriteRender::DrawString(std::string text, std::string fontName, glm::vec2 position, GLfloat scale, glm::vec3 color, glm::vec3 outerColor, TextMode mode) {

	scale /= 10; //And here is where I makes scale completely arbitrary to font size. Losers. //TODO: Say it ain't so
	this->Textshader.Use();
	this->Textshader.SetVector3f("textColor", color);
	this->Textshader.SetVector3f("outerColor", outerColor);
	this->Textshader.SetInteger("textMode", mode);

	GLfloat x = position.x; //pen point moves forward

	glActiveTexture(GL_TEXTURE0); //Now we just need to bind the right texture to it and set the sampler2D uniform
	Resource_manager::GetFont(fontName).Bind();
	glBindVertexArray(this->wordVAO);


	std::string::const_iterator c;
	FontGlyph ch;

	GLuint width = Resource_manager::GetFont(fontName).Width; //Tex width/height
	GLuint height = Resource_manager::GetFont(fontName).Height;

	for (c = text.begin(); c != text.end(); c++) {

		ch = Resource_manager::GetFont(fontName).getGlyph((unsigned char)* c);



		//Freetype = bottomleft origin

		GLfloat xpos = x + ch.Bearing.x * scale; //Move xbearing from origin on the x
		GLfloat ypos = position.y + ch.Bearing.y * scale; // Move down from baseline so bottom rests at real bottom

		GLfloat w = ch.Size.x * width * scale; //Adjust size to scale
		GLfloat h = ch.Size.y * height * scale;

		// Update VBO for each character- this is why the shader was made dynamic... because we are updating the VBO every single character
		//CLOCKWISE
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   ch.Position.x,			   ch.Position.y + ch.Size.y}, //TopLeft- was 0,1
			{ xpos + w, ypos ,      ch.Position.x + ch.Size.x, ch.Position.y}, //BottomRight- was 1,0
			{ xpos,     ypos,       ch.Position.x,			   ch.Position.y}, //Bottomleft- was 0,0

			{ xpos,     ypos + h,   ch.Position.x,			   ch.Position.y + ch.Size.y}, //Topleft- was 0,1
			{ xpos + w, ypos + h,   ch.Position.x + ch.Size.x, ch.Position.y + ch.Size.y},  //Topright- was 1,1
			{ xpos + w, ypos,		ch.Position.x + ch.Size.x, ch.Position.y} //Bottomright- was 1,0
		};


		//Here's an idea! pass these changes along to the shader instead of doing this every time!
		glBindBuffer(GL_ARRAY_BUFFER, this->wordVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); //Update the values in the buffer according to new pos.
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph
		x += ch.Advance * scale;
	}
	glBindVertexArray(0);
	Resource_manager::GetFont(fontName).Unbind();




}



