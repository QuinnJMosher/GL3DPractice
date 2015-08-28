//my object includes
#include <Application.h>

int main() {
	
	Application* app = new Application();
	if (app->Start() == true) {
		while (app->Update() == true) {
			app->Draw();
		}
		app->Shutdown();
	}

	delete app;
	return 0;
}