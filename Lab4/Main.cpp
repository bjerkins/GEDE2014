#include "MyApplication.cpp"

int main (void)
{
	MyApplication app;
	app.startup();
	while (app.keepRunning()) {
		app.renderOneFrame();
	}
	return 0;
}