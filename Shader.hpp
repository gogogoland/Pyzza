/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Shader.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/02/25 21:09:40 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/03/25 17:31:32 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADER_HPP
# define SHADER_HPP

# include <OpenGL/gl3.h>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>

class Shader
{
public:
	class ShaderException : public std::exception
	{
	public:
		ShaderException(std::string const &error);
		virtual ~ShaderException() throw();

		virtual const char		*what() const throw();
	private: 
		std::string const &_msg;
	};

	Shader();
	Shader(std::string fragment, std::string vertex);
	Shader(Shader const &);
	~Shader();
	
	Shader&			operator=(Shader const &rhs);
	void			CreateShaders();
	GLuint			GetProgramID() const;
	void			PrintVertexShader();
	void			PrintFragmentShader();
	void			CompileShader(GLuint shaderID,
					std::string content) throw(ShaderException);

private:
	std::string		_frag;
	std::string		_vert;
	GLuint			_fragID;
	GLuint			_vertID;
	GLuint			_progID;
	std::string		_contentFrag;
	std::string		_contentVert;

	std::string		_LoadShader(std::string name) throw(ShaderException);

	void			_InitProgram() throw(ShaderException);
	void			_PrintInfo(GLint *params);
};

#endif