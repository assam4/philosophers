#include "types.h"

static void	semaphores_unlink(t_table *table)
{
	sem_unlink(PRINT);
	sem_unlink(DEAD);
	sem_unlink(FULL);
	sem_unlink(SECURE);
	sem_unlink(FORKS);
}

void	destroy_semaphores(t_table *table)
{
	sem_close(table->dead_stop);
	sem_close(table->full_stop);
	sem_close(table->secure_lock);
	sem_close(table->forks);
	sem_close(table->print);
	semaphores_unlink(table);
}

int	init_semaphores(t_table *table)
{
	semaphores_unlink(table);
	table->print = sem_open(PRINT, O_CREAT | O_EXCL , 0644, 1);
	table->dead_stop = sem_open(DEAD, O_CREAT | O_EXCL, 0644, START_VAL);
	table->full_stop = sem_open(FULL, O_CREAT | O_EXCL, 0644, START_VAL);
	table->secure_lock = sem_open(SECURE, O_CREAT | O_EXCL, 0644, table->philos_count / 2);
	table->forks = sem_open(FORKS, O_CREAT | O_EXCL, 0644, table->philos_count);
		if (table->forks == SEM_FAILED
		|| table->print == SEM_FAILED
		|| table->dead_stop == SEM_FAILED
		|| table->full_stop == SEM_FAILED
		|| table->forks == SEM_FAILED)
	{
		destroy_semaphores(table);
		printf(SEM_ERR);
		return (EAGAIN);
	}
	else
		return (EXIT_SUCCESS);
}
