#include "philo.h"
#include <ctype.h>
#include <sys/time.h>

bool is_positive_integer(char *str)
{
    if (!str || !*str)
        return false;
    for (int i = 0; str[i]; i++) {
        if (!isdigit((unsigned char)str[i]))
            return false;
    }
    return true;
}

bool parse_arguments(int argc, char **argv, t_params *params)
{
    if (argc != 5 && argc != 6)
        return false;
    for (int i = 1; i < argc; i++) {
        if (!is_positive_integer(argv[i]))
            return false;
    }

    params->number_of_philosophers = atoi(argv[1]);
    params->time_to_die = atol(argv[2]);
    params->time_to_eat = atol(argv[3]);
    params->time_to_sleep = atol(argv[4]);
    if (argc == 6) {
        params->number_of_times_each_philosopher_must_eat = atoi(argv[5]);
        params->must_eat_count = true;
    } else {
        params->must_eat_count = false;
    }

    return true;
}

long get_timestamp_in_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void set_death(t_table *table)
{
    pthread_mutex_lock(&table->data_mutex);
    table->someone_died = true;
    pthread_mutex_unlock(&table->data_mutex);
}

bool has_someone_died(t_table *table)
{
    bool died;

    pthread_mutex_lock(&table->data_mutex);
    died = table->someone_died;
    pthread_mutex_unlock(&table->data_mutex);
    return died;
}
