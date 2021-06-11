/*
 * SmartTexture.h
 * CSC 474 Cal Poly, Shawn Harris, 2021
 * 
 */
#include "SmartTexture.h"
#include <iostream>
#include <cassert>

#define NOMINMAX  
#include <limits>
#include <cstddef>

#include "GLSL.h"
#include "stb_image.h"

using namespace std;
using namespace glm;

GLuint SmartTexture::gUnit = 0;

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb_image.h"

shared_ptr<SmartTexture> SmartTexture::createTexture(const std::string& filename)
{
	auto texture = make_shared<SmartTexture>(filename, gUnit);
	gUnit++;
	return texture;
}


SmartTexture::SmartTexture(const string &filename, GLuint id)
{
	unit = id;
	init(filename);
}


SmartTexture::~SmartTexture()
{
	
}

unsigned char* gettextureData(char *filename, int& width, int& height, int& channels)
	{
	unsigned char* l_data = stbi_load(filename, &width, &height, &channels, 4);
	return l_data;
	}

void SmartTexture::init(const string &filename)
{
	// Load texture
	int w, h, ncomps;
	stbi_set_flip_vertically_on_load(true);
	
	data = stbi_load(filename.c_str(), &w, &h, &ncomps, 0);
	if(!data) {
		cerr << "texture " << filename << " not found" << endl;
		exit(0);
	}
	if(ncomps != 3) {
		cerr << filename << " must have 3 components (RGB)" << endl;
	}
	if((w & (w - 1)) != 0 || (h & (h - 1)) != 0) {
		cerr << filename << " must be a power of 2" << endl;
	}
	width = w;
	height = h;

	// Generate a texture buffer object
	glGenTextures(1, &unit);
	// Bind the current texture to be the newly generated texture object
	glBindTexture(GL_TEXTURE_2D, unit);
	// Load the actual texture data
	// Base level is 0, number of channels is 3, and border is 0.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	// Generate image pyramid
	glGenerateMipmap(GL_TEXTURE_2D);
	// Set texture wrap modes for the S and T directions
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set filtering mode for magnification and minimification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	// Free image, since the data is now on the GPU
	stbi_image_free(data);
}

void SmartTexture::setWrapModes(GLint wrapS, GLint wrapT)
{
	// Must be called after init()
	glBindTexture(GL_TEXTURE_2D, unit);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void SmartTexture::bind(GLint handle,GLuint location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, unit);
	//glUniform1i(handle, unit);
}

void SmartTexture::unbind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}



	
