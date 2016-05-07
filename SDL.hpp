/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SDL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nd-heyge <nd-heyge@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/02 15:53:55 by nd-heyge          #+#    #+#             */
/*   Updated: 2016/05/04 19:26:47 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SDL_HPP
# define SDL_HPP

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <string>

class SDL
{
public:

	class SDLException : public std::exception
	{
	public:
		SDLException(std::string const &error);
		virtual ~SDLException() throw();
		virtual const char			*what() const throw();
	private:
		std::string const &_msg;
	};
	SDL(char const *name, int width, int height);
	~SDL();

	double		DiffGetTime();
	bool		StillRunning() const;
	void		StopRunning();
	SDL_Event	GetEvents() const;
	int			GetWinEvents() const;
	int			GetKeyEvents() const;
	int			GetWidth() const;
	int			GetHeight() const;

	bool		WinCloseEvents();
	bool		KeyDownEvent();
	int			PollEvent();
	void		WaitEvents();
	void		SwapBuffer();
private:
	char const		*_namewin;
	int				_width;
	int				_height;
	bool			_running;
	SDL_Window*		_win;
	SDL_GLContext	_contextOpenGL;
	SDL_Event		_events;
};

#endif