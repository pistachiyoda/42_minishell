#include "minishell.h"


int	is_space_for_atol(char c)
{
	if (c == '\t')
		return (1);
	if (c == '\n')
		return (1);
	if (c == '\v')
		return (1);
	if (c == '\f')
		return (1);
	if (c == '\r')
		return (1);
	if (c == ' ')
		return (1);
	return (0);
}

// unsigned long long 18446744073709551615
long long	ft_atol(const char *str, bool *is_invalid)
{
	long long			sign;
	unsigned long long	n;

	while (is_space_for_atol(*str))
		str++;
	if (*str == '-')
		sign = (long long)-1;
	else
		sign = (long long)1;
	if (*str == '-' || *str == '+')
		str++;
	n = 0;
	while (ft_isdigit(*str))
	{
		n = (unsigned long long)10 * n + (unsigned long long)(*str - '0');
		printf("n = %llu\n", n);
		if (sign == 1 && n > (unsigned long long)LLONG_MAX)
		{
			*is_invalid = true;
			return (0);
		}
		if (sign == (long long)-1 && (n > ((unsigned long long)LLONG_MAX + (unsigned long long)1)))
		{
			*is_invalid = true;
			return (0);
		}
		str++;
	}
	*is_invalid = false;
	return (sign * (long long)n);
}
