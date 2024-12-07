#include "philo.h"

void cleanup_table(t_table *table)
{
    for (int i = 0; i < table->params.number_of_philosophers; i++)
        pthread_mutex_destroy(&table->forks[i]);

    free(table->forks);

    pthread_mutex_destroy(&table->writing_mutex);
    pthread_mutex_destroy(&table->data_mutex);

    free(table->philosophers);
}
