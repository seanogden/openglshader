/*
 * material.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: nbingham
 */

#include "material.h"
#include "light.h"
#include "lodepng.h"

GLuint whitehdl::vertex = 0;
GLuint whitehdl::fragment = 0;
GLuint whitehdl::program = 0;

GLuint gouraudhdl::vertex = 0;
GLuint gouraudhdl::fragment = 0;
GLuint gouraudhdl::program = 0;

GLuint phonghdl::vertex = 0;
GLuint phonghdl::fragment = 0;
GLuint phonghdl::program = 0;

GLuint customhdl::vertex = 0;
GLuint customhdl::fragment = 0;
GLuint customhdl::program = 0;

GLuint texturehdl::vertex = 0;
GLuint texturehdl::fragment = 0;
GLuint texturehdl::program = 0;
GLuint texturehdl::texture = 0;

extern string working_directory;

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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
        //load shaders
        vertex = load_shader_file(working_directory + "res/white.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file(working_directory + "res/white.ft", GL_FRAGMENT_SHADER);

        //link shaders
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
	}
}

whitehdl::~whitehdl()
{

}

void whitehdl::apply(const vector<lighthdl*> &lights)
{
	glUseProgram(program);
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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
        //load shaders
        vertex = load_shader_file(working_directory + "res/gouraud.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file(working_directory + "res/gouraud.ft", GL_FRAGMENT_SHADER);

        //link shaders
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
	}
}

gouraudhdl::~gouraudhdl()
{

}

void gouraudhdl::apply(const vector<lighthdl*> &lights)
{
	glUseProgram(program);

	int emission_location = glGetUniformLocation(program, "emission");
	int ambient_location = glGetUniformLocation(program, "ambient");
	int diffuse_location = glGetUniformLocation(program, "diffuse");
	int specular_location = glGetUniformLocation(program, "specular");
	int shininess_location = glGetUniformLocation(program, "shininess");
	glUniform3f(emission_location, emission[0], emission[1], emission[2]);
	glUniform3f(ambient_location, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_location, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_location, specular[0], specular[1], specular[2]);
	glUniform1f(shininess_location, shininess);

    int dlights = 0;
    int slights = 0;
    int plights = 0;

    //Send light structs to shader uniforms
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        std::stringstream name;

        if (lights[i]->type.compare("directional") == 0)
        {
            name << "dlights[" << dlights << "].";
            dlights++;
        }
        else if (lights[i]->type.compare("spot") == 0)
        {
            name << "slights[" << slights << "].";
            slights++;
        }
        else
        {
            name << "plights[" << plights << "].";
            plights++;
        }

        lights[i]->apply(name.str(), program);
    }

    
    //Send counts of lights to shader uniforms.
    GLuint loc = glGetUniformLocation(program, "num_dlights");
    glUniform1i(loc, dlights);
    loc = glGetUniformLocation(program, "num_slights");
    glUniform1i(loc, slights);
    loc = glGetUniformLocation(program, "num_plights");
    glUniform1i(loc, plights);

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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
        std::cout << "loading phong" << std::endl;
        //load shaders
        vertex = load_shader_file(working_directory + "res/phong.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file(working_directory + "res/phong.ft", GL_FRAGMENT_SHADER);

        //link shaders
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
	}

}

phonghdl::~phonghdl()
{

}

