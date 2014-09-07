/*
**	Author:		Martin Schwarz
**	Name:		msr.cpp
**	Project:	msr - Mars Software Renderer
**	Compile:	include in other project, linker flags: lSDL2
*/

#ifndef MSR
#define MSR

#include "msr_include.h"
#include "image.h"
#include "msr_input.h"
#include "msr_render.h"
#include "msr_camera.h"
#include "msr_model.h"

namespace msr{

class msr{
public:
//###############################################################  constructor
	msr(unsigned int screen_w = 800, unsigned int screen_h = 600, bool fullscreen = 0,
			bool input_grabbed = 0, unsigned int monitor = 0, unsigned int framerate = 240);
	~msr();
//###############################################################  init
	bool init();
//###############################################################  swap window
	void swap_window(SDL_Surface *blit);
	void swap_window(image &img);
//###############################################################  get
	float get_aspect_ratio();
	unsigned int get_height();
	unsigned int get_width();
protected:
//###############################################################  variables
	SDL_Window *window;
	unsigned int screen_w, screen_h, monitor, framerate;
	bool fullscreen, input_grabbed;
	SDL_Surface *window_surface;
private:
};

}

#endif // MSR











