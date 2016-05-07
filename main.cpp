/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/30 17:43:22 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/07 16:08:00 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gfx.hpp"

int	ft_create_grid(OpenGL *openGL, int column, int line)
{
	/*
	** VBO == points to draw a grid
	** 4 == numbers of points for Square
	** 3 == numbers of coordinated (x, y, z)
	** 2 == numbers of triangles to draw a square
	** VT == positions of texture
	*/
	std::vector<GLfloat>	vbo(column * line * 4 * 3 * 2);
	std::vector<GLfloat>	vt(column * line * 4 * 2 * 2);
	int						pts = 0;

	for (int c = 0; c < column; c++)
	{	
		for (int l = 0; l < line; l++)
		{
			vbo[pts + 0] = 0.0f + l;
			vbo[pts + 1] = 0.0f;
			vbo[pts + 2] = 0.0f + c;

			vbo[pts + 3] = 1.0f + l;
			vbo[pts + 4] = 0.0f;
			vbo[pts + 5] = 0.0f + c;

			vbo[pts + 6] = 1.0f + l;
			vbo[pts + 7] = 0.0f;
			vbo[pts + 8] = 1.0f + c;

			vbo[pts + 9] = 0.0f + l;
			vbo[pts + 10] = 0.0f;
			vbo[pts + 11] = 0.0f + c;

			vbo[pts + 12] = 1.0f + l;
			vbo[pts + 13] = 0.0f;
			vbo[pts + 14] = 1.0f + c;

			vbo[pts + 15] = 0.0f + l;
			vbo[pts + 16] = 0.0f;
			vbo[pts + 17] = 1.0f + c;
			pts += 18;
		}
	}
	int		pts2 = 0;
	for (int c = 0; c < column; c++)
	{	
		for (int l = 0; l < line; l++)
		{
			vt[pts2 + 0] = 0.0f;
			vt[pts2 + 1] = 0.0f;

			vt[pts2 + 2] = 1.0f;
			vt[pts2 + 3] = 0.0f;

			vt[pts2 + 4] = 1.0f;
			vt[pts2 + 5] = 1.0f;

			vt[pts2 + 6] = 0.0f;
			vt[pts2 + 7] = 1.0f;
			pts2 += 8;
		}
	}
	openGL->InitVBO(vbo, 3);
	openGL->InitVBO(vt, 2);
	return (vbo.size());
}

void	ft_matrix(SDL *sdl, t_matrix *mvp, Camera *cam)
{
	float		aspect;
	glm::mat4	translate;
	glm::mat4	rotate;
	glm::vec3	axisY;

	aspect = sdl->GetWidth() / sdl->GetHeight();
	mvp->proj = glm::perspective(67.0f, aspect, 0.1f, 100.0f);
	translate = glm::translate(glm::mat4(1.0f), cam->InversePos());
	axisY = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate = glm::rotate(glm::mat4(1.0f), -cam->GetAngleRot(), axisY);
	mvp->view = rotate * translate;
}

void	ft_key_event(SDL *sdl, Camera *cam)
{
	if (sdl->KeyDownEvent())
	{
		switch (sdl->GetKeyEvents())
		{
			case SDLK_ESCAPE: sdl->StopRunning(); break;
			case SDLK_a : cam->MoveCamera(0, sdl->DiffGetTime()); break;
			case SDLK_d : cam->MoveCamera(0, -sdl->DiffGetTime()); break;
			case SDLK_w : cam->MoveCamera(2, sdl->DiffGetTime()); break;
			case SDLK_s : cam->MoveCamera(2, -sdl->DiffGetTime()); break;
			case SDLK_UP : cam->MoveCamera(1, sdl->DiffGetTime()); break;
			case SDLK_DOWN : cam->MoveCamera(1, -sdl->DiffGetTime()); break;
			default : break;
		}
	}
	if (sdl->WinCloseEvents())
		sdl->StopRunning();
}

void	ft_update_view(OpenGL *openGL, t_matrix *mvp, Camera cam)
{
	glm::mat4	translate;
	glm::mat4	rotate;
	glm::vec3	axisY;

	translate = glm::translate(glm::mat4(1.0f), cam.InversePos());
	axisY = glm::vec3(0.0f, 1.0f, 0.0f);
	rotate = glm::rotate(glm::mat4(1.0f), -cam.GetAngleRot(), axisY);
	mvp->view = rotate * translate;
	openGL->SetUniform("view", mvp->view);
}

int		main(void)
{
	SDL						sdl("SamouraiPyzzaPussy", 800, 600);
	OpenGL					openGL;
	Shader					shader("frag.glsl", "vertex.glsl");
	Camera					cam(CAMERA_POS, 1.0f, 0.0005f, 0.0f);
	t_matrix				mvp;
	int						nbrPts;

	try {
		nbrPts = ft_create_grid(&openGL, 5, 5);
		shader.CreateShaders();
		openGL.UseProgram(shader.GetProgramID());
		openGL.GetUniform(shader.GetProgramID(), "view");
		openGL.GetUniform(shader.GetProgramID(), "proj");
		ft_matrix(&sdl, &mvp, &cam);
		openGL.SetUniform("view", mvp.view);
		openGL.SetUniform("proj", mvp.proj);
		openGL.InitTexture("img/ILTQq.png");
		openGL.CullFace(GL_CULL_FACE, GL_FRONT, GL_CW);
		openGL.ClearColor(0.5,0.5,0.5,1.0);
		while (sdl.StillRunning())
		{
			openGL.Clear();
			openGL.ViewPort(sdl.GetWidth(), sdl.GetHeight());
			openGL.ParameterPoints(5);
			openGL.UpdateTexture();
			openGL.DrawTriangle(nbrPts);
			openGL.UseProgram(shader.GetProgramID());
			openGL.SetVAO();
			openGL.SetUniform("view", mvp.view);
			sdl.WaitEvents();
			ft_key_event(&sdl, &cam);
			ft_update_view(&openGL, &mvp, cam);
			sdl.SwapBuffer();
		}
	} catch (std::exception const &err) {
		std::cerr << "Error main (" << err.what() << std::endl;
	}
	return (0);
}