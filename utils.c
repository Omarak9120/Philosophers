// utils.c
#include "philo.h"

/* Function to check if a string is a positive integer */
bool is_positive_integer(char *str)
{
    int i = 0;

    if (!str)
        return false;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

/* Function to convert string to long and validate */
bool parse_arguments(int argc, char **argv, t_params *params)
{
    if (argc != 5 && argc != 6)
        return false;
    for (int i = 1; i < argc; i++)
    {
        if (!is_positive_integer(argv[i]))
            return false;
    }
    params->number_of_philosophers = atoi(argv[1]);
    params->time_to_die = atol(argv[2]);
    params->time_to_eat = atol(argv[3]);
    params->time_to_sleep = atol(argv[4]);
    if (argc == 6)
    {
        params->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
        params->must_eat_count = true;
    }
    else
        params->must_eat_count = false;
    return true;
}
// utils.c (add the following function)
long get_timestamp_in_ms(void)
{
    struct timeval tv;
    long milliseconds;

    gettimeofday(&tv, NULL);
    milliseconds = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return milliseconds;
}
