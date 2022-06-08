#ifndef MUTEX_H
#define MUTEX_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void * Auto(void *numer);
pthread_mutex_t mutex_most, mutex_dane;
int a_chilluje, a_czeka, b_czeka, b_chilluje, car_on_bridge, bridge_way;

#endif