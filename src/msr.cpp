/*
**	Author:		Martin Schwarz
**	Name:		msr.cpp
**	Project:	msr - Mars Software Renderer
**	Compile:	include in other project, linker flags: lSDL2
*/

#include "msr.h"

namespace msr{

//###############################################################  msr class

//###############################################################  constructor
msr::msr(unsigned int screen_w, unsigned int screen_h, bool fullscreen,
		   bool input_grabbed, unsigned int monitor, unsigned int framerate)
{
	this->window = NULL;
	this->screen_w = screen_w;
	this->screen_h = screen_h;
	this->fullscreen = fullscreen;
	this->input_grabbed = input_grabbed;
	this->monitor = monitor;
	this->framerate = framerate;
	this->window_surface = NULL;
}

msr::~msr(){
	std::cerr << "closing SDL" << std::endl;
	SDL_FreeSurface(window_surface);
	window = NULL;
	SDL_DestroyWindow(window);
	window_surface = NULL;
	SDL_Quit();
	std::cerr << "SDL closed!" << std::endl;
}

//###############################################################  init_sdl
bool msr::init(){
	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cerr << "SDL_Init failed! SDL_Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	if(success){
		int flags = IMG_INIT_PNG | IMG_INIT_JPG;
		if(!(IMG_Init(flags) & flags))
		{
			std::cerr << "IMG_Init failed! IMG_Error: " << IMG_GetError() << std::endl;
			success = false;
		}
	}
	if(success){
		int flags = SDL_WINDOW_SHOWN;
		if(fullscreen == 1) flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		if(input_grabbed == 1) flags |= SDL_WINDOW_INPUT_GRABBED;

		int windowpos_x = SDL_WINDOWPOS_UNDEFINED_DISPLAY(monitor);
		int windowpos_y = SDL_WINDOWPOS_UNDEFINED_DISPLAY(monitor);

		window = SDL_CreateWindow("mgf", windowpos_x, windowpos_y,
											screen_w, screen_h, flags);
		if(window == NULL){
			std::cerr << "SDL_CreateWindow failed! SDL_Error: " << SDL_GetError() << std::endl;
			success = false;
		}
	}

	if(success) window_surface = SDL_GetWindowSurface(window);

	if(!success){
		std::cerr << "initializing SDL failed!" << std::endl;
		std::cerr << "closing SDL" << std::endl;
		SDL_FreeSurface(window_surface);
		window_surface = NULL;
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		std::cerr << "SDL closed!" << std::endl;
	}

	std::cerr << "SDL initialized successfully!" << std::endl;
	return true;
}

//###############################################################  swap window
void msr::swap_window(SDL_Surface *blit){
	SDL_BlitScaled(blit, NULL, window_surface, &window_surface->clip_rect);
	SDL_UpdateWindowSurface(window);
	return;
}

void msr::swap_window(image &img){
	SDL_Surface *blit = SDL_CreateRGBSurfaceFrom((void *)img.get_data(), img.get_width(),
				img.get_height(), 24, img.get_width() * 3, 0xff0000, 0x00ff00, 0x0000ff, 0);
	SDL_BlitScaled(blit, NULL, window_surface, &window_surface->clip_rect);
	SDL_UpdateWindowSurface(window);
	return;
}

//###############################################################  get
float msr::get_aspect_ratio(){
	return (float)screen_w / (float)screen_h;
}

unsigned int msr::get_height(){
	return screen_h;
}

unsigned int msr::get_width(){
	return screen_w;
}

}	//msr























