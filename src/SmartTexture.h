/*
 * SmartTexture.h
 * CSC 474 Cal Poly, Shawn Harris, 2021
 * 
 */
#pragma once
#ifndef SMART_TEXTURE_H
#define SMART_TEXTURE_H

#include <string>
#include <vector>
#include <memory>
#include <map>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "SmartTexture.h"


unsigned char* gettextureData(char* filename, int& width, int& height, int& channels);


class SmartTexture
{
public:
	SmartTexture(const std::string &filename, GLuint id);
	virtual ~SmartTexture();
	
	void bind(GLint handle, GLuint location);
	void unbind();
	void setWrapModes(GLint wrapS, GLint wrapT); // Must be called after init()
	static std::shared_ptr<SmartTexture> createTexture(const std::string &filename);

protected:
	void init(const std::string& filename);

	int width;
	int height;
	GLuint unit;
	unsigned char* data;
	
	static GLuint gUnit;
};

#endif // SMART_TEXTURE_H
