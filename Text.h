#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "glm\glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include "shader.h"
#include "stb_image.h"
#include <math.h>

struct Character {
public:
	GLuint textureID;
public:
	int id, x, y, width, height, xoffset, yoffset, xadvance;
	unsigned char* pixels;
	Character(int id, int x, int y, int width, int height, int xoffset, int yoffset, int xadvance) {
		this->id = id;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
		this->xoffset = xoffset;
		this->yoffset = yoffset;
		this->xadvance = xadvance;

		pixels = new unsigned char[width * height * 4];
	}

	void initGL() {
		glGenTextures(1, &textureID);
		GLenum format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Character::loadPixels(unsigned char *&data, int png_width, int png_height, int nrComponents) {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				pixels[i*width*nrComponents + j*nrComponents + 0] = data[(y + i)*png_width*nrComponents + x*nrComponents + j*nrComponents + 0];
				pixels[i*width*nrComponents + j*nrComponents + 1] = data[(y + i)*png_width*nrComponents + x*nrComponents + j*nrComponents + 1];
				pixels[i*width*nrComponents + j*nrComponents + 2] = data[(y + i)*png_width*nrComponents + x*nrComponents + j*nrComponents + 2];
				pixels[i*width*nrComponents + j*nrComponents + 3] = data[(y + i)*png_width*nrComponents + x*nrComponents + j*nrComponents + 3];
			}
		}
	}

	void Character::clean() {
		delete pixels;
		glDeleteTextures(1, &textureID);
	}
};



class Font {
public:
	Shader* shader;
private:
	unsigned int VBO, VAO, EBO;
public:
	std::map<int, Character> chars;
public:
	unsigned char* data;
	int png_width, png_height, nrComponents;
	float lineHeight, base, scaleW, scaleH;
public:
	Font() {

	}

	Font(std::string path) {
		loadPng(path);
		loadFnt(path);
		loadPixels(data, png_width, png_height, nrComponents);
		initGL();
	}

	void Font::loadFnt(std::string path) {
		std::ifstream file("font/" + path + ".fnt");
		if (file.is_open()) {
			std::string word;
			while (file >> word) {
				if (word == "common") {
					file >> word;
					lineHeight = std::stof(word.substr(word.find_first_of("=") + 1));
					file >> word;
					base = std::stof(word.substr(word.find_first_of("=") + 1));
					file >> word;
					scaleW = std::stof(word.substr(word.find_first_of("=") + 1));
					file >> word;
					scaleH = std::stof(word.substr(word.find_first_of("=") + 1));
				}
				else if (word == "char") {
					file >> word;
					int id = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int x = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int y = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int width = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int height = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int xoffset = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int yoffset = std::stoi(word.substr(word.find_first_of("=") + 1));
					file >> word;
					int xadvance = std::stoi(word.substr(word.find_first_of("=") + 1));
					chars.insert(std::pair<int, Character>(id, Character(id, x, y, width, height, xoffset, yoffset, xadvance)));
				}
			}
		}
		else std::cout << path << " - could not be opened" << std::endl;
		file.close();
	}

	void loadPng(std::string path) {
		//stbi_set_flip_vertically_on_load(true);
		data = stbi_load(("font/" + path + ".png").c_str(), &png_width, &png_height, &nrComponents, 0);
		if (!data) {
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}
	}

	void Font::loadPixels(unsigned char *&data, int width, int height, int nrComponents) {
		std::map<int, Character>::iterator it;

		for (it = chars.begin(); it != chars.end(); it++) {
			it->second.loadPixels(data, width, height, nrComponents);
			it->second.initGL();
		}
	}

	void Font::printAll() {
		std::map<int, Character>::iterator it;

		for (it = chars.begin(); it != chars.end(); it++) {
			std::cout << (char)(it->first) << ':' << it->second.id << std::endl;
		}
	}

