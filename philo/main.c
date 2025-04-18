#include "simulation.h"

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
	if (init_mutexs(table))
		return (deallocation_mem(&table), EAGAIN);
	exit_bit = start_simulation(table);
	if (exit_bit)
		exit(exit_bit);
	destroy_mutexs(table);
	deallocation_mem(&table);
	return (exit_bit);
}
