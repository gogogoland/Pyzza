/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbalea <tbalea@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/04/23 12:39:04 by tbalea            #+#    #+#             */
/*   Updated: 2017/02/08 18:44:59 by nd-heyge         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include "libft.h"

# include <sys/time.h>
# include <stdbool.h>
# include <arpa/inet.h>
# include <ctype.h>
# include <fcntl.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdio.h>
# include <sys/dir.h>
# include <sys/socket.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <time.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>

# define ERR_P "[ERROR] : Port isn't correct.\n"
# define ERR_W "[ERROR] : Width isn't correct.\n"
# define ERR_H "[ERROR] : Height isn't correct.\n"
# define ERR_NBP "[ERROR] : Number of player isn't correct.\n"
# define ERR_TEAM "[ERROR] : Team specified are not correct correct.\n"
# define ERR_TIME "[ERROR] : time specified isn't correct.\n"
# define ERR_INT "[ERROR] : It must be an integer.\n"
# define ERR_NAME "[ERROR] : you can't use this team name.\n"
# define ERR_ARG "[ERROR] : too few arguments...\n"

# define P_MIN 0
# define P_MAX 2000000

# define X_MIN 17
# define X_MAX 100

# define Y_MIN 17
# define Y_MAX 100

# define C_MIN 0
# define C_MAX 100

# define TIME_MIN 0
# define TIME_MAX 200

# define NBR_RESRC_CASE_MAX 7

typedef struct sockaddr_in	t_sokadr_in;
typedef struct sockaddr		t_sokadr;
typedef struct in_addr		t_i_adr;

typedef struct				s_cmd
{
	int						beg;
	int						end;
	char					*cmd;
}							t_cmd;

typedef struct				s_ring
{
	char					**command;
	char					end;
	int						cur;
	int						len;
}							t_ring;

typedef struct				s_coord
{
	int						x;
	int						y;
	int						*rsc;
}							t_coord;

typedef struct				s_fds
{
	fd_set					rd;
	fd_set					wr;
	fd_set					ex;
	int						max;
}							t_fds;

typedef struct				s_gfx
{
	int						socket;
	unsigned int			len;
	struct sockaddr_in		sin;
	struct s_ring			*ring;
	struct s_gfx			*prev;
	struct s_gfx			*next;
	bool					isgfx;
}							t_gfx;

typedef struct				s_client
{
	int						socket;
	unsigned int			len;
	struct sockaddr_in		sin;
	struct s_coord			pos;
	int						sens;
	int						team;
	int						name;
	bool					fork;
	int						mom;
	int						lvl;
	int						tolvl;
	int						action;
	long long int			time;
	long long int			health;
	char					*current_cmd;
	char					*fornext_cmd;
	int						*acolyte;
	bool					casting;
	struct s_ring			*ring;
	struct s_client			*next;
	struct s_client			*prev;
}							t_client;

typedef struct				s_server
{
	struct s_coord			plateau;
	int						port;
	int						socket;
	int						name;
	int						time;
	int						lvl;
	int						old_player_max;
	int						player_max;
	int						egg;
	int						***map;
	char					**team;
	struct s_gfx			*gfx;
	struct s_client			*clt;
	bool					bonus_fork;
}							t_server;

void						ring_recv(t_server *srv, t_cmd cmd,
										t_ring *ring, int socket);
int							ring_recv_allow(t_cmd cmd, t_ring *ring, int cur);
char						*ring_send(t_server *srv, t_ring *ring);
t_ring						*ring_init(t_server *srv, int len);
void						ring_zero(t_ring *ring);
void						ring_kill(t_ring *ring);

t_client					*client_init(t_server *srv);
void						client_init_data(t_client *clt,
											t_server *srv);
void						client_zero(t_client *clt, t_fds *fds,
										t_server *srv);
void						client_kill(t_client *clt, t_fds *fds);

t_gfx						*graphe_init(t_server *srv);
t_gfx						*graphe_news(t_server *srv, t_gfx *prev, t_fds *fds,
										int s);
void						graphe_kill(t_server *srv, t_gfx *gfx, t_fds *fds,
										bool gfxtoclt);

t_server					*init_map(t_server *srv);
void						kill_map(t_server *srv);

void						generate_map(t_server *srv, int nbr_resrc_case);

bool						parser(int ac, char **av, t_server *srv);
bool						parser_int(int ac, char **av, t_server *srv);
bool						parser_team(int ac, char **av, t_server *srv);
int							server_time(struct timeval *time);
t_server					*server_create(int argc, char **argv);
void						server_log(t_server *srv, const char *msg);

bool						recv_client(t_fds *fds, t_server *srv, int ret);
void						send_client(t_fds *fds, t_server *srv, int tim);
bool						save_cur_cmd(t_client *clt, char *msg, int action);
void						oldtonew_cmd(t_client *clt);
void						erasecur_cmd(t_client *clt);
void						send_client_action(t_server *srv, t_client *clt,
												bool ok);
void						send_graphe_action(t_server *srv, char *msg,
												int spec, t_client *clt);

void						command_forward(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_eject(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_forward(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_inventory(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_left(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_lvlup(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_msg(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
bool						msg_save(t_client *clt, char *msg);
void						command_pose(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_right(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_seek(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_take(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_incant(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
bool						incant_process(t_client *cllt, t_server *srv);
void						incant_msg_acolyte(t_server *srv, t_client *clt,
												int lim_acolyte, int state);
bool						incant_help_acolyte(t_server *srv, t_client *clt,
												int lim_acolyte);
void						incant_lvlup_acolyte(t_server *srv, t_client *clt,
												int lim_acolyte);
void						incant_reset_acolyte(t_server *srv, t_client *clt,
												int lim_acolyte);
void						command_fork(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_nbr_co(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_death(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);
void						command_eat(t_fds *fds, t_server *srv,
								t_client *clt, char *cmd);

void						command_player(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_graphe(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_size(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_time_server(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_time_change(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_player_inv(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_player_lvl(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_player_pos(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_map(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_box(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
void						command_team_name(t_fds *fds, t_server *srv,
								t_gfx *gfx, char *cmd);
int							command_get_int(int i, char *cmd);
void						command_box_content(t_gfx *gfx, int x, int y,
												int *box);
void						command_graphical_bad_parameters(t_fds *fds,
															t_server *srv,
															t_gfx *gfx,
															char *cmd);

char						*command_write_msg(t_client *clt, int action,
												int n, char *pbc);
char						*command_write_msg_incant_end(t_client *clt);
char						*command_write_msg_end_fork(t_client *frk);

#endif
