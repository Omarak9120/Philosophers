#include "philo.h"

bool init_forks(t_table *table)
{
    table->forks = malloc(sizeof(pthread_mutex_t) * table->params.number_of_philosophers);
    if (!table->forks) {
        printf("Error: Memory allocation for forks failed.\n");
        return false;
    }
    for (int i = 0; i < table->params.number_of_philosophers; i++) {
        if (pthread_mutex_init(&table->forks[i], NULL) != 0) {
            for (int j = 0; j < i; j++)
                pthread_mutex_destroy(&table->forks[j]);
            free(table->forks);
            return false;
        }
    }
    return true;
}

bool init_philosophers(t_table *table)
{
    table->philosophers = malloc(sizeof(t_philosopher) * table->params.number_of_philosophers);
    if (!table->philosophers) {
        printf("Error: Memory allocation for philosophers failed.\n");
        return false;
    }
    for (int i = 0; i < table->params.number_of_philosophers; i++) {
        table->philosophers[i].id = i + 1;
        table->philosophers[i].left_fork = &table->forks[i];
        table->philosophers[i].right_fork = &table->forks[(i + 1) % table->params.number_of_philosophers];

        // Protect meals_eaten and last_meal_time with data_mutex
        pthread_mutex_lock(&table->data_mutex);
        table->philosophers[i].last_meal_time = table->start_time;
        table->philosophers[i].meals_eaten = 0;
        pthread_mutex_unlock(&table->data_mutex);

        table->philosophers[i].table = table;
    }
    return true;
}

bool init_mutexes(t_table *table)
{
    if (pthread_mutex_init(&table->writing_mutex, NULL) != 0)
        return false;
    if (pthread_mutex_init(&table->data_mutex, NULL) != 0) {
        pthread_mutex_destroy(&table->writing_mutex);
        return false;
    }
    return true;
}

bool initialize_table(t_table *table)
{
    if (!init_forks(table))
        return false;

    table->start_time = get_timestamp_in_ms();

    // Protect someone_died under data_mutex
    pthread_mutex_lock(&table->data_mutex);
    table->someone_died = false;
    pthread_mutex_unlock(&table->data_mutex);

    if (!init_philosophers(table))
        return false;

    if (!init_mutexes(table))
        return false;

    return true;
}
