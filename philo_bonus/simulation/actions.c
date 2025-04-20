#include "simulation.h"

void	print_state(t_philosopher *philo,const char *message)
{
	sem_wait(philo->table->print);
	printf(message, time_ms() - philo->table->start_time, philo->number);
	sem_post(philo->table->print);
}

static int	take_forks(t_philosopher *philo)
{
	if (!(philo->number % 2))
	{
		usleep(1000);
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);

	}
	else
	{
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);
		if (philo->table->philos_count == 1)
		{
			sem_post(philo->table->forks);
			usleep(philo->table->time_to_die * 1000);
			print_state(philo, DIED);
			return (EXIT_FAILURE);
		}
		sem_wait(philo->table->forks);
		print_state(philo, GET_FORK);

	}
	return (EXIT_SUCCESS);
}

static void	crossed_target(t_philosopher *philo)
{
	if (++philo->eat_count == philo->table->must_eat_count)
	{
		sem_wait(philo->table->full);
		++philo->table->fullnes;
		sem_post(philo->table->full);
	}
}

static void	actions(t_philosopher *philo)
{
	long long	time;

	time = time_ms();
	print_state(philo, EATING);
	while (time_ms() - time < philo->table->time_to_eat)
		usleep(INTERVAL);
	sem_wait(philo->dead_s);
	philo->last_eat_time = time_ms();
	sem_post(philo->dead_s);
	sem_post(philo->table->forks);
	sem_post(philo->table->forks);
	if (philo->table->must_eat_count)
		crossed_target(philo);
	time = time_ms();
	print_state(philo, SLEEPING);
	while (time_ms() - time < philo->table->time_to_sleep)
		usleep(INTERVAL);
	print_state(philo, THINKING);
}

void	*life_cycle(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	while (1)
	{
		if (take_forks(philo))
		{
			sem_post(philo->table->stop);
			while (wait(NULL) > 0)
			;
			return (NULL);
		}
		else
			actions(philo);
	}
	return (NULL);
}
