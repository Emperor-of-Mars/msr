/*
**	Author:		Martin Schwarz
**	Name:		image.cpp
**	Project:	image - create and save bmp
**	Compile:	include in other project
*/

#include "image.h"

image::image(unsigned int w, unsigned int h){
	data_pos = 54;
	width = w;
	height = h;
	size = width * height * sizeof(unsigned char) * 3;
	data = new unsigned char[width * height * 3];
	//std::cerr << size / 1024 << " KB" << std::endl;
}

image::~image(){
	delete data;
}

void image::resize(unsigned int w, unsigned int h){
	delete data;
	width = w;
	height = h;
	size = width * height * sizeof(unsigned char) * 3;
	data = new unsigned char[width * height * 3];
	return;
}

void image::clear(unsigned char r, unsigned char g, unsigned char b){
	for(unsigned int i = 0; i < width * height * 3; i += 3){
		data[i] = b;
		data[i + 1] = g;
		data[i + 2] = r;
	}
	return;
}

bool image::draw_point(unsigned int pos_w, unsigned int pos_h, unsigned char r, unsigned char g, unsigned char b){
	if(pos_w >= width || pos_h >= height){
		std::cerr << "wrong image position!" << std::endl;
		return false;
	}
	//std::cerr << "render at: " << pos_w << " " << pos_h << std::endl;
	pos_h = height - pos_h - 1;
	data[pos_h * width * 3 + pos_w * 3] = b;
	data[pos_h * width * 3 + pos_w * 3 + 1] = g;
	data[pos_h * width * 3 + pos_w * 3 + 2] = r;
	return true;
}

/*SDL_Surface *image::to_surface(){
	SDL_Surface *res = SDL_CreateRGBSurfaceFrom(data, width, height, 24, width * 3, 0xff0000, 0x00ff00, 0x0000ff, 0);
	if(res == NULL) std::cerr << "image to surface failed!" << std::endl;
	return res;
}*/

bool image::save(){
	if(width % 4 != 0 && height % 4 != 0){
		std::cerr << "wrong image size!" << std::endl;
		return false;
	}
	unsigned char head[54];
	head[0] = 'B';
	head[1] = 'M';
	*(uint32_t*)&(head[2]) = size + 54;
	*(uint32_t*)&(head[6]) = 0;
	*(uint32_t*)&(head[10]) = 54;
	*(uint32_t*)&(head[14]) = 40;
	*(uint32_t*)&(head[18]) = width;
	*(uint32_t*)&(head[22]) = height;
	*(uint16_t*)&(head[26]) = 1;
	*(uint16_t*)&(head[28]) = 24;
	*(uint32_t*)&(head[30]) = 0;
	*(uint32_t*)&(head[34]) = size;
	*(uint32_t*)&(head[38]) = 0;
	*(uint32_t*)&(head[42]) = 0;
	*(uint32_t*)&(head[46]) = 0;
	*(uint32_t*)&(head[50]) = 0;

	std::ofstream out("image_test.bmp", std::ofstream::out);
	out.write((const char*)head, 54);
	out.write((const char*)data, width * height * 3);
	while(out.tellp() % 4 != 0) out.put(0);
	out.close();
	return true;
}

unsigned int image::get_width(){
	return width;
}

unsigned int image::get_height(){
	return height;
}

unsigned int image::get_size(){
	return size;
}

const unsigned char *image::get_data(){
	return (const unsigned char *)data;
}

























