#define GL_GLEXT_PROTOTYPES

#include "App.h"

int main()
{
	App* app = App::create();
	app->run();

	delete app;

	return EXIT_SUCCESS;
}

