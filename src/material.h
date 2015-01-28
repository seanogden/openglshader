/*
 * material.h
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "core/geometry.h"

using namespace core;

#ifndef material_h
#define material_h

struct materialhdl
{
	materialhdl();
	~materialhdl();

	vec3f emission;
	vec3f ambient;
	vec3f diffuse;
	vec3f specular;
	float shininess;

	vec3f color(vec3f light_ambient, vec3f light_diffuse, vec3f light_specular) const;
};

#endif
