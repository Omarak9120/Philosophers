// cleanup.c
#include "philo.h"

/* Destroy mutexes and free allocated memory */
void cleanup_table(t_table *table)
{
    int i;

    // Destroy fork mutexes
    for (i = 0; i < table->params.number_of_philosophers; i++)
        pthread_mutex_destroy(&table->forks[i]);
    free(table->forks);

    // Destroy other mutexes
    pthread_mutex_destroy(&table->writing_mutex);
    pthread_mutex_destroy(&table->death_mutex);

    // Free philosophers
    free(table->philosophers);
}
