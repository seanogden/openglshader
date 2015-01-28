/*
 * light.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: nbingham
 */

#include "light.h"
#include "object.h"

lighthdl::lighthdl()
{
	model = NULL;
	type = "light";
}

lighthdl::lighthdl(const vec3f &ambient, const vec3f &diffuse, const vec3f &specular)
{
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	model = NULL;
	type = "light";
}

lighthdl::~lighthdl()
{

}

directionalhdl::directionalhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	type = "directional";
}

directionalhdl::directionalhdl(const vec3f &direction, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	type = "directional";
}

directionalhdl::~directionalhdl()
{

}

void directionalhdl::update(mat4f modelview, mat4f projection)
{
	if (model != NULL)
		direction = rol3(vec3f(0.0, 0.0, 1.0), model->orientation);
}

void directionalhdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, mat4f modelview, mat4f projection, vec3f vertex, vec3f normal, float shininess) const
{
	vec3f eye = modelview.col(3)(0,3);

	vec3f ndirection = norm(direction);
	vec3f half_vector = norm(ndirection + norm(-vertex));

	float ndotdir = max(0.0f, dot(normal, ndirection));
	float ndothv = max(0.0f, dot(normal, half_vector));

	float power_factor;
	if (ndotdir == 0.0)
		power_factor = 0.0;
	else
		power_factor = pow(ndothv, shininess);

	ambient += this->ambient;
	diffuse += this->diffuse*ndotdir;
	specular += this->specular*power_factor;
}

pointhdl::pointhdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	this->attenuation = vec3f(1.0, 0.14, 0.7);
	type = "point";
}

pointhdl::pointhdl(const vec3f &position, const vec3f &attenuation, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	this->attenuation = attenuation;
	type = "point";
}

pointhdl::~pointhdl()
{

}

void pointhdl::update(mat4f modelview, mat4f projection)
{
	if (model != NULL)
	{
		vec4f p = projection*modelview*homogenize(model->position);
		position = p/p[3];
	}
}

void pointhdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, mat4f modelview, mat4f projection, vec3f vertex, vec3f normal, float shininess) const
{
	vec3f eye = modelview.col(3)(0,3);

	vec3f vp = position - vertex;
	float d = mag(vp);
	vp /= d;

	float att = 1.0/(attenuation[0] + attenuation[1]*d + attenuation[2]*d*d);

	vec3f half_vector = norm(vp + norm(-vertex));

	float ndotvp = max(0.0f, dot(normal, vp));
	float ndothv = max(0.0f, dot(normal, half_vector));

	float power_factor = 0.0;
	if (ndotvp != 0.0)
		power_factor = pow(ndothv, shininess);

	ambient += this->ambient*att;
	diffuse += this->diffuse*ndotvp*att;
	specular += this->specular*power_factor*att;
}

spothdl::spothdl() : lighthdl(white*0.1f, white*0.5f, white)
{
	this->attenuation = vec3f(1.0, 0.14, 0.7);
	this->cutoff = 0.5;
	this->exponent = 1.0;
	type = "spot";
}

spothdl::spothdl(const vec3f &attenuation, const float &cutoff, const float &exponent, const vec3f &ambient, const vec3f &diffuse, const vec3f &specular) : lighthdl(ambient, diffuse, specular)
{
	this->attenuation = attenuation;
	this->cutoff = cutoff;
	this->exponent = exponent;
	type = "spot";
}

spothdl::~spothdl()
{

}

void spothdl::update(mat4f modelview, mat4f projection)
{
	if (model != NULL)
	{
		direction = rol3(vec3f(0.0, 0.0, 1.0), -model->orientation);
		vec4f p = projection*modelview*homogenize(model->position);
		position = p/p[3];
	}
}

void spothdl::shade(vec3f &ambient, vec3f &diffuse, vec3f &specular, mat4f modelview, mat4f projection, vec3f vertex, vec3f normal, float shininess) const
{
	vec3f eye = modelview.col(3)(0,3);

	vec3f vp = position - vertex;
	float d = mag(vp);
	vp /= d;

	float att = 1.0/(attenuation[0] + attenuation[1]*d + attenuation[2]*d*d);
	float spotdot = dot(-vp, direction);

	float spotatt = 0.0;
	if (spotdot >= cutoff)
		spotatt = pow(spotdot, exponent);

	att *= spotatt;

	vec3f half_vector = norm(vp + norm(-vertex));
	float ndotvp = max(0.0f, dot(normal, vp));
	float ndothv = max(0.0f, dot(normal, half_vector));

	float power_factor = 0.0;
	if (ndotvp > 0)
		power_factor = pow(ndothv, shininess);

	ambient += this->ambient*att;
	diffuse += this->diffuse*ndotvp*att;
	specular += this->specular*power_factor*att;
}