void phonghdl::apply(const vector<lighthdl*> &lights)
{
	// TODO Assignment 4: Apply the shader program and pass it the necessary uniform values
	glUseProgram(program);

	int emission_location = glGetUniformLocation(program, "emission");
	int ambient_location = glGetUniformLocation(program, "ambient");
	int diffuse_location = glGetUniformLocation(program, "diffuse");
	int specular_location = glGetUniformLocation(program, "specular");
	int shininess_location = glGetUniformLocation(program, "shininess");
	glUniform3f(emission_location, emission[0], emission[1], emission[2]);
	glUniform3f(ambient_location, ambient[0], ambient[1], ambient[2]);
	glUniform3f(diffuse_location, diffuse[0], diffuse[1], diffuse[2]);
	glUniform3f(specular_location, specular[0], specular[1], specular[2]);
	glUniform1f(shininess_location, shininess);

    int dlights = 0;
    int slights = 0;
    int plights = 0;

    //Send light structs to shader uniforms
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        std::stringstream name;

        if (lights[i]->type.compare("directional") == 0)
        {
            name << "dlights[" << dlights << "].";
            dlights++;
        }
        else if (lights[i]->type.compare("spot") == 0)
        {
            name << "slights[" << slights << "].";
            slights++;
        }
        else
        {
            name << "plights[" << plights << "].";
            plights++;
        }

        lights[i]->apply(name.str(), program);
    }

    
    //Send counts of lights to shader uniforms.
    GLuint loc = glGetUniformLocation(program, "num_dlights");
    glUniform1i(loc, dlights);
    loc = glGetUniformLocation(program, "num_slights");
    glUniform1i(loc, slights);
    loc = glGetUniformLocation(program, "num_plights");
    glUniform1i(loc, plights);
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

	if (vertex == 0 && fragment == 0 && program == 0)
	{
		/* TODO Assignment 4: Load and link the shaders. Keep in mind that vertex, fragment, 
		 * and program are static variables meaning they are *shared across all instances of
		 * this class. So you only have to initialize them once when the first instance of
		 * the class is created.
		 */
	}
}

customhdl::~customhdl()
{

}

void customhdl::apply(const vector<lighthdl*> &lights)
{
}

materialhdl *customhdl::clone() const
{
	customhdl *result = new customhdl();
	result->type = type;
	return result;
}

texturehdl::texturehdl()
{
	type = "texture";

	shininess = 1.0;
    


	if (vertex == 0 && fragment == 0 && program == 0)
	{
        glEnable(GL_TEXTURE_2D);
        std::cout << "loading texture" << std::endl;
        unsigned int width;
        unsigned int height;
        unsigned char* image;

        unsigned int error = lodepng_decode32_file(&image, &width, &height, (working_directory + "res/texture.png").c_str());

        if(error) 
        {
            std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
            return;
        }

        //set give the working texture an ID 
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Set texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glBindTexture(GL_TEXTURE_2D, 0);

        std::cout << "loading texture shaders" << std::endl;
        //load shaders
        vertex = load_shader_file(working_directory + "res/texture.vx", GL_VERTEX_SHADER);
        fragment = load_shader_file(working_directory + "res/texture.ft", GL_FRAGMENT_SHADER);

        //link shaders
        program = glCreateProgram();
        glAttachShader(program, vertex);
        glAttachShader(program, fragment);
        glLinkProgram(program);
	}

}

texturehdl::~texturehdl()
{
}

void texturehdl::apply(const vector<lighthdl*> &lights)
{
	glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    int tex_location = glGetUniformLocation(program, "tex");
    glUniform1i(tex_location, GL_TEXTURE0);
    int shininess_location = glGetUniformLocation(program, "shininess");
	glUniform1f(shininess_location, shininess);

    int dlights = 0;
    int slights = 0;
    int plights = 0;

    //Send light structs to shader uniforms
    for (unsigned int i = 0; i < lights.size(); ++i)
    {
        std::stringstream name;

        if (lights[i]->type.compare("directional") == 0)
        {
            name << "dlights[" << dlights << "].";
            dlights++;
        }
        else if (lights[i]->type.compare("spot") == 0)
        {
            name << "slights[" << slights << "].";
            slights++;
        }
        else
        {
            name << "plights[" << plights << "].";
            plights++;
        }

        lights[i]->apply(name.str(), program);
    }

    
    //Send counts of lights to shader uniforms.
    GLuint loc = glGetUniformLocation(program, "num_dlights");
    glUniform1i(loc, dlights);
    loc = glGetUniformLocation(program, "num_slights");
    glUniform1i(loc, slights);
    loc = glGetUniformLocation(program, "num_plights");
    glUniform1i(loc, plights);

}

materialhdl *texturehdl::clone() const
{
	texturehdl *result = new texturehdl();
	result->type = type;
	result->shininess = shininess;
	return result;
}
