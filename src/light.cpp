/*
 * light.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: nbingham
 */

#include "light.h"
#include "object.h"
#include "opengl.h"

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

void directionalhdl::update()
{
	if (canvas != NULL && model != NULL)
	{
		canvas->translate(model->position);
		canvas->rotate(model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->rotate(model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(model->orientation[2], vec3f(0.0, 0.0, 1.0));

		canvas->update_normal_matrix();
		direction = canvas->matrices[canvashdl::normal_matrix]*vec4f(0.0, 0.0, -1.0, 0.0);

		canvas->rotate(-model->orientation[2], vec3f(0.0, 0.0, 1.0));
		canvas->rotate(-model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(-model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->translate(-model->position);
	}
}

void directionalhdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for the directional light.
	 */
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

void pointhdl::update()
{
	if (canvas != NULL && model != NULL)
	{
		canvas->translate(model->position);
		canvas->rotate(model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->rotate(model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(model->orientation[2], vec3f(0.0, 0.0, 1.0));

		vec4f p = canvas->matrices[canvashdl::modelview_matrix]*vec4f(0.0, 0.0, 0.0, 1.0);
		position = p(0,3)/p[3];

		canvas->rotate(-model->orientation[2], vec3f(0.0, 0.0, 1.0));
		canvas->rotate(-model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(-model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->translate(-model->position);
	}
}

void pointhdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for point lights.
	 */
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

void spothdl::update()
{
	if (canvas != NULL && model != NULL)
	{
		canvas->translate(model->position);
		canvas->rotate(model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->rotate(model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(model->orientation[2], vec3f(0.0, 0.0, 1.0));

		vec4f p = canvas->matrices[canvashdl::modelview_matrix]*vec4f(0.0, 0.0, 0.0, 1.0);
		position = p(0,3)/p[3];
		canvas->update_normal_matrix();
		direction = canvas->matrices[canvashdl::normal_matrix]*vec4f(0.0, 0.0, -1.0, 0.0);

		canvas->rotate(-model->orientation[2], vec3f(0.0, 0.0, 1.0));
		canvas->rotate(-model->orientation[1], vec3f(0.0, 1.0, 0.0));
		canvas->rotate(-model->orientation[0], vec3f(1.0, 0.0, 0.0));
		canvas->translate(-model->position);
	}
}

void spothdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for spot lights.
	 */
}
