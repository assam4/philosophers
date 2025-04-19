#include "types.h"

static char	*i_to_str(int n)
{
	char	*ptr;
	size_t	nb_len;
	int	n_cpy;

	nb_len = 1;
	n_cpy = n;
	while (n_cpy)
	{
		++nb_len;
		n_cpy /= 10;
	}
	ptr = (char *)malloc((nb_len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	ptr[nb_len] = '\0';
	if (n == 0)
		*ptr = '0';
	while (n)
	{
		ptr[--nb_len] = n % 10 + '0';
		n /= 10;
	}
	return (ptr);
}

void	destroy_semaphores(t_table *table)
{
	char	*i_str;
	int	i;

	i = START_VAL;
	while (i < table->philos_count)
	{
		sem_close(table->philos[i].dead_s);
		i_str = i_to_str(i);
		sem_unlink(i_str);
		free(i_str);
		++i;
	}
	sem_close(table->forks);
	sem_close(table->print);
	sem_close(table->stop);
	sem_close(table->full);
	sem_unlink(FORKS);
	sem_unlink(PRINT);
	sem_unlink(STOP);
	sem_unlink(FULL);
}

static	int	init_philos_sem(t_table *table)
{
	int	i;
	char	*i_str;

	i = START_VAL;
	while (i < table->philos_count)
	{
		i_str = i_to_str(i);
		table->philos[i].dead_s = sem_open(i_str, O_CREAT | O_EXCL, 0644, 1);
		free(i_str);
		if (table->philos[i].dead_s == SEM_FAILED)
			break ;
		++i;
	}
	if (i == table->philos_count)
		return (EXIT_SUCCESS);
	else
		return (EAGAIN);
}
int	init_semaphores(t_table *table)
{
	sem_unlink(FORKS);
	sem_unlink(PRINT);
	sem_unlink(STOP);
	sem_unlink(FULL);
	table->forks = sem_open(FORKS, O_CREAT | O_EXCL , 0644, table->philos_count);
	table->print = sem_open(PRINT, O_CREAT | O_EXCL , 0644, 1);
	table->stop = sem_open(STOP, O_CREAT | O_EXCL, 0644, 0);
	table->full = sem_open(FULL, O_CREAT | O_EXCL , 0644, 1);
	if (table->forks == SEM_FAILED
		|| table->print == SEM_FAILED
		|| table->stop == SEM_FAILED
		|| table->full == SEM_FAILED
		|| init_philos_sem(table))
	{
		destroy_semaphores(table);
		printf(SEM_ERR);
		return (EAGAIN);
	}
	else
		return (EXIT_SUCCESS);
}
