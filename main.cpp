#include "global.h"
float app_ratio, app_width, app_height;
#include "config.h"
#include "window.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Window window("");

int main() {
	try {
		window.run();
	}
	catch (const std::runtime_error& error) {
		std::cerr << error.what() << std::endl;
		return EXIT_FAILURE;
	}

	window.cleanUp();
	return 1;
}