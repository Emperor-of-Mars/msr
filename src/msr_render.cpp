/*
**	Author:		Martin Schwarz
**	Name:		msr_renderer.cpp
**	Project:	msr - Mars Software Renderer
**	Compile:	include in other project, linker flags: lSDL2
*/

#include "msr_render.h"

namespace msr{

inline glm::vec3 transform_point(glm::vec3 &p, glm::mat4 &trans){
	glm::vec4 res(p, 1);
	res = trans * res;
	//std::cerr << res[0] << " " << res[1] << " " << res[2] << " " << res[3] << std::endl;
	return glm::vec3(res[0], res[1], res[3]);
}

inline void render_points(std::vector<glm::vec3> &points, glm::mat4 trans, image &img, unsigned int size){
	float near = 0.1;
	for(unsigned int i = 0; i < points.size(); i++){
		glm::vec3 transformed = transform_point(points[i], trans);
		if(transformed[2] < near){
			continue;
		}
		if(transformed[0] < -transformed[2]){
			continue;
		}
		if(transformed[0] > transformed[2]){
			continue;
		}
		if(transformed[1] < -transformed[2]){
			continue;
		}
		if(transformed[1] > transformed[2]){
			continue;
		}
		glm::vec2 p(transformed[0] / transformed[2] * img.get_width() / 2 + img.get_width() / 2,
					transformed[1] / transformed[2] * img.get_height() / 2 + img.get_height() / 2);
		for(unsigned int y = size; y < -size; y--){
			for(unsigned int x = size; x < -size; x--){
				img.draw_point((int)p[0] + y, (int)p[1] + x, 255, 255, 255);
			}
		}
	}
	return;
}

inline void draw_line(glm::vec3 point1, glm::vec3 point2, image &img, unsigned int size){
	float near = 0.1;
	if(point1[2] < near && point2[2] < near){
		return;
	}
	if(point1[2] < near){
        float n = (near - point1[2]) / (point2[2] - point1[2]);
        point1[0] = point1[0] + n * (point2[0] - point1[0]);
        point1[1] = point1[1] + n * (point2[1] - point1[1]);
        point1[2] = near;
	}
	else if(point2[2] < near){
		glm::vec3 tmp = point1;
		point1 = point2;
		point2 = tmp;
        float n = (near - point1[2]) / (point2[2] - point1[2]);
        point1[0] = point1[0] + n * (point2[0] - point1[0]);
        point1[1] = point1[1] + n * (point2[1] - point1[1]);
        point1[2] = near;
	}
//#################################### this doesnt work properly
    float mul = 0.8;
	if(point1[0] < -point1[2] * mul && point2[0] < -point2[2] * mul){
		return;
	}
	if(point1[0] < -point1[2] * mul){
        float n = (-point1[2] * mul - point1[0]) / ((point2[0] - point1[0]) - (point2[2] - point1[2]));
        point1[0] = point1[0] + n * (point2[0] - point1[0]);
        point1[1] = point1[1] + n * (point2[1] - point1[1]);
        point1[2] = point1[0];
	}
	else if(point2[0] < -point2[2] * mul){
		glm::vec3 tmp = point1;
		point1 = point2;
		point2 = tmp;
        float n = (-point1[2] * mul - point1[0]) / ((point2[0] - point1[0]) - (point2[2] - point1[2]));
        point1[0] = point1[0] + n * (point2[0] - point1[0]);
        point1[1] = point1[1] + n * (point2[1] - point1[1]);
        point1[2] = point1[0];
	}
//####################################

	glm::vec2 p1(point1[0] / point1[2] * img.get_width() / 2 + img.get_width() / 2,
				point1[1] / point1[2] * img.get_height() / 2 + img.get_height() / 2);
	glm::vec2 p2(point2[0] / point2[2] * img.get_width() / 2 + img.get_width() / 2,
				point2[1] / point2[2] * img.get_height() / 2 + img.get_height() / 2);

	glm::vec2 dir = p2 - p1;
	float length = sqrt(dir[0] * dir[0] + dir[1] * dir[1]);
	glm::vec2 step(dir[0] / length, dir[1] / length);

	for(unsigned int i = 0; i < (unsigned int)length; i++){
		if(!img.draw_point((int)p2[0] - (int)(step[0] * i), (int)p2[1] - (int)(step[1] * i), 255, 255, 255)){
			for(unsigned int i = (unsigned int)length; i > 0; i--){
				if(!img.draw_point((int)p2[0] - (int)(step[0] * i), (int)p2[1] - (int)(step[1] * i), 255, 255, 255)) break;
			}
			break;
		}
		//img.draw_point((int)p2[0] - (int)(step[0] * i), (int)p2[1] - (int)(step[1] * i), 255, 255, 255);
	}
	return;
}

inline void render_lines(std::vector<glm::vec3> &points, glm::mat4 trans, image &img, unsigned int size){
	for(unsigned int i = 0; i < points.size(); i += 3){
		glm::vec3 transformed1 = transform_point(points[i], trans);
		glm::vec3 transformed2 = transform_point(points[i + 1], trans);
		glm::vec3 transformed3 = transform_point(points[i + 2], trans);
		if(transformed1[2] < 0.f && transformed2[2] < 0.f && transformed3[2] < 0.f){
			continue;
		}
		draw_line(transformed1, transformed2, img, size);
		draw_line(transformed2, transformed3, img, size);
		draw_line(transformed3, transformed1, img, size);
	}
	return;
}

void render(unsigned int mode, model &mod, camera &cam, image &img, unsigned int size){
	for(unsigned int i = 0; i < mod.meshes.size(); i++){
		if(mod.meshes[i]->has_vertices){
			glm::mat4 trans(cam.get_vp() * mod.get_trans_mat());
			switch(mode){
			case 0:
				render_points(mod.meshes[i]->vertices, trans, img, size);
				break;
			case 1:
				render_lines(mod.meshes[i]->vertices, trans, img, size);
				break;
			}
		}
	}
	return;
}


}

















