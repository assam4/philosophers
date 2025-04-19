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
# include <fcntl.h>

# define ALLOC_ERR "\033[1;31mFailed attempt at alocation!.\n\033[0m"
# define SEM_ERR "\033[1;31mFailed to open semaphore!.\n\033[0m"

# define FORKS "forks"
# define PRINT "print"
# define STOP "stop"
# define FULL "full"
# define DIED_S "died_sem_"

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int			number;
	int			eat_count;
	long long	last_eat_time;
	pid_t		pid;
	t_table		*table;
	sem_t		*dead_s;
	pthread_t	die_m;
}	t_philosopher;

typedef struct s_table
{
	int				philos_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				fullnes;
	long long		start_time;
	pid_t			*pids;
	t_philosopher	*philos;
	sem_t			*forks;
	sem_t			*print;
	sem_t			*stop;
	sem_t			*full;
}	t_table;

long long	time_ms(void);
int			allocation_mem(int count, t_table **table);
void		deallocation_mem(t_table **table);
void		set_userdef_params(int count, const char **params, t_table *table);
void		philos_init(t_table *table);
int	init_semaphores(t_table *table);
void	destroy_semaphores(t_table *table);
#endif
