/* this class reads window hint config file and stores relevant info*/
#pragma once
#include "global.h"

class Config {
public:
	int WIDTH;
	int HEIGHT;
	bool FULLSCREEN;
	bool RESIZEABLE;
	bool DECORATED;
public:
	Config::Config(const char* path) {
		this->WIDTH = 1280;
		this->HEIGHT = 720;
		this->FULLSCREEN = false;
		this->RESIZEABLE = false;
		this->DECORATED = false;

		app_width = this->WIDTH;
		app_height = this->HEIGHT;
		app_ratio = (float)this->WIDTH / this->HEIGHT;
	}

	Config::Config() {
		this->WIDTH = 800;
		this->HEIGHT = 800;
		this->FULLSCREEN = true;
		this->RESIZEABLE = false;
		this->DECORATED = true;
	}
};

