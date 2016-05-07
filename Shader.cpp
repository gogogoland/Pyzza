/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 21:09:40 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/03/25 17:54:07 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Shader.hpp"

/*
**		Exception
*/

Shader::ShaderException::ShaderException(std::string const &error) : _msg(error)
{
}

Shader::ShaderException::~ShaderException() throw()
{}

const char 	*Shader::ShaderException::what() const throw()
{
	return (this->_msg.c_str());
}

/*
**		Class_Principal
*/

Shader::Shader() : _frag(NULL), _vert(NULL)
{
	std::cerr << "Error init shader" << std::endl;
}

Shader::Shader(std::string fragment, std::string vertex) :
					_frag(fragment), _vert(vertex)
{
	this->_contentFrag = this->_LoadShader(_frag);
	this->_contentVert = this->_LoadShader(_vert);
}

Shader::~Shader()
{
	glDetachShader(this->_progID, this->_vertID);
	glDetachShader(this->_progID, this->_fragID);
	glDeleteShader(this->_fragID);
	glDeleteShader(this->_vertID);
	glDeleteProgram(this->_progID);
}

Shader::Shader(Shader const &instance)
{
	*this = instance;
}

Shader&		Shader::operator=(Shader const &rhs)
{
	this->_frag = rhs._frag;
	this->_vert = rhs._vert;
	this->_fragID = rhs._fragID;
	this->_vertID = rhs._vertID;
	this->_progID = rhs._progID;
	this->_contentFrag = rhs._contentFrag;
	this->_contentVert = rhs._contentVert;
	return (*this);
}

void		Shader::CreateShaders()
{
	try {
		this->_fragID = glCreateShader(GL_FRAGMENT_SHADER);
		this->_vertID = glCreateShader(GL_VERTEX_SHADER);
		CompileShader(this->_fragID, this->_contentFrag);
		CompileShader(this->_vertID, this->_contentVert);
		this->_InitProgram();
	}
	catch (ShaderException const &err) {
		std::cerr << "Error shader :" << err.what() << std::endl;
	}
}

GLuint		Shader::GetProgramID() const
{
	return (this->_progID);
}

void		Shader::PrintVertexShader()
{
	std::cout << "Vertex Shader :" << std::endl;
	std::cout << this->_contentVert << std::endl;
}

void		Shader::PrintFragmentShader()
{
	std::cout << "Fragment Shader :" << std::endl;
	std::cout << this->_contentFrag << std::endl;
}

std::string	Shader::_LoadShader(std::string name) throw(ShaderException)
{
	std::ifstream	fd(name);
	std::string		content;

	if (fd.is_open())
	{
		fd.seekg (0, std::ios::end);
		content.reserve(fd.tellg());
		fd.seekg (0, std::ios::beg);
		content.assign((std::istreambuf_iterator<char>(fd)),
						std::istreambuf_iterator<char>());
		fd.close();
		return (content);
	}
	else
		throw ShaderException::ShaderException("Exception opening/reading/closing file : " + name);
}

void		Shader::CompileShader(GLuint shaderID, std::string content) throw(ShaderException)
{
	GLint			params;
	int				len;
	char const		*dst;

	params = GL_FALSE;
	len = content.size();
	dst = content.c_str();
	glShaderSource(shaderID, 1, &dst, &len);
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &params);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char				ilog[2048];
		char				ilog2[2048];
		
		glGetShaderInfoLog(shaderID, len, &params, ilog);
		sprintf(ilog2, "%s", ilog);
		throw ShaderException::ShaderException(ilog2);
	}
}

void	Shader::_InitProgram() throw(ShaderException)
{
	GLint			params;
	int				len;

	this->_progID = glCreateProgram();
	glAttachShader(this->_progID, this->_vertID);
	glAttachShader(this->_progID, this->_fragID);
	glLinkProgram(this->_progID);
	glGetProgramiv(this->_progID, GL_LINK_STATUS, &params);
	glGetProgramiv(this->_progID, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		char				ilog[2048];
		char				ilog2[2048];
		
		glGetProgramInfoLog(this->_progID, len, &params, ilog);
		sprintf(ilog2, "%s", ilog);
		glDeleteProgram(this->_progID);
		throw ShaderException::ShaderException(ilog2);
	}
	this->_PrintInfo(&params);
}

void	Shader::_PrintInfo(GLint *params)
{
	glGetProgramiv(this->_progID, GL_LINK_STATUS, params);
	std::cout << "Link Status : " << *params << std::endl;
	glGetProgramiv(this->_progID, GL_ATTACHED_SHADERS, params);
	std::cout << "Numbers of shaders attached : " << *params << std::endl;
	glGetProgramiv(this->_progID, GL_ACTIVE_ATTRIBUTES, params);
	std::cout << "Numbers of active attribute variables  : " << *params << std::endl;
	glGetProgramiv(this->_progID, GL_ACTIVE_UNIFORMS, params);
	std::cout << "Numbers of active uniform variables : " << *params << std::endl;
}