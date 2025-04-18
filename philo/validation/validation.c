#include "validation.h"

static int	count_validation(int count)
{
	if (count != VARG_COUNT1 && count != VARG_COUNT2)
		return (EINVAL);
	else
		return (EXIT_SUCCESS);
}

static int	is_all_digit(const char *current)
{
	while (*current)
	{
		if (*current >= '0' && *current <= '9')
			++current;
		else
			return (EINVAL);
	}
	return (EXIT_SUCCESS);
}

int	ft_atoi(const char *str)
{
	int	result;

	result = START_VAL;
	while (*str && *str >= '0' && *str <= '9')
		result = result * DECIMAL + (*str++ - '0');
	return (result);
}

static int	params_validation(int count, char **argv)
{
	int	i;

	i = LOOP_START;
	while (++i < count)
		if (is_all_digit(argv[i]))
			return (EINVAL);
	i = LOOP_START;
	if (ft_atoi(argv[1]) > MAX_PHILOS)
		return (EINVAL);
	while (++i < count)
		if (!ft_atoi(argv[i]))
			return (EINVAL);
	return (EXIT_SUCCESS);
}

int	parse_args(int argc, char **argv)
{
	if (count_validation(argc))
		return (printf(ARGC_ERR), EINVAL);
	if (params_validation(argc, argv))
		return (printf(ARGV_ERR), EINVAL);
	return (EXIT_SUCCESS);
}
