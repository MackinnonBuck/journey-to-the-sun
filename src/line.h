


#pragma once

#ifndef LAB474_LINE_H_INCLUDED
#define LAB474_LINE_H_INCLUDED

#include <string>
#include <vector>
#include <memory>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;
using namespace std;



class Program;

class Line
{

public:
	//stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
	bool init();
	bool re_init_line(const std::vector<vec3> &points);
	void draw(const mat4 &P, const mat4 &V, const vec3 &colorvec3);
	bool is_active();
	void reset();

private:
	int segment_count = 0;
	unsigned int posBufID = 0;	
	unsigned int vaoID;
	unsigned int pid;
	unsigned int ucolor,uP,uV;
};
void spline(vector<vec3> &result_path, const vector<vec3> &original_path, int lod);

#endif // LAB471_SHAPE_H_INCLUDED
