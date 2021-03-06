/*
**	Author:		Martin Schwarz
**	Name:		mgf_model.cpp
**	Project:	mgf - Mars Graphics Framework
**	Compile:	include in other project, linker flags: lSDL2 -lGLEW -lGL
*/

#include "msr_model.h"

namespace msr{


//###############################################################  obj_mesh class

//###############################################################  constructor
obj_mesh::obj_mesh(){
	material_index = 0;
	has_vertices = 0;
	has_normals = 0;
	has_texcoords = 0;
	vertices.clear();
	texcoords.clear();
	normals.clear();
}

obj_mesh::~obj_mesh(){
	material_index = 0;
	has_vertices = 0;
	has_normals = 0;
	has_texcoords = 0;
}


//###############################################################  obj_material class

//###############################################################  constructor
obj_material::obj_material(){
	color_diffuse = glm::vec3(0);
	alpha = 1.f;
}

obj_material::~obj_material(){
}

//###############################################################  load from file
bool obj_material::load_texture(std::string file){
	std::cerr << "texture: " << file << std::endl;

	SDL_Surface *image = IMG_Load(file.c_str());

    SDL_FreeSurface(image);
	return true;
}

//###############################################################  use_mtl
void obj_material::use_mtl(){
	return;
}


//###############################################################  model class

//###############################################################  constructor
model::model(std::string file){
	load_file(file);
	trans = glm::mat4(1.f);
}

model::~model(){
}


//###############################################################  load from file
bool model::load_file(std::string file){
	std::cerr << "loading file: " << file << std::endl;
	std::ifstream in(file);
	if(!in.is_open()){
		std::cerr << "failed to load file: " << file << std::endl;
		return false;
	}
	name = file;
	std::string line;
	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec3> tmp_texcoords;
	std::vector<glm::vec3> tmp_normals;
	std::vector<std::vector<unsigned int>> indices_vertex;
	std::vector<std::vector<unsigned int>> indices_texcoords;
	std::vector<std::vector<unsigned int>> indices_normals;
	std::vector<bool> has_vertices;
	std::vector<bool> has_texcoords;
	std::vector<bool> has_normals;
	std::vector<unsigned int> material_index;
	std::vector<std::string> names;

	unsigned int cursize = 0;

	obj_material *mat = new obj_material;
	mat->color_diffuse = glm::vec3(1.0f, 0.5f, 0.7f);
	materials.push_back(mat);

	while(getline(in, line)){
		if(line.substr(0, 2) == "v "){
			has_vertices[cursize - 1] = 1;
			glm::vec3 vertex(0);
			sscanf(line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
			tmp_vertices.push_back(vertex);
			//std::cerr << "v " << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
		}
		else if(line.substr(0, 2) == "vt"){
			has_texcoords[cursize - 1] = 1;
			glm::vec3 texcoord(0);
			sscanf(line.c_str(), "vt %f %f", &texcoord[0], &texcoord[1]);
			//std::cerr << "vt " << texcoord[0] << " " << texcoord[1] << std::endl;
			tmp_texcoords.push_back(texcoord);
		}
		else if(line.substr(0, 2) == "vn"){
			has_normals[cursize - 1] = 1;
			glm::vec3 normal(0);
			sscanf(line.c_str(), "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
			//std::cerr << "vn " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
			tmp_normals.push_back(normal);
		}
		else if(line.substr(0, 2) == "f "){
			//std::cerr << "face" << std::endl;
			unsigned int vert_index[3], norm_index[3], texcoord_index[3];
			if(has_normals[cursize - 1] == 1 && has_texcoords[cursize - 1] == 1){
				sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
						&vert_index[0], &texcoord_index[0], &norm_index[0],
						&vert_index[1], &texcoord_index[1], &norm_index[1],
						&vert_index[2], &texcoord_index[2], &norm_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_normals[cursize - 1].push_back(norm_index[0]);
				indices_normals[cursize - 1].push_back(norm_index[1]);
				indices_normals[cursize - 1].push_back(norm_index[2]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[0]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[1]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[2]);
			}
			else if(has_normals[cursize - 1] == 1 && has_texcoords[cursize - 1] == 0){
				sscanf(line.c_str(), "f %d//%d %d//%d %d//%d",
						&vert_index[0], &norm_index[0],
						&vert_index[1], &norm_index[1],
						&vert_index[2], &norm_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_normals[cursize - 1].push_back(norm_index[0]);
				indices_normals[cursize - 1].push_back(norm_index[1]);
				indices_normals[cursize - 1].push_back(norm_index[2]);
			}
			else if(has_normals[cursize - 1] == 0 && has_texcoords[cursize - 1] == 1){
				sscanf(line.c_str(), "f %d/%d %d/%d %d/%d",
						&vert_index[0], &texcoord_index[0],
						&vert_index[1], &texcoord_index[1],
						&vert_index[2], &texcoord_index[2]);
				indices_vertex[cursize - 1].push_back(vert_index[0]);
				indices_vertex[cursize - 1].push_back(vert_index[1]);
				indices_vertex[cursize - 1].push_back(vert_index[2]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[0]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[1]);
				indices_texcoords[cursize - 1].push_back(texcoord_index[2]);
			}
		}
		else if(line.substr(0, 2) == "o "){
			cursize++;
			indices_vertex.resize(cursize);
			indices_texcoords.resize(cursize);
			indices_normals.resize(cursize);
			has_vertices.resize(cursize);
			has_texcoords.resize(cursize);
			has_normals.resize(cursize);
			material_index.resize(cursize);
			names.resize(cursize);
			has_vertices[cursize - 1] = 0;
			has_texcoords[cursize - 1] = 0;
			has_normals[cursize - 1] = 0;
			material_index[cursize - 1] = 0;
			names[cursize - 1] = line.substr(2);

			/*if(cursize > 1) std::cerr << indices_vertex[cursize - 2].size() << std::endl << std::endl;
			else std::cerr << "0" << std::endl << std::endl;
			std::cerr << "new 1" << std::endl;
			std::cerr << line.substr(2) << std::endl;*/
		}
		else if(line.substr(0, 6) == "usemtl"){
			if(indices_vertex[cursize - 1].size() > 0){
				cursize++;
				indices_vertex.resize(cursize);
				indices_texcoords.resize(cursize);
				indices_normals.resize(cursize);
				has_vertices.resize(cursize);
				has_texcoords.resize(cursize);
				has_normals.resize(cursize);
				material_index.resize(cursize);
				names.resize(cursize);
				has_vertices[cursize - 1] = has_vertices[cursize - 2];
				has_texcoords[cursize - 1] = has_texcoords[cursize - 2];
				has_normals[cursize - 1] = has_normals[cursize - 2];
				material_index[cursize - 1] = get_mtl_index(line.substr(7));
				names[cursize - 1] = names[cursize - 2];

				/*std::cerr << indices_vertex[cursize - 2].size() << std::endl << std::endl;
				std::cerr << "new 2.1 " << std::endl;
				std::cerr << line.substr(7) << std::endl;*/
			}
			else{
				material_index[cursize - 1] = get_mtl_index(line.substr(7));

				/*std::cerr << indices_vertex[cursize - 1].size() << std::endl << std::endl;
				std::cerr << "new 2.2 " << std::endl;
				std::cerr << line.substr(7) << std::endl;*/
			}
		}
		else if(line.substr(0, 6) == "mtllib"){
			std::string mtl_loc = file;
			for(unsigned int i = mtl_loc.size() - 1; i > 0; i--){
				if(mtl_loc[i] == '/') break;
				else mtl_loc.erase(mtl_loc.begin() + i);
			}
			mtl_loc.append(line.substr(7));
			load_mtl(mtl_loc);
			//std::cerr << mtl_loc << std::endl;
		}
		else if(line.substr(0, 2) == "# "){
			//ignore
		}
		else{
			//implement later
		}
		//std::cerr << (char)in.peek() << std::endl;
	}

	for(unsigned int i = 0; i < indices_vertex.size(); i++){
		obj_mesh *m = new obj_mesh;
		m->material_index = material_index[i];
		if(has_vertices[i] == 1){
			m->has_vertices = 1;
			for(unsigned int j = 0; j < indices_vertex[i].size(); j++){
				indices_vertex[i][j] -= 1;
				m->vertices.push_back(tmp_vertices[indices_vertex[i][j]]);
			}
		}
		if(has_texcoords[i] == 1){
			m->has_texcoords = 1;
			for(unsigned int j = 0; j < indices_texcoords[i].size(); j++){
				indices_texcoords[i][j] -= 1;
				m->texcoords.push_back(tmp_texcoords[indices_texcoords[i][j]]);
			}
		}
		if(has_normals[i] == 1){
			m->has_normals = 1;
			for(unsigned int j = 0; j < indices_normals[i].size(); j++){
				indices_normals[i][j] -= 1;
				m->normals.push_back(tmp_normals[indices_normals[i][j]]);
			}
		}
		meshes.push_back(m);

		/*std::cerr << "vertex_size: " << m->vertices.size() << std::endl;
		std::cerr << "texcoords_size: " << m->texcoords.size() << std::endl;
		std::cerr << "normals_size: " << m->normals.size() << std::endl << std::endl;*/
	}

	/*for(unsigned int i = 0; i < vertices.size(); i++)
		std::cerr << "vertices: " << vertices[i][0] << " " << vertices[i][1] << " " << vertices[i][2] << std::endl;
	for(unsigned int i = 0; i < normals.size(); i++)
		std::cerr << "normals: " << normals[i][0] << " " << normals[i][1] << " " << normals[i][2] << std::endl;
	for(unsigned int i = 0; i < texcoords.size(); i++)
		std::cerr << "texcoords: " << i + 1 << " : " << texcoords[i][0] << " " << texcoords[i][1] << std::endl;*/

	/*std::cerr << "bla" << std::endl;
	for(unsigned int i = 0; i < meshes.size(); i++){
		for(unsigned int j = 0; j < meshes[i]->vertices.size(); j++){
			std::cerr << "mesh: " << j << " vertices: " << meshes[i]->vertices[j][0] << " " << meshes[i]->vertices[j][1] << " " << meshes[i]->vertices[j][2] << std::endl;
		}
	}*/

	std::cerr << "loaded successfully: " << file << std::endl;
	return true;
}

bool model::load_mtl(std::string file){
	//std::cerr << "loading mtl: " << file << std::endl;
	std::ifstream in(file);
	if(!in.is_open()){
		std::cerr << "failed to load mtl: " << file << std::endl;
		return false;
	}
	std::string line;
	obj_material *mat = NULL;

	while(getline(in, line)){
		if(line.substr(0, 6) == "newmtl"){
			if(mat != NULL) materials.push_back(mat);
			mat = new obj_material;
			mat->name = line.substr(7);
			//std::cerr << mat->name << std::endl;
		}
		else if(line.substr(0, 2) == "Kd"){
			sscanf(line.c_str(), "Kd %f %f %f", &mat->color_diffuse[0], &mat->color_diffuse[1], &mat->color_diffuse[2]);
		}
		else if(line.substr(0, 6) == "map_Kd"){
			std::string tex_loc = file;
			for(unsigned int i = tex_loc.size() - 1; i > 0; i--){
				if(tex_loc[i] == '/') break;
				else tex_loc.erase(tex_loc.begin() + i);
			}
			tex_loc.append(line.substr(7));
			mat->load_texture(tex_loc);
		}
		else{
			//implement later
		}
	}
	if(mat != NULL) materials.push_back(mat);

	//std::cerr << "materials_size: " << materials.size() << std::endl;
	std::cerr << "loaded successfully mtl: " << file << std::endl;
	return true;
}

unsigned int model::get_mtl_index(std::string name){
	unsigned int index = 0;
	for(unsigned int i = 0; i < materials.size(); i++){
		if(materials[i]->name == name){
			index = i;
			break;
		}
	}
	//std::cerr << "index: " << index << " " << materials.size() << std::endl;
	return index;
}

//###############################################################  transformation matrix
glm::mat4 model::get_trans_mat(){
	return trans;
}

void model::set_trans_mat(glm::mat4 mat){
	trans = mat;
	return;
}

//###############################################################  move
glm::mat4 model::move(glm::vec3 mov){
	trans *= glm::translate(glm::mat4(1.f), mov);
	return trans;
}

glm::mat4 model::rotate(float degrees, glm::vec3 axis){
	trans *= glm::rotate(glm::mat4(1.f), degrees, axis);
	return trans;
}

glm::mat4 model::scale(glm::vec3 scale){
	trans *= glm::scale(glm::mat4(1.f), scale);
	return trans;
}

//###############################################################  render
void model::render(){
	for(unsigned int i = 0; i < meshes.size(); i++){
		if(meshes[i]->has_vertices){
			//
		}
	}
	return;
}


}
















