/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGL.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 15:23:22 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/05 18:24:00 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPENGL_HPP
# define OPENGL_HPP

# include <OpenGL/gl3.h>
# include <OpenGL/gl3ext.h>
# include <string>
# include <vector>
# include <map>
# include <iostream>
# include <unistd.h>
# include <glm/glm.hpp>
# include <glm/gtc/type_ptr.hpp>
# include "stb_image.hpp"

//# include "Texture.hpp"
# include "Camera.hpp"

class	OpenGL
{
public:
	std::vector<GLuint *>	vboID;
	std::vector<GLuint *>	texID;
	int						nbrV;

	OpenGL();
	~OpenGL();

	OpenGL&		operator=(OpenGL const &rhs);

	void	InitOpenGL();
	void	SetVAO();
	GLuint	InitVBO(const glm::vec4 *vbo, int size);
	GLuint	InitVBO(const std::vector<GLfloat> & vbo);
	void	InitVBO(const std::vector<GLfloat> & vbo, int dimension);
	void	InitVBO(const std::vector<glm::vec3> & vbo, int dimension);
	void	GetUniform(GLuint progID, std::string name);
	void	SetUniform(std::string name, GLfloat content);
	void	SetUniform(std::string name, glm::vec3 content);
	void	SetUniform(std::string name, glm::mat4 content);
	void	UseProgram(GLuint progID);
	void	CullFace(GLenum enabled, GLenum cullFace, GLenum frontFace);
	void	ClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a);
	void	Clear();
	void	ViewPort(GLsizei width, GLsizei height);
	void	ParameterPoints(GLfloat size);
	void	DrawTriangle(int nbr);
	void	DrawPoints(int nbr);
	void	DrawLines(int nbr);
//	void	InitTexture(Texture *gltex);
	void	InitTexture(const char *nameTex);
	void	UpdateTexture();
	void	PrintVBOID();
	void	PrintTexID();
	void	Finish();

private:
	GLuint							_vaoID;
	std::map<std::string, GLint>	_uniform;
	static int						_vertexAttrib;
	static int						_textureAttrib;

	OpenGL(OpenGL const &);
	
};

#endif