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

	// Set up the modelview matrix
    if (model != NULL)
    {
        glTranslatef(model->position[0], model->position[1], model->position[2]);
        glRotatef(radtodeg(model->orientation[0]), 1.0, 0.0, 0.0);
        glRotatef(radtodeg(model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(model->orientation[2]), 0.0, 0.0, 1.0);

        GLfloat m[16]; 
        glGetFloatv (GL_MODELVIEW_MATRIX, m);
        mat4f mv = mat4f(   m[0], m[1], m[2], m[3],
                            m[4], m[5], m[6], m[7],
                            m[8], m[9], m[10], m[11],
                            m[12], m[13], m[14], m[15]);

		direction = transpose(inverse(mv))*vec4f(0.0, 0.0, -1.0, 0.0);

        glRotatef(radtodeg(-model->orientation[2]), 0.0, 0.0, 1.0);
        glRotatef(radtodeg(-model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(-model->orientation[0]), 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0], -model->position[1], -model->position[2]);
    }
}

void directionalhdl::apply(string name, GLuint program)
{
    GLuint loc = glGetUniformLocation(program, (name + "ambient").c_str());
    glUniform3f(loc, ambient[0], ambient[1], ambient[2]);

    loc = glGetUniformLocation(program, (name + "diffuse").c_str());
    glUniform3f(loc, diffuse[0], diffuse[1], diffuse[2]);

    loc = glGetUniformLocation(program, (name + "specular").c_str());
    glUniform3f(loc, specular[0], specular[1], specular[2]);

    loc = glGetUniformLocation(program, (name + "direction").c_str());
    glUniform3f(loc, direction[0], direction[1], direction[2]);
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
	if (model != NULL)
	{
        glTranslatef(model->position[0], model->position[1], model->position[2]);
        glRotatef(radtodeg(model->orientation[0]), 1.0, 0.0, 0.0);
        glRotatef(radtodeg(model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(model->orientation[2]), 0.0, 0.0, 1.0);

        mat4f mv;
        glGetFloatv(GL_TRANSPOSE_MODELVIEW_MATRIX, (float*)mv.data);

		vec4f p = mv*vec4f(0.0, 0.0, 0.0, 1.0);
		position = p(0,3)/p[3];

        glRotatef(radtodeg(-model->orientation[2]), 0.0, 0.0, 1.0);
        glRotatef(radtodeg(-model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(-model->orientation[0]), 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0], -model->position[1], -model->position[2]);
	}
}

void pointhdl::apply(string name, GLuint program)
{
    GLuint loc = glGetUniformLocation(program, (name + "ambient").c_str());
    glUniform3f(loc, ambient[0], ambient[1], ambient[2]);

    loc = glGetUniformLocation(program, (name + "diffuse").c_str());
    glUniform3f(loc, diffuse[0], diffuse[1], diffuse[2]);

    loc = glGetUniformLocation(program, (name + "specular").c_str());
    glUniform3f(loc, specular[0], specular[1], specular[2]);

    loc = glGetUniformLocation(program, (name + "attenuation").c_str());
    glUniform3f(loc, attenuation[0], attenuation[1], attenuation[2]);

    loc = glGetUniformLocation(program, (name + "position").c_str());
    glUniform3f(loc, position[0], position[1], position[2]);

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
	if (model != NULL)
	{
        glTranslatef(model->position[0], model->position[1], model->position[2]);
        glRotatef(radtodeg(model->orientation[0]), 1.0, 0.0, 0.0);
        glRotatef(radtodeg(model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(model->orientation[2]), 0.0, 0.0, 1.0);

        /* TODO: update position
		vec4f p = canvas->matrices[canvashdl::modelview_matrix]*vec4f(0.0, 0.0, 0.0, 1.0);
		position = p(0,3)/p[3];
		canvas->update_normal_matrix();
		direction = canvas->matrices[canvashdl::normal_matrix]*vec4f(0.0, 0.0, -1.0, 0.0);
        */

        glRotatef(radtodeg(model->orientation[2]), 0.0, 0.0, 1.0);
        glRotatef(radtodeg(model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(model->orientation[0]), 1.0, 0.0, 0.0);
        glTranslatef(-model->position[0], -model->position[1], -model->position[2]);
	}
}

void spothdl::apply(string name, GLuint program)
{
	/* TODO Assignment 4: Pass all necessary uniforms to the shaders for spot lights.
	 */
}
