/*
**	Author:		Martin Schwarz
**	Name:		image.h
**	Project:	image - create and save bmp
**	Compile:	include in other project
*/

#include <iostream>
#include <fstream>
//#include <SDL2/SDL.h>

#ifndef IMAGE
#define IMAGE

class image{
public:
	image(unsigned int w = 0, unsigned int h = 0);
	~image();
	void resize(unsigned int w, unsigned int h);
	void clear(unsigned char r = 0, unsigned char g = 0, unsigned char b = 0);
	bool draw_point(unsigned int pos_w, unsigned int pos_h, unsigned char r, unsigned char g, unsigned char b);
	//SDL_Surface *to_surface();
	bool save();
	unsigned int get_width();
	unsigned int get_height();
	unsigned int get_size();
	const unsigned char *get_data();
private:
	unsigned int data_pos, width, height, size;
	unsigned char *data;
};

#endif // IMAGE
