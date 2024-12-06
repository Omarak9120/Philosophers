#include "philo.h"
#include <stdio.h>
#include <string.h>

/*
 * main.c
 *
 * This file:
 * - Parses arguments
 * - Initializes the simulation table
 * - Creates philosopher threads
 * - Starts the monitor thread
 * - Waits for termination and cleans up
 */

int main(int argc, char **argv)
{
    t_table table;

    memset(&table, 0, sizeof(t_table));

    // Parse arguments
    if (!parse_arguments(argc, argv, &table.params))
    {
        printf("Error: Invalid arguments.\n");
        return 1;
    }

    // Initialize all resources (forks, philosophers, mutexes)
    if (!initialize_table(&table))
    {
        printf("Error: Initialization failed.\n");
        return 1;
    }

    // Create philosopher threads
    if (!create_philosopher_threads(&table))
    {
        printf("Error: Thread creation failed.\n");
        return 1;
    }

    // Start monitor thread, which will:
    // - Stop when a philosopher dies OR
    // - Stop when all philosophers have eaten enough times (if must_eat_count is set)
    if (!monitor_philosophers(&table))
    {
        printf("Error: Monitoring failed.\n");
        return 1;
    }

    // Join philosopher threads once monitoring is done
    for (int i = 0; i < table.params.number_of_philosophers; i++)
        pthread_join(table.philosophers[i].thread, NULL);

    // Cleanup allocated resources
    cleanup_table(&table);

    return 0;
}
