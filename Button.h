#pragma once
#include "Text.h"
#include "image_loader.h"
#include "mouse.h"

class Button {
private:
	Shader* shader;
	unsigned int VBO, VAO, EBO;
	glm::vec4 color;
	Text* innerText;
	float gl_width, gl_height;
	float padding_vert, padding_hor;
	float button_x, button_y;
	float button_width, button_height;
public: //animation
	float start_rot_x = 0.0f, end_rot_x = 90.0f, rot_speed_x = 10.0f;
	float rotation_x = end_rot_x, rotation_y = 0.0f, rotation_z = 0.0f;
private:
	void (*callback)();
	bool canCallback = false;
public:
	Button(Text *&text, float x, float y, float padding_hor, float padding_vert, void (*ptr)()) {
		shader = new Shader("button2D.vs", "button2D.fs", nullptr);
		color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		innerText = text;
		this->padding_hor = padding_hor;
		this->padding_vert = padding_vert;
		this->button_x = x;
		this->button_y = y;
		initGL();
		this->callback = ptr;
	}

	void Button::initGL() {
		//float ratio = (innerText->gl_width / innerText->gl_height) * app_ratio;
		this->padding_hor = padding_hor * app_ratio;
		this->padding_vert = padding_vert / app_ratio;
		float vertices[] = {
			// positions          
			innerText->gl_width / 2 + padding_hor,    innerText->gl_height / 2 + padding_vert, 0.0f, // top right
			innerText->gl_width / 2 + padding_hor,   -innerText->gl_height / 2 - padding_vert, 0.0f, // bottom right
			-innerText->gl_width / 2 - padding_hor,   -innerText->gl_height / 2 - padding_vert, 0.0f,  // bottom left
		    -innerText->gl_width / 2 - padding_hor,    innerText->gl_height / 2 + padding_vert, 0.0f,  // top left 
		};

		gl_width = innerText->gl_width + padding_hor*2;
		gl_height = innerText->gl_height + padding_vert*2;

		button_width = gl_width*innerText->scale;
		button_height = gl_height*innerText->scale;

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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void Button::tick() {
		if ((Mouse::mouse_gl_x >= button_x - 1.0f && Mouse::mouse_gl_x <= button_x + button_width - 1.0f) && (Mouse::mouse_gl_y <= -button_y + 1.0f && Mouse::mouse_gl_y >= -button_y - button_height + 1.0f)) {
			rotate_x_to_end();
			if (Mouse::new_actions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && Mouse::old_actions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
				glow();
				canCallback = true;
			}
			else {
				//fade();
				if (Mouse::new_actions[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
					if (canCallback) {
						canCallback = false;
						callback();
					}
					fade();
				}
			}
		}else{
			canCallback = false;
			rotate_x_to_start();
			fade();
		}
	}

	void Button::rotate_x_to_end() {
		if (rotation_x > start_rot_x) {
			rotation_x -= rot_speed_x;
		}
	}

	void Button::rotate_x_to_start() {
		if (rotation_x < end_rot_x) {
			rotation_x += rot_speed_x;
		}
	}

	void Button::glow() {
		if (innerText->added_edge < 0.17f) {
			innerText->added_edge += 0.02f;
		}
	}

	void Button::fade() {
		if (innerText->added_edge > 0.0f) {
			innerText->added_edge -= 0.02f;
		}
	}

	void Button::render() {
		shader->use();
		glm::mat4 model(1);
		model = glm::translate(model, glm::vec3(button_x - 1.0f + gl_width*innerText->scale / 2, -1 * button_y + 1.0f - gl_height*innerText->scale / 2, 0.0f));
		model = glm::scale(model, glm::vec3(innerText->scale, innerText->scale, 1.0f));
		model = glm::rotate(model, glm::radians(rotation_x), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotation_z), glm::vec3(0.0f, 0.0f, 1.0f));
		shader->setMat4("model", model);
		shader->setVec4("color", color);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		///innerText->render(glm::vec3(translate.x + padding_vert*scale, translate.y + padding_hor*scale, translate.z), scale);
		innerText->render(glm::vec3(button_x + padding_hor*innerText->scale, button_y + padding_vert*innerText->scale, 0.0f));
	}

	void Button::clean() {
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
	}

	void Button::setColor(float r, float g, float b) {
		color = glm::vec4(r, g, b, color.a);
	}

	void setAlpha(float a) {
		color = glm::vec4(color.r, color.g, color.b, a);
	}
};

class TextButton {
	
};

class ImageButton {

};