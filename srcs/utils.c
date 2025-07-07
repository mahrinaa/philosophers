/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mapham <mapham@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 06:52:27 by mapham            #+#    #+#             */
/*   Updated: 2025/07/07 13:38:38 by mapham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	display_action(t_philo *philo, const char *msg)
{
	long long	time;

	pthread_mutex_lock(&philo->rules->print_mutex);
	if (!check_death_status(philo->rules))
	{
		time = get_current_time_in_ms() - philo->rules->start_time;
		printf("%lld %d %s\n", time, philo->id, msg);
	}
	pthread_mutex_unlock(&philo->rules->print_mutex);
}

long long	get_current_time_in_ms(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("error with function gettimeofday\n");
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_error_and_free(char *msg, t_rules *rules)
{
	free_rules(rules);
	exit_print_error(msg);
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

