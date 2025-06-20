/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:35:12 by saslanya          #+#    #+#             */
/*   Updated: 2025/06/10 00:02:27 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

static char	*convert_to_str(int num)
{
	int		len;
	int		i;
	int		num_cpy;
	char	*str;

	len = 1;
	num_cpy = num;
	while (num)
	{
		++len;
		num /= DECIMAL;
	}
	str = malloc(len * sizeof(char));
	if (!str)
		return (NULL);
	i = LOOP_START;
	while (num_cpy)
	{
		str[i++] = num_cpy % DECIMAL + '0';
		num_cpy /= DECIMAL;
	}
	str[i] = '\0';
	return (str);
}

static void	semaphores_unlink(t_table *table)
{
	int		i;
	char	*name;

	i = LOOP_START;
	while (i < table->philos_count)
	{
		name = convert_to_str(table->philos[i++].number);
		sem_unlink(name);
		free(name);
	}
	sem_unlink(PRINT);
	sem_unlink(DEAD);
	if (table->must_eat_count)
		sem_unlink(FULL);
	sem_unlink(SECURE);
	sem_unlink(FORKS);
}

void	destroy_semaphores(t_table *table)
{
	int	i;

	i = LOOP_START;
	while (i < table->philos_count)
	{
		sem_close(table->philos[i].die);
		++i;
	}
	sem_close(table->dead_stop);
	if (table->must_eat_count)
		sem_close(table->full_stop);
	sem_close(table->secure_lock);
	sem_close(table->forks);
	sem_close(table->print);
	semaphores_unlink(table);
}

static int	init_philos_sem(t_table *table)
{
	int		i;
	char	*name;

	i = LOOP_START;
	while (i < table->philos_count)
	{
		name = convert_to_str(table->philos[i].number);
		if (!name)
			return (printf(ALLOC_ERR), ENOMEM);
		table->philos[i].die = sem_open(name, O_CREAT | O_EXCL, 0644, 1);
		free(name);
		if (table->philos[i].die == SEM_FAILED)
			return (EAGAIN);
		++i;
	}
	return (EXIT_SUCCESS);
}

int	init_semaphores(t_table *table)
{
	int	secure_count;

	if (!(table->philos_count / 2))
		secure_count = 1;
	else
		secure_count = table->philos_count / 2;
	semaphores_unlink(table);
	table->print = sem_open(PRINT, O_CREAT | O_EXCL, 0644, 1);
	table->dead_stop = sem_open(DEAD, O_CREAT | O_EXCL, 0644, START_VAL);
	if (table->must_eat_count)
		table->full_stop = sem_open(FULL, O_CREAT | O_EXCL, 0644, START_VAL);
	table->secure_lock = sem_open(SECURE, O_CREAT | O_EXCL, 0644, secure_count);
	table->forks = sem_open(FORKS, O_CREAT | O_EXCL, 0644, table->philos_count);
	if (table->forks == SEM_FAILED || table->print == SEM_FAILED
		|| table->dead_stop == SEM_FAILED || table->forks == SEM_FAILED
		|| (1 && (table->must_eat_count && table->full_stop == SEM_FAILED))
		|| init_philos_sem(table))
	{
		destroy_semaphores(table);
		printf(SEM_ERR);
		return (EAGAIN);
	}
	else
		return (EXIT_SUCCESS);
}
