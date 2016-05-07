/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gfx.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/30 17:44:38 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/07 16:08:39 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GFX_HPP
# define GFX_HPP

# include <cstdlib>
# include <iostream>
# include <glm/glm.hpp>
# include <glm/gtc/matrix_transform.hpp>
# include <glm/gtx/rotate_vector.hpp>

# include "SDL.hpp"
# include "OpenGL.hpp"
# include "Shader.hpp"
# include "Camera.hpp"

# define CAMERA_POS		glm::vec3(0.0f, -1.0f, 2.0f)

typedef struct	s_matrix
{
	glm::mat4	view;
	glm::mat4	proj;
}				t_matrix;

#endif