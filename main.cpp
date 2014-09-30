/*
**	Author:		Martin Schwarz
**	Name:		msr.cpp
**	Project:	msr - Mars Software Renderer
**	Compile:	include in other project, linker flags: lSDL2
*/

#include "msr_test.h"

int main(int argc, char *argv[]){
	msr::msr g(800, 600, 0);
	msr::input i;
	g.init();

	image img(g.get_width(), g.get_height());
	img.clear(0, 0, 0);

	msr::camera cam(90.f * 0.0174532925f, g.get_aspect_ratio(), 0.1f, 1000.f, 2, 0.6f, 0.06f);
	msr::model obj1("res/cube.obj");
	msr::model obj2("res/suzanne.obj");
	obj2.move(glm::vec3(0.f, 0.f, -10.f));
	//img.save();
	float current = 0, last = 0, frametime = 0;

	bool quit = false;
	while(quit != true){
		i.update();
		quit = i.get_quit();
		cam.update(i.get_pos(), i.get_rot());

		img.clear(0, 0, 0);

		msr::render(1, obj1, cam, img, 3);
		msr::render(1, obj2, cam, img, 3);

		g.swap_window(img);

		current = SDL_GetTicks();
		frametime = current - last;
		last = current;
		std::cerr << "FPS: " << 1000.f / frametime << std::endl;
		float delay = 16.7 - frametime;
		//if(delay > 0) SDL_Delay(delay);
	}

	return 0;
}
