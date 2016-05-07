/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OpenGL.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/11/30 17:48:02 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/06 15:10:12 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "OpenGL.hpp"

int		OpenGL::_vertexAttrib = 0;
int		OpenGL::_textureAttrib = 0;

OpenGL::OpenGL()
{
	InitOpenGL();
}

OpenGL::OpenGL(OpenGL const &instance)
{
	*this = instance;
}

OpenGL::~OpenGL()
{
	if (_vertexAttrib != 0)
	{
		for (std::vector<GLuint *>::iterator it = vboID.begin(); it != vboID.end(); ++it)
			glDeleteBuffers(1, *it);
	}
	if (_textureAttrib != 0)
	{
		for (std::vector<GLuint *>::iterator it = texID.begin(); it != texID.end(); it ++)
			glDeleteTextures(1, *it);
	}
	glDeleteVertexArrays(1, &(this->_vaoID));
}

OpenGL&		OpenGL::operator=(OpenGL const &rhs)
{
	vboID = rhs.vboID;
	texID = rhs.texID;
	nbrV = rhs.nbrV;
	this->_vaoID = rhs._vaoID;
	return (*this);
}

void	OpenGL::InitOpenGL()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glGenVertexArrays(1, &(this->_vaoID));
	this->SetVAO();
}

void	OpenGL::SetVAO()
{
	glBindVertexArray(this->_vaoID);
}

GLuint	OpenGL::InitVBO(const std::vector<GLfloat> & vbo)
{
	GLuint	id = 0;
	int		bufferSize = 0;

	nbrV += vbo.size();
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	vboID.push_back(&id);
	glBufferData(GL_ARRAY_BUFFER,
		vbo.size() * sizeof(GLfloat),
		&vbo.front(),
		GL_DYNAMIC_DRAW);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	this->_vertexAttrib++;
	if (vbo.size() * sizeof(GLfloat) != (unsigned long)bufferSize)
	{
		glDeleteBuffers(1, &id);
		id = 0;
		vboID.pop_back();
		std::cout << "[InitVBO()] Data size is mismatch with input array" << std::endl;
		this->_vertexAttrib--;
	}
	return (id);
}

