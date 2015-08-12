/*
 * material.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "material.h"
#include "canvas.h"
#include "light.h"

materialhdl::materialhdl()
{
	type = "material";
}

materialhdl::~materialhdl()
{
}

whitehdl::whitehdl()
{
	type = "white";
}

whitehdl::~whitehdl()
{

}

vec3f whitehdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	vec4f eye_space_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);
	eye_space_vertex = canvas->matrices[canvashdl::projection_matrix]*eye_space_vertex;
	eye_space_vertex /= eye_space_vertex[3];
	return eye_space_vertex;
}

vec3f whitehdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	return vec3f(1.0, 1.0, 1.0);
}

materialhdl *whitehdl::clone() const
{
	whitehdl *result = new whitehdl();
	result->type = type;
	return result;
}

gouraudhdl::gouraudhdl()
{
	type = "gouraud";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

gouraudhdl::~gouraudhdl()
{

}

/* This is the vertex shader for a uniform material. The vertex and normal are in world coordinates, and
 * you have write access to the varying array which is used to pass data to the fragment shader. Don't
 * forget that this data is interpolated before it reaches the fragment shader.
 */
vec3f gouraudhdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	vec4f eye_space_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);

	const vector<lighthdl*> *lights;

	canvas->get_uniform("lights", lights);

	vec3f color(1.0, 1.0, 1.0);
	if (mag2(normal) > 0.0)
	{
		vec3f eye_space_normal = canvas->matrices[canvashdl::normal_matrix]*(vec4f)normal;

		vec3f light_ambient(0.0, 0.0, 0.0);
		vec3f light_diffuse(0.0, 0.0, 0.0);
		vec3f light_specular(0.0, 0.0, 0.0);

		for (int j = 0; j < lights->size(); j++)
			if (lights->at(j) != NULL)
				lights->at(j)->shade(light_ambient, light_diffuse, light_specular, eye_space_vertex, eye_space_normal, shininess);

		color = clamp(emission + ambient*light_ambient + diffuse*light_diffuse + specular*light_specular, 0.0f, 1.0f);
	}

	varying.push_back(color[0]);
	varying.push_back(color[1]);
	varying.push_back(color[2]);

	eye_space_vertex = canvas->matrices[canvashdl::projection_matrix]*eye_space_vertex;
	eye_space_vertex /= eye_space_vertex[3];
	return eye_space_vertex;
}

vec3f gouraudhdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	return vec3f(varying[0], varying[1], varying[2]);
}

materialhdl *gouraudhdl::clone() const
{
	gouraudhdl *result = new gouraudhdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

phonghdl::phonghdl()
{
	type = "phong";
	emission = vec3f(0.0, 0.0, 0.0);
	ambient = vec3f(0.1, 0.1, 0.1);
	diffuse = vec3f(1.0, 1.0, 1.0);
	specular = vec3f(1.0, 1.0, 1.0);
	shininess = 1.0;
}

phonghdl::~phonghdl()
{

}

/* This is the vertex shader for a uniform material. The vertex and normal are in world coordinates, and
 * you have write access to the varying array which is used to pass data to the fragment shader. Don't
 * forget that this data is interpolated before it reaches the fragment shader.
 */
vec3f phonghdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	vec4f eye_space_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);

	for (int i = 0; i < 3; i++)
		varying.push_back(vertex[i]);
	for (int i = 0; i < 3; i++)
		varying.push_back(normal[i]);

	eye_space_vertex = canvas->matrices[canvashdl::projection_matrix]*eye_space_vertex;
	eye_space_vertex /= eye_space_vertex[3];
	return eye_space_vertex;
}

vec3f phonghdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	vec3f vertex = vec3f(varying[0], varying[1], varying[2]);
	vec3f normal = vec3f(varying[3], varying[4], varying[5]);

	varying.erase(varying.begin(), varying.begin() + 6);

	vec3f light_ambient(0.0, 0.0, 0.0);
	vec3f light_diffuse(0.0, 0.0, 0.0);
	vec3f light_specular(0.0, 0.0, 0.0);

	const vector<lighthdl*> *lights;
	canvas->get_uniform("lights", lights);

	vec3f eye_coord_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);
	vec3f eye_coord_normal = canvas->matrices[canvashdl::normal_matrix]*(vec4f)normal;

	float m = mag2(eye_coord_normal);

	if (m > 0.0)
	{
		eye_coord_normal /= sqrt(m);
		for (int j = 0; j < lights->size(); j++)
			if (lights->at(j) != NULL)
				lights->at(j)->shade(light_ambient, light_diffuse, light_specular, eye_coord_vertex, eye_coord_normal, shininess);

		return clamp(emission + ambient*light_ambient + diffuse*light_diffuse + specular*light_specular, 0.0f, 1.0f);
	}

	return vec3f(1.0, 1.0, 1.0);
}

materialhdl *phonghdl::clone() const
{
	phonghdl *result = new phonghdl();
	result->type = type;
	result->emission = emission;
	result->ambient = ambient;
	result->diffuse = diffuse;
	result->specular = specular;
	result->shininess = shininess;
	return result;
}

customhdl::customhdl()
{
	type = "custom";
}

customhdl::~customhdl()
{

}

vec3f customhdl::shade_vertex(canvashdl *canvas, vec3f vertex, vec3f normal, vector<float> &varying) const
{
	vec4f eye_space_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);
	float mult = (float)(fmod(fabs(vertex[1] + 1000.0)*3.0, 1.0) > 0.5);
	vec3f scale(mult, 0.0f, 1.0-mult);

	for (int i = 0; i < 6; i++)
		varying.push_back(vertex[i]);

	eye_space_vertex = canvas->matrices[canvashdl::projection_matrix]*eye_space_vertex;
	eye_space_vertex /= eye_space_vertex[3];
	return eye_space_vertex;
}

vec3f customhdl::shade_fragment(canvashdl *canvas, vector<float> &varying) const
{
	vec3f vertex = vec3f(varying[0], varying[1], varying[2]);
	vec3f normal = vec3f(varying[3], varying[4], varying[5]);

	varying.erase(varying.begin(), varying.begin() + 6);

	float mult = (float)(fmod(fabs(vertex[1] + 1000.0)*3.0, 1.0) > 0.5);
	vec3f scale(mult, 0.0f, 1.0-mult);

	vec3f light_ambient(0.0, 0.0, 0.0);
	vec3f light_diffuse(0.0, 0.0, 0.0);
	vec3f light_specular(0.0, 0.0, 0.0);

	const vector<lighthdl*> *lights;
	canvas->get_uniform("lights", lights);

	vec3f eye_coord_vertex = canvas->matrices[canvashdl::modelview_matrix]*homogenize(vertex);
	vec3f eye_coord_normal = canvas->matrices[canvashdl::normal_matrix]*(vec4f)normal;

	float m = mag2(eye_coord_normal);

	if (m > 0.0)
	{
		eye_coord_normal /= sqrt(m);
		for (int j = 0; j < lights->size(); j++)
			if (lights->at(j) != NULL)
				lights->at(j)->shade(light_ambient, light_diffuse, light_specular, eye_coord_vertex, eye_coord_normal, 5.0);

		return clamp(scale*light_ambient + scale*light_diffuse + scale*light_specular, 0.0f, 1.0f);
	}

	return vec3f(1.0, 1.0, 1.0);
}

materialhdl *customhdl::clone() const
{
	customhdl *result = new customhdl();
	result->type = type;
	return result;
}
