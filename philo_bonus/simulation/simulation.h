#ifndef SIMULATION_H
# define SIMULATION_H

# include "types.h"
# include <fcntl.h>  // для O_CREAT и O_EXCL


# define INTERVAL 100

# define THREAD_ERR "\033[31mError creating thread\n\033[0m"
# define FORK_ERR "\033[31mError creating process\n\033[0m"

# define GET_FORK "[%llu] %i has taken a fork 🍴\n"
# define EATING "\033[0;32m[%llu] %i is eating 😋\n\033[0m"
# define SLEEPING "\033[1;33m[%llu] %i is sleeping 😴\n\033[0m"
# define THINKING "\033[0;33m[%llu] %i is thinking 🤔\n\033[0m"
# define DIED "\033[38;5;88m[%llu] %i is dead 💀\033[0m\n"
# define DINNER_OVER "\033[1;32m[%llu]Dinner is over 🎉\033[0m\n"

void	print_state(t_philosopher *philo,const char *message);
void		*life_cycle(void *param);
int		start_simulation(t_table *table);


#endif
