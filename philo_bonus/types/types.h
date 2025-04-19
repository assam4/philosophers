#ifndef TYPES_H
# define TYPES_H

# include "validation.h"
# include <signal.h>
# include <semaphore.h>
# include <pthread.h>
# include <sys/types.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <unistd.h>

# define ALLOC_ERR "\033[1;31mFailed attempt at alocation!.\n\033[0m"

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int			number;
	int			eat_count;
	long long	last_meal;
	pid_t		pid;
	t_table		*table;
	sem_t		*eat_s;
	sem_t		*died_s;
	pthread_t	eat_m;
	pthread_t	die_m;
}	t_philosopher;

typedef struct s_table
{
	int				philos_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	long long		start_time;
	pid_t			*pids;
	t_philosopher	*philo;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*stop;
}	t_table;

#endif
