#include <stdio.h>
#include <pthread.h>
#include "mutex.h"

int main(int argc, char** argv)
{
    if (pthread_mutex_init(&mutex_most, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    if (pthread_mutex_init(&mutex_dane, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    pthread_cond_init(&cond_most, NULL);

    //pobieranie liczby aut
    int car_number = atoi(argv[1]);
    int* liczby = (int*)malloc(sizeof(int)*car_number);
    //wszystkie auta czekają ze strony A
    a_chilluje = car_number;
    car_on_bridge = -1; 

    pthread_t cars[car_number];
    pthread_t bridge;

    for(int i = 0; i<car_number; i++){
        liczby[i] = i;
        if ( pthread_create(&cars[i], NULL, CondAuto, &liczby[i]) != 0 ) 
        { 
            perror("Could not create thread auto."); 
        }
    }
    for(int i = 0; i<car_number; i++){
        if (pthread_join(cars[i], NULL) != 0) { 
            perror("Could not join thread auto."); 
        }
    }
    
    return 0;
}
