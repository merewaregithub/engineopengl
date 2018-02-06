#pragma once
#include "Shader.h"
#include "Model.h"
#include "config.h"

class Light {
public:
	Model* lightModel;
	Shader* lightModelShader;
	Shader* shader;
public:
	int id;
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
public:
	Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	Light::Light() {

	}

	Light::~Light() {

	}
};

struct DirectLight : public Light {
public:
	std::string type;
	glm::vec3 direction;
	DirectLight::DirectLight(Shader*& shd, int id, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		this->type = "direct_light";
		this->id = id;
		this->direction = direction;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		this->shader = shd;
		shader->setVec3("dirLight.direction", direction);
		shader->setVec3("dirLight.ambient", ambient);
		shader->setVec3("dirLight.diffuse", diffuse);
		shader->setVec3("dirLight.specular", specular);
	}
};

struct PointLight : public Light {
public:
	float constant;
	float linear;
	float quadratic;
	std::string type;
	PointLight::PointLight(Shader*& shd, int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, float linear, float quadratic) {
		type = "point_light";
		this->id = id;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->position = position;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		lightModel = new  Model("point_light");
		lightModelShader = new Shader("light_model.vs", "light_model.fs");
		lightModelShader->use();
		lightModelShader->setMat4("projection", glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f));
		lightModelShader->setVec4("color", glm::vec4(diffuse, 1.0f));

		lightModel->setShader(lightModelShader);

		this->shader = shd;
		this->shader->use();
		this->shader->setInt("point_light_count", id + 1);
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].position", position);
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].ambient", ambient);
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].diffuse", diffuse);
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].specular", specular);
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].constant", constant);
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].linear", linear);
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].quadratic", quadratic);
	}

	void PointLight::setDiffuse(glm::vec3 value) {
		this->lightModelShader->use();
		this->lightModelShader->setVec4("color", glm::vec4(value, 1.0f));
		this->shader->use();
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].diffuse", value);
	}

	void PointLight::setLinear(float value) {
		this->shader->use();
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].linear", value);
	}

	void PointLight::setConstant(float value) {
		this->shader->use();
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].constant", value);
	}

	void PointLight::setQuadratic(float value) {
		this->shader->use();
		this->shader->setFloat("pointLights[" + std::to_string(id) + "].quadratic", value);
	}

	void PointLight::show(glm::mat4 view) {
		lightModel->render(view, this->position);
	}

	void PointLight::setPosition(glm::vec3 pos) {
		this->position = pos;
		this->shader->use();
		this->shader->setVec3("pointLights[" + std::to_string(id) + "].position", position);
	}
};

struct SpotLight : public Light {
public:
	glm::vec3 direction;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;

	glm::vec3 modelDirection;
	float angle;
	glm::vec3 normal;

	std::string type;
	SpotLight::SpotLight(Shader*& shader, int id, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff, float constant, float linear, float quadratic) {
		lightModel = new  Model("point_light");
		type = "spot_light";
		this->id = id;
		this->cutOff = cutOff;
		this->outerCutOff = outerCutOff;
		this->constant = constant;
		this->linear = linear;
		this->quadratic = quadratic;
		this->position = position;
		this->direction = direction;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;

		this->angle = 0.0f;
		this->modelDirection = direction;

		lightModelShader = new Shader("lightModelShader.vs", "lightModelShader.frag");
		lightModelShader->use();
		lightModelShader->setMat4("projection", glm::perspective(glm::radians(45.0f), app_ratio, 0.1f, 150.0f));
		lightModelShader->setVec3("color", diffuse);

		lightModel->setShader(lightModelShader);

		this->shader = shader;
		this->shader->use();
		this->shader->setInt("spot_light_count", id + 1);
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].position", position);
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].direction", direction);
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].ambient", ambient);
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].diffuse", diffuse);
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].specular", specular);
		this->shader->setFloat("spotLights[" + std::to_string(id) + "].cutOff", cutOff);
		this->shader->setFloat("spotLights[" + std::to_string(id) + "].outerCutOff", outerCutOff);
		this->shader->setFloat("spotLights[" + std::to_string(id) + "].constant", constant);
		this->shader->setFloat("spotLights[" + std::to_string(id) + "].linear", linear);
		this->shader->setFloat("spotLights[" + std::to_string(id) + "].quadratic", quadratic);
	}

	void SpotLight::show(glm::mat4 view) {
		//lightModel->render(view, normal, glm::radians(angle), position);
	}

	void SpotLight::setPosition(glm::vec3 pos) {
		this->position = pos;
		this->shader->use();
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].position", position);
	}

	void SpotLight::setDirection(glm::vec3 dir) {
		this->direction = dir;
		this->shader->use();
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].direction", direction);
	}

	void SpotLight::rotate(glm::vec3 normal, float angle) {
		this->normal = normal;
		this->angle = angle;
		//this->direction = glm::rotate(modelDirection, glm::radians(angle), normal);
		this->shader->use();
		this->shader->setVec3("spotLights[" + std::to_string(id) + "].direction", direction);
	}
};