/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/05 18:52:02 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/03 16:28:49 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CAMERA_HPP
# define CAMERA_HPP

# include <OpenGL/gl3.h>
# include <OpenGL/gl3ext.h>
# include <glm/glm.hpp>

class	Camera
{
private:
	glm::vec3		_cam;
	GLfloat			_speedRot;
	GLfloat			_speedTrans;
	GLfloat			_angleRot;
public:
	Camera(glm::vec3 cam, GLfloat speedRot,
			GLfloat speedTrans, GLfloat angleRot);
	~Camera();
	glm::vec3		GetPos() const;
	glm::vec3		InversePos();
	GLfloat			GetSpeedRot() const;
	GLfloat			GetSpeedTrans() const;
	GLfloat			GetAngleRot() const ;
	void			MoveCamera(int axis, double diff_sec);
	void			SetCamera(glm::vec3 cam);
};

#endif