GLuint	OpenGL::InitVBO(const glm::vec4 *vbo, int size)
{
	GLuint	id = 0;
	int		bufferSize = 0;

	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	vboID.push_back(&id);
	glBufferData(GL_ARRAY_BUFFER,
		size,
		vbo,
		GL_DYNAMIC_DRAW);
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
	if (size != bufferSize)
	{
		glDeleteBuffers(1, &id);
		id = 0;
		vboID.pop_back();
		std::cout << "[InitVBO()] Data size is mismatch with input array" << std::endl;
	}
	glVertexAttribPointer(this->_vertexAttrib, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(this->_vertexAttrib);
	this->_vertexAttrib++;
	return (id);
}

void	OpenGL::InitVBO(const std::vector<GLfloat> & vbo, int dimension)
{
	GLuint id;

	nbrV += vbo.size();
	if (!vbo.empty())
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		vboID.push_back(&id);
		glBufferData(GL_ARRAY_BUFFER,
			vbo.size() * sizeof(GLfloat),
			&vbo.front(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(this->_vertexAttrib, dimension, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(this->_vertexAttrib);		
		this->_vertexAttrib++;
	}
}

void	OpenGL::InitVBO(const std::vector<glm::vec3> & vbo, int dimension)
{
	GLuint id;

	nbrV += vbo.size();
	if (!vbo.empty())
	{
		glGenBuffers(1, &id);
		glBindBuffer(GL_ARRAY_BUFFER, id);
		vboID.push_back(&id);
		glBufferData(GL_ARRAY_BUFFER,
			vbo.size() * sizeof(glm::vec3),
			&vbo.front(),
			GL_STATIC_DRAW);
		glVertexAttribPointer(this->_vertexAttrib, dimension, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(this->_vertexAttrib);		
		this->_vertexAttrib++;
	}
}

void	OpenGL::GetUniform(GLuint progID, std::string name)
{
	std::map<std::string, GLint>::iterator	it;

	it = this->_uniform.find(name);
	if (it == this->_uniform.end())
		this->_uniform[name] = glGetUniformLocation(progID, name.c_str());
}

void	OpenGL::SetUniform(std::string name, GLfloat content)
{
	glUniform1f(this->_uniform[name], content);
}

void	OpenGL::SetUniform(std::string name, glm::vec3 content)
{
	glUniform3fv(this->_uniform[name], 1, glm::value_ptr(content));
}

void	OpenGL::SetUniform(std::string name, glm::mat4 content)
{
	glUniformMatrix4fv(this->_uniform[name], 1, GL_FALSE, glm::value_ptr(content));
}

void	OpenGL::UseProgram(GLuint progID)
{
	glUseProgram(progID);
}

void	OpenGL::CullFace(GLenum enabled, GLenum cullFace, GLenum frontFace)
{
	glEnable(enabled);
	glCullFace(cullFace);
	glFrontFace(frontFace);
}

void	OpenGL::ClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	glClearColor (r, g, b, a);
}

void	OpenGL::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void	OpenGL::ViewPort(GLsizei width, GLsizei height)
{
	glViewport(0, 0, width, height);
}

void	OpenGL::ParameterPoints(GLfloat size)
{
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(size);
}

void	OpenGL::DrawTriangle(int nbr)
{
	glDrawArrays(GL_TRIANGLES, 0, nbr);
}

void	OpenGL::DrawPoints(int nbr)
{
	glDrawArrays(GL_POINTS, 0, nbr);
	glDisable(GL_BLEND);
}

void	OpenGL::DrawLines(int nbr)
{
	glDrawArrays(GL_LINES, 0, nbr);
}
/*
void	OpenGL::InitTexture(Texture *gltex)
{
	GLfloat		max_aniso;
	GLuint		id;

	max_aniso = 0.0f;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + _textureAttrib);
	glBindTexture(GL_TEXTURE_2D, id);
	texID.push_back(&id);
	glTexImage2D(GL_TEXTURE_2D, 0, gltex->format,
				gltex->width, gltex->height, 0, gltex->format,
				GL_UNSIGNED_BYTE, gltex->texels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
}
*/

void	OpenGL::UpdateTexture()
{
	int		position = 0;

	for (std::vector<GLuint *>::iterator it = texID.begin(); it != texID.end(); it++) {
		glGenTextures(1, *it);
		glActiveTexture(GL_TEXTURE0 + position);
		position++;
	}
}

void	OpenGL::InitTexture(const char *nameTex)
{
	GLfloat			max_aniso;
	GLuint			id;
	int				x, y, n;

	unsigned char *img = stbi_load(nameTex, &x, &y, &n, 4);
	if (!img) {
		fprintf (stderr, "ERROR: could not load %s\n", nameTex);
		exit(-1);
	}
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf (
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", nameTex
		);
	}
	int width_in_bytes = x * 4;
	unsigned char *top = NULL;
	unsigned char *bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {
		top = img + row * width_in_bytes;
		bottom = img + (y - row - 1) * width_in_bytes;
		for (int col = 0; col < width_in_bytes; col++) {
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}
	max_aniso = 0.0f;
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0 + _textureAttrib);
	glBindTexture(GL_TEXTURE_2D, id);
	texID.push_back(&id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
				x, y, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, img);
	std::cout << x << " " << y << std::endl;
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
}

void	OpenGL::PrintVBOID()
{
	std::cout << "VBO ID :" << std::endl;
	for (std::vector<GLuint *>::iterator it = vboID.begin(); it != vboID.end(); ++it)
		std::cout << "\t" << **it << std::endl; 
}

void	OpenGL::PrintTexID()
{
	std::cout << "Texture ID :" << std::endl;
	for (std::vector<GLuint *>::iterator it = texID.begin(); it != texID.end(); it++)
		std::cout << "\t" << **it << std::endl;
}

void	OpenGL::Finish()
{
	glFinish();
}
