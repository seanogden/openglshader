/*
 * material.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "material.h"

materialhdl::materialhdl()
{
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(1.0, 1.0, 1.0);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

materialhdl::~materialhdl()
{

}

vec3f materialhdl::color(vec3f light_ambient, vec3f light_diffuse, vec3f light_specular) const
{
	return emission + light_ambient*ambient + diffuse*light_diffuse + specular*light_specular;
}
