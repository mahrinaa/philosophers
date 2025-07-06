/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:27 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 01:25:29 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_current_time_in_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("error with function gettimeofday\n");
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_error_and_free(char *msg, t_rules *rules)
{
	free_rules(rules);         // Libère tout ce qui a déjà été initialisé
	exit_print_error(msg);     // Affiche et quitte
}

void	exit_print_error(char *msg)
{
	int	i;

	i = 0;
	while (msg && msg[i])
	{
		write(STDERR_FILENO, &msg[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}


int	ft_atoi(const char *str)
{
	size_t	i;
	size_t	nb;
	size_t	sign;

	i = 0;
	nb = 0;
	sign = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
	{
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sign *= -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		nb = nb * 10 + (str[i] - '0');
		i++;
	}
	return (nb * sign);
}

void	ft_putstr_fd(char *str, int fd)
{
	size_t	i;

	i = 0;
	if (str == 0 || fd < 0)
		return ;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

