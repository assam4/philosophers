#ifndef TYPES_H
# define TYPES_H

# include "validation.h"
# include <unistd.h>
# include <signal.h>
# include <pthread.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <semaphore.h>

# define ALLOC_ERR "\033[1;31mFailed attempt at alocation!.\n\033[0m"
# define MUTEX_ERR "\033[1;31mSystem limit on mutexes reached\n\033[0m"

typedef struct s_table	t_table;

typedef struct s_philosopher
{
	int				pid;
	int				meals_eaten;
	long long		last_eat_time;
	pthread_t		thread;
	t_table			*table;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}	t_philosopher;

typedef struct s_table
{
	int				philos_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				someone_died;
	int				fullnes;
	long long		start_time;
	t_philosopher	*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_m;
	pthread_mutex_t	time_m;
	pthread_mutex_t	dead_m;
	pthread_mutex_t	fullnes_m;
}	t_table;

// memory utils
long long	time_ms(void);
int			allocation_mem(int count, t_table **table);
void		deallocation_mem(t_table **table);
void		set_userdef_params(int count, const char **params, t_table *table);
void		philos_init(t_table *table);

// mutex utils
int			init_mutexs(t_table *table);
void		destroy_mutexs(t_table *table);

#endif
