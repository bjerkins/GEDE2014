#include "OGRE\Ogre.h"
#include "MyApplication.h"

int main(void) {
	MyApplication app;

	// Load up the application resources and stuff :)
	app.startUp();

	//  Start the rendering loop
	while (app.keepRunning()) {
		app.renderOneFrame();
	}

	return 0;
}
