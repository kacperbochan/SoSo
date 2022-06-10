#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "mutex.h"

int main(int argc, char** argv)
{
    //rand seed
    srand(time(NULL));

    //sprawdzenie czy przyjeliśmy tylko jeden argument
    if (argc > 2) {
        perror("You can't put more than 1 argument");
        exit(EXIT_FAILURE);
    }

    //inicjalizacja mutexu mostu
    if (pthread_mutex_init(&mutex_most, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    //inicjalizacja mutexu danych
    if (pthread_mutex_init(&mutex_dane, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }
    //inicjalizacja zmiennej warunkowej cond_most
    pthread_cond_init(&cond_most, NULL);

    //pobieranie liczby aut
    int car_number = atoi(argv[1]);
    
    //sprawdzenie czy podana liczba aut jest większa od 0
    if (car_number < 1) {
        perror("Car number cannot be less than 1");
        exit(EXIT_FAILURE);
    }

    //tablica liczb, wykorzystywana do numerowania wątków aut
    int* liczby = (int*)malloc(sizeof(int)*car_number);
    //inicjalizacja zmiennych oznaczających liczby aut czekających przy moście i przebywających w mieście
    a_chilluje = 0;
    b_chilluje = 0;
    a_czeka = 0;
    b_czeka = 0;
    //inicjalizacja zmiennej oznaczającej jakie auto aktualnie przejeżdża przez most
    car_on_bridge = -1; 

    //tablica wątków aut
    pthread_t cars[car_number];

    //tworzenie wątków aut z przekazaniem odpowiedniej liczby jako parametr funkcji Auto/CondAuto
    for(int i = 0; i<car_number; i++){
        liczby[i] = i;
        //zamiennie jako argument można przekazać funkcję Auto(same mutexy) lub CondAuto(zmienne warunkowe)
        if ( pthread_create(&cars[i], NULL, CondAuto, &liczby[i]) != 0 )
        { 
            perror("Could not create thread auto."); 
        }
    }
    //przyłączenie wątków
    for(int i = 0; i<car_number; i++){
        if (pthread_join(cars[i], NULL) != 0) { 
            perror("Could not join thread auto."); 
        }
    }
    
    return 0;
}
