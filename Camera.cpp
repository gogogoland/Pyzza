/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Camera.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/05 18:52:02 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/03 16:41:30 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Camera.hpp"

Camera::Camera(glm::vec3 cam, GLfloat speedRot,
				GLfloat speedTrans, GLfloat angleRot) :
				_cam(cam), _speedRot(speedRot),
				_speedTrans(speedTrans), _angleRot(angleRot)
{}

Camera::~Camera()
{}

glm::vec3	Camera::GetPos() const
{
	return (this->_cam);
}

glm::vec3	Camera::InversePos()
{
	return (glm::vec3(-this->_cam.x, -this->_cam.y, -this->_cam.z));
}

GLfloat			Camera::GetSpeedRot() const
{
	return (this->_speedRot);
}

GLfloat			Camera::GetSpeedTrans() const
{
	return (this->_speedTrans);
}

GLfloat			Camera::GetAngleRot() const
{
	return (this->_angleRot);
}

void			Camera::MoveCamera(int axis, double diff_sec)
{
	this->_cam[axis] += this->_speedTrans * diff_sec;
}

void			Camera::SetCamera(glm::vec3 cam)
{
	this->_cam = cam;
}