/*
 * object.h
 *
 *  Created on: Dec 3, 2014
 *      Author: nbingham
 */

#include "core/geometry.h"
#include "standard.h"
#include "opengl.h"

#include "material.h"

using namespace core;

#ifndef object_h
#define object_h

struct lighthdl;

/* This represents a rigid body, which
 * is just a group of geometry to be
 * rendered together. Its grouped in
 * this way so that you can apply different
 * materials to different parts of the
 * same model.
 */
struct rigidhdl
{
	rigidhdl();
	~rigidhdl();

	vector<vec8f> geometry;
	vector<int> indices;
	string material;

	void draw();
};

struct objecthdl
{
	objecthdl();
	objecthdl(const objecthdl &o);
	virtual ~objecthdl();

	vector<rigidhdl> rigid;
	map<string, materialhdl*> material;

	vec3f position;
	vec3f orientation;
	float scale;

	// The bounding box of this object
	// (left, right, bottom, top, front, back)
	vec6f bound;

	void draw(const vector<lighthdl*> &lights);
	void draw_bound();
	void draw_normals(bool face = false);
};

#endif
