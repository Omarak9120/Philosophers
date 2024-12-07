#include "philo.h"
#include <string.h>

int main(int argc, char **argv)
{
    t_table *table;

    table = malloc(sizeof(t_table));
    if (!table) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }
    memset(table, 0, sizeof(t_table));

    if (!parse_arguments(argc, argv, &table->params)) {
        printf("Error: Invalid arguments.\n");
        free(table);
        return 1;
    }

    if (!initialize_table(table)) {
        printf("Error: Initialization failed.\n");
        free(table);
        return 1;
    }

    if (!create_philosopher_threads(table)) {
        printf("Error: Thread creation failed.\n");
        cleanup_table(table);
        free(table);
        return 1;
    }

    if (!monitor_philosophers(table)) {
        printf("Error: Monitoring failed.\n");
    }

    for (int i = 0; i < table->params.number_of_philosophers; i++)
        pthread_join(table->philosophers[i].thread, NULL);

    cleanup_table(table);
    free(table);
    return 0;
}
