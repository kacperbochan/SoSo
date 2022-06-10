#ifndef MUTEX_H
#define MUTEX_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void * Auto(void *numer);
void * CondAuto(void *numer);

//mutexy, wykorzystywane kolejno do kontrolowania ruchu na moście i do kontrolowania danych w zmiennych
pthread_mutex_t mutex_most, mutex_dane;
//zmienna warunkowa, wykorzystywana do sygnalizowania zwolnienia mostu
pthread_cond_t cond_most;

//a_chilluje/b_chilluje - liczba samochodów w mieścia A/B, nie oczekująca w kolejce do mostu
//a_czeka/b_czeka - liczba samochodow w mieście A/B, oczekująca przy moście na wjazd
//car_on_bridge - numer auta, które aktualnie przejeżdża przez most
//bridge_way - miasto, z którego strony wjechało auto na most (0-A, 1-B)
int a_chilluje, a_czeka, b_czeka, b_chilluje, car_on_bridge, bridge_way;

#endif
