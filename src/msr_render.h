/*
**	Author:		Martin Schwarz
**	Name:		msr_renderer.cpp
**	Project:	msr - Mars Software Renderer
**	Compile:	include in other project, linker flags: lSDL2
*/

#include "msr_include.h"

#include "msr_camera.h"
#include "msr_model.h"
#include "image.h"

#ifndef MSR_RENDERER
#define MSR_RENDERER

namespace msr{

glm::vec3 transform_point(glm::vec3 &p, glm::mat4 &trans);
void render_points(std::vector<glm::vec3> &points, glm::mat4 trans, image &img, unsigned int size);
void draw_line(glm::vec3 point1, glm::vec3 point2, image &img, unsigned int size);
void render_lines(std::vector<glm::vec3> &points, glm::mat4 trans, image &img, unsigned int size);
void render(unsigned int mode, model &mod, camera &cam, image &img, unsigned int size = 1);


}

#endif // MSR_RENDERER
