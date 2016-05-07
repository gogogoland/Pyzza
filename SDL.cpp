/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDL.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 15:55:57 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/04 19:03:33 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SDL.hpp"

SDL::SDLException::SDLException(std::string const &error) : _msg(error) {}

SDL::SDLException::~SDLException() throw() {}

const char 	*SDL::SDLException::what() const throw() {
	return (this->_msg.c_str());
}

SDL::SDL(char const *name, int width, int height) :
			_namewin(name), _width(width), _height(height) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0) {
		std::string error = "Erreur Init SDL : ";
		std::string error2 = SDL_GetError();
		throw SDLException(error + error2);
	}
	// Version d'OpenGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Double Buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	this->_win = SDL_CreateWindow(this->_namewin,
							SDL_WINDOWPOS_CENTERED,
							SDL_WINDOWPOS_CENTERED,
							this->_width, this->_height,
							SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (this->_win == 0) {
		std::string error = "Erreur Create Windows : ";
		std::string error2 = SDL_GetError();
		throw SDLException(error + error2);
	}
	this->_contextOpenGL = SDL_GL_CreateContext(this->_win);
	if (this->_contextOpenGL == 0)
		throw SDLException(SDL_GetError());
	this->_running = true;
}

SDL::~SDL() {
	if (this->_contextOpenGL != 0)
		SDL_GL_DeleteContext(this->_contextOpenGL);
	if (this->_win != 0)
		SDL_DestroyWindow(this->_win);
	SDL_Quit();
}

double		SDL::DiffGetTime() {
	static double	prev_sec = 0.0f;
	double			curr_sec;
	double			diff_sec;

	if (!prev_sec)
		prev_sec = SDL_GetTicks();
	curr_sec = SDL_GetTicks();
	diff_sec = curr_sec - prev_sec;
	prev_sec = curr_sec;
	return (diff_sec);
}

bool		SDL::StillRunning() const {
	return (this->_running);
}

void		SDL::StopRunning() {
	this->_running = false;
}

int			SDL::GetWidth() const {
	return (this->_width);
}

int			SDL::GetHeight() const {
	return (this->_height);
}

SDL_Event	SDL::GetEvents() const {
	return (this->_events);
}

int			SDL::GetWinEvents() const {
	return (this->_events.window.event);
}

int			SDL::GetKeyEvents() const {
	return (this->_events.key.keysym.sym);
}

bool		SDL::WinCloseEvents() {
	return (this->_events.window.event == SDL_WINDOWEVENT_CLOSE);
}

bool		SDL::KeyDownEvent() {
	return (this->_events.type == SDL_KEYDOWN);
}

void		SDL::WaitEvents() {
	SDL_WaitEvent(&this->_events);
}

int			SDL::PollEvent() {
	return (SDL_PollEvent(&this->_events));
}

void		SDL::SwapBuffer() {
	SDL_GL_SwapWindow(this->_win);
}
