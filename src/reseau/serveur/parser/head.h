#ifndef HEAD_H
# define HEAD_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <ctype.h>

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
# define P_MAX 100

# define X_MIN 0
# define X_MAX 100

# define Y_MIN 0
# define Y_MAX 100

# define C_MIN 0
# define C_MAX 100

# define T_MIN 0
# define T_MAX 100
/*
** [0] : -p, [1] : -x, [2] : -y, [3] : -c, [4] -t [5] -b
*/ 
typedef struct	s_err
{
	int			flag_int[7];
	char		**team;
}				t_err;

#endif
