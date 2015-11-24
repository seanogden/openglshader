/*
 * camera.cpp
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "camera.h"
#include "object.h"

camerahdl::camerahdl()
{
	position = vec3f(0.0, 0.0, 0.0);
	orientation = vec3f(0.0, 0.0, 0.0);
	model = NULL;
	type = "camera";
	focus = NULL;
	radius = 10.0f;
}

camerahdl::~camerahdl()
{

}

void camerahdl::view()
{
	glLoadIdentity();

	if (focus == NULL)
	{
        glRotatef(radtodeg(-model->orientation[0]), 1.0, 0.0, 0.0);
        glRotatef(radtodeg(-model->orientation[1]), 0.0, 1.0, 0.0);
        glRotatef(radtodeg(-model->orientation[2]), 0.0, 0.0, 1.0);
        glTranslatef(-position[0], -position[1], -position[2]);
	}
	else
	{
		position = focus->position + ror3(vec3f(0.0, 0.0, radius), orientation);
	    vec3f up = ror3(vec3f(0.0, 1.0, 0.0), orientation);
        gluLookAt(position[0], position[1], position[2],
                  focus->position[0], focus->position[1], focus->position[2],
                  up[0], up[1], up[2]);
	}

	if (model != NULL)
	{
		model->position = position;
		model->orientation = orientation;
		model->orientation[0] *= -1;
	}
}

orthohdl::orthohdl()
{
	left = -10.0;
	right = 10.0;
	bottom = -10.0;
	top = 10.0;
	front = 2.0;
	back = 101.0;
	type = "ortho";
}

orthohdl::~orthohdl()
{
}

void orthohdl::project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, front, back);
    glMatrixMode(GL_MODELVIEW);
}

frustumhdl::frustumhdl()
{
	left = -1.0;
	right = 1.0;
	bottom = -1.0;
	top = 1.0;
	front = 2.0;
	back = 101.0;
	type = "frustum";
}

frustumhdl::~frustumhdl()
{

}

void frustumhdl::project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(left, right, bottom, top, front, back);
    glMatrixMode(GL_MODELVIEW);
}

perspectivehdl::perspectivehdl()
{
	fovy = m_pi/4.0;
	aspect = 1.0;
	front = 2.0;
	back = 101.0;
	type = "perspective";
}

perspectivehdl::~perspectivehdl()
{

}

void perspectivehdl::project()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, aspect, front, back);
    glMatrixMode(GL_MODELVIEW);
}
