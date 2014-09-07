/*
**	Author:		Martin Schwarz
**	Name:		mgf_model.h
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "msr_include.h"

#ifndef MSR_MODEL
#define MSR_MODEL

namespace msr{


//###############################################################  mesh class
class obj_mesh{
public:
//###############################################################  constructor
	obj_mesh();
	~obj_mesh();
//###############################################################  variables
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texcoords;
	unsigned int material_index;
	std::string name;
	bool has_vertices, has_normals, has_texcoords;
protected:
private:
};


//###############################################################  obj_material class
class obj_material{
public:
//###############################################################  constructor
	obj_material();
	~obj_material();
//###############################################################  load from file
	bool load_texture(std::string file);
//###############################################################  use_mtl
	void use_mtl();
//###############################################################  variables
	unsigned int material_index;
	glm::vec3 color_diffuse;
	float alpha;
	std::string name;
protected:
private:
};


//###############################################################  model class
class model{
public:
//###############################################################  constructor
	model(std::string file);
	~model();
//###############################################################  load from file
	bool load_file(std::string file);
	bool load_mtl(std::string file);
	unsigned int get_mtl_index(std::string name);
//###############################################################  transformation matrix
	glm::mat4 get_trans_mat();
	void set_trans_mat(glm::mat4 mat);
//###############################################################  move
	glm::mat4 move(glm::vec3 mov);
	glm::mat4 rotate(float degrees, glm::vec3 axis);
	glm::mat4 scale(glm::vec3 scale);
//###############################################################  render
	void render();
//###############################################################  variables
	std::vector<obj_mesh *> meshes;
	std::vector<obj_material *> materials;
	std::string name;
	glm::mat4 trans;
protected:
private:
};

}

#endif // MSR_MODEL















