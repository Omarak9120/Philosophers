// init.c
#include "philo.h"

/* Initialize mutexes for forks */
bool init_forks(t_table *table)
{
    int i;

    table->forks = malloc(sizeof(pthread_mutex_t) * table->params.number_of_philosophers);
    if (!table->forks)
        return false;
    for (i = 0; i < table->params.number_of_philosophers; i++)
    {
        if (pthread_mutex_init(&table->forks[i], NULL) != 0)
            return false;
    }
    return true;
}

/* Initialize philosophers */
bool init_philosophers(t_table *table)
{
    int i;

    table->philosophers = malloc(sizeof(t_philosopher) * table->params.number_of_philosophers);
    if (!table->philosophers)
        return false;
    for (i = 0; i < table->params.number_of_philosophers; i++)
    {
        table->philosophers[i].id = i + 1;
        table->philosophers[i].left_fork = &table->forks[i];
        table->philosophers[i].right_fork = &table->forks[(i + 1) % table->params.number_of_philosophers];
        table->philosophers[i].last_meal_time = 0;
        table->philosophers[i].meals_eaten = 0;
        table->philosophers[i].table = table;
    }
    return true;
}

/* Initialize other mutexes */
bool init_mutexes(t_table *table)
{
    if (pthread_mutex_init(&table->writing_mutex, NULL) != 0)
        return false;
    if (pthread_mutex_init(&table->death_mutex, NULL) != 0)
        return false;
    return true;
}

/* Initialize the entire table */
bool initialize_table(t_table *table)
{
    if (!init_forks(table))
        return false;
    if (!init_philosophers(table))
        return false;
    if (!init_mutexes(table))
        return false;
    table->start_time = get_timestamp_in_ms(); // Function to be implemented in utils.c
    table->someone_died = false;
    return true;
}
