/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saslanya <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 12:30:06 by saslanya          #+#    #+#             */
/*   Updated: 2025/04/19 12:30:08 by saslanya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "simulation.h"

static void	wait_childs(t_table *table)
{
	int	i;

	i = START_VAL;
	while (i < table->philos_count)
		waitpid(table->philos[i++].pid, NULL, 0);
}
int	main(int argc, char **argv)
{
	t_table	*table;
	int		exit_bit;

	table = NULL;
	if (parse_args(argc, argv))
		return (EINVAL);
	if (allocation_mem(ft_atoi(argv[1]), &table))
		return (ENOMEM);
	set_userdef_params(argc, (const char **)argv, table);
	philos_init(table);
	if (init_semaphores(table))
		return (deallocation_mem(&table), EAGAIN);
	exit_bit = start_simulation(table);
	wait_childs(table);
	destroy_semaphores(table);
	deallocation_mem(&table);
	return (exit_bit);
}