	void initGL() {
		shader = new Shader("text2D.vs", "text2D.fs", nullptr);

		float vertices[] = {
			// positions          // colors           // texture coords
			0.1f,  0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // top right
			0.1f, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // bottom right
			-0.1f, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // bottom left
			-0.1f,  0.1f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f  // top left 
		};
		unsigned int indices[] = {
			3, 1, 0, // first triangle
			3, 2, 1  // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void renderChar(Character &ch, glm::vec3 translate) {
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		shader->use();
		glm::mat4 model(1);
		model = glm::translate(model, translate);
		model = glm::scale(model, glm::vec3((ch.width / base), (ch.height / lineHeight), 1.0f));
		shader->setMat4("model", model);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void renderText(std::string text, glm::vec3 translate) {
		float advance_x = 0.0f;
		for (int i = 0; i < text.size(); i++) {
			Character ch = chars.find((int)text[i])->second;
			renderChar(ch, glm::vec3(translate.x + (advance_x - ch.xoffset) / scaleW, translate.y - (ch.yoffset / 1024.0f), translate.z));
			advance_x += ch.xadvance;
		}
	}

	void Font::clean() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		delete data;

		std::map<int, Character>::iterator it;

		for (it = chars.begin(); it != chars.end(); it++) {
			it->second.clean();
		}
	}
};

class Text {
private:
	unsigned int VBO, VAO, EBO;
	GLuint textureID;
	int width = 0, height = 0;
	unsigned char* data;
	Font* font;
	float sdf_width_default = 0.4f, sdf_edge_default = 0.5f;
	float sdf_width = 0.6f, sdf_edge = 0.01f;
	glm::vec4 fontColor;
public:
	float gl_width = 0.0f, gl_height = 0.0f;
	float scale;
	float added_edge = 0.0f;
public:
	Text(std::string text, Font *&font, float scale) {
		this->scale = scale;
		this->font = font;
		fontColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		height = font->lineHeight;
		for (int i = 0; i < text.size(); i++) {
			Character ch = font->chars.find((int)text[i])->second;
			width += (ch.xadvance/* - ch.xoffset*/);
		}

		data = new unsigned char[width * height * 4];

		for (size_t i = 0; i < width * height * 4; i++) {
			data[i] = 0;
		}

		int advance = 0;
		for (int i = 0; i < text.size(); i++) {
			Character ch = font->chars.find((int)text[i])->second;
			loadPixels(ch.pixels, ch.width, ch.height, data, advance+ch.xoffset, ch.yoffset, width, height, 4);
			advance += ch.xadvance;
		}

		initGL(width, height);
	}

	void Text::loadPixels(unsigned char *&from_data, int from_width, int from_height, unsigned char *&to_data, int to_x, int to_y, int to_width, int to_height, int nrComponents) {
		for (int i = 0; i < from_height; i++) {
			for (int j = 0; j < from_width; j++) {
				to_data[(to_y + i)*to_width*nrComponents + (to_x + j)*nrComponents + 0] = from_data[i*from_width*nrComponents + j*nrComponents + 0];
				to_data[(to_y + i)*to_width*nrComponents + (to_x + j)*nrComponents + 1] = from_data[i*from_width*nrComponents + j*nrComponents + 1];
				to_data[(to_y + i)*to_width*nrComponents + (to_x + j)*nrComponents + 2] = from_data[i*from_width*nrComponents + j*nrComponents + 2];
				to_data[(to_y + i)*to_width*nrComponents + (to_x + j)*nrComponents + 3] = from_data[i*from_width*nrComponents + j*nrComponents + 3];
			}
		}
	}

	void Text::initGL(float w, float h) {
		float ratio = (w / h)/app_ratio;
		float vertices[] = {
			// positions          // colors			  // texture coords
			 0.1f*ratio,   0.1f, 0.0f,   1.0f, 1.0f, 1.0f,	 1.0f, 0.0f, // top right
			 0.1f*ratio,  -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,	 1.0f, 1.0f, // bottom right
			 -0.1f*ratio, -0.1f, 0.0f,   1.0f, 1.0f, 1.0f,	  0.0f, 1.0f, // bottom left
			 -0.1f*ratio,  0.1f, 0.0f,   1.0f, 1.0f, 1.0f,	  0.0f, 0.0f  // top left 
		};

		gl_height = 0.2f;
		gl_width = 0.2f*ratio;

		unsigned int indices[] = {
			3, 1, 0, // first triangle
			3, 2, 1  // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glGenTextures(1, &textureID);
		GLenum format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Text::render(glm::vec3 translate) {
		glBindTexture(GL_TEXTURE_2D, textureID);
		font->shader->use();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(translate.x - 1.0f + gl_width*this->scale / 2, -1*translate.y + 1.0f - gl_height*this->scale / 2, translate.z));
		model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
		font->shader->setMat4("model", model);
		font->shader->setFloat("width", map(this->scale, 0.1f, 10.0f, 0.49f, 0.6f));
		font->shader->setFloat("edge", map(this->scale, 0.1f, 10.0f, 0.20f, 0.00001f)+ added_edge);
		font->shader->setVec4("color", fontColor);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void Text::setColor(float r, float g, float b) {
		fontColor = glm::vec4(r, g, b, 1.0f);
	}

	void Text::setAlpha(float a) {
		fontColor = glm::vec4(fontColor.r, fontColor.g, fontColor.b, a);
	}

	void Text::setText(std::string text) {
		clean();
		width = 0.0f;
		height = font->lineHeight;
		for (int i = 0; i < text.size(); i++) {
			Character ch = font->chars.find((int)text[i])->second;
			width += (ch.xadvance/* - ch.xoffset*/);
		}

		data = new unsigned char[width * height * 4];

		for (size_t i = 0; i < width * height * 4; i++) {
			data[i] = 0;
		}

		int advance = 0;
		for (int i = 0; i < text.size(); i++) {
			Character ch = font->chars.find((int)text[i])->second;
			loadPixels(ch.pixels, ch.width, ch.height, data, advance + ch.xoffset, ch.yoffset, width, height, 4);
			advance += ch.xadvance;
		}

		initGL(width, height);
	}

	float map(float x, float in_min, float in_max, float out_min, float out_max) {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}

	void Text::clean() {
		glDeleteTextures(1, &textureID);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		delete data;
	}
};