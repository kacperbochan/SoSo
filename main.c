#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex_most, mutex_dane;

int a_chilluje, a_czeka, b_czeka, b_chilluje;  

void * Auto(void *numer){

	int* num = (int *) numer;

    printf("dolacza -%d \n ", *num);
	char strona = 0; //0-A 1-B
	while(1){
		//faza nic nie robienia
    sleep(rand()%20+5);
        //faza nic nie robienia - koniec



        //faza czekania
    pthread_mutex_lock(&mutex_dane);
        if(strona == 0){
			a_chilluje--;
			a_czeka++;
		}
		else{
			b_chilluje--;
			b_czeka++;
		}
    pthread_mutex_unlock(&mutex_dane);

    pthread_mutex_lock(&mutex_most);
	    sleep(rand()%4+1);
	    if(strona==0){
            pthread_mutex_lock(&mutex_dane);
                printf("A-%d %d>>> [>> %d >>] <<<%d %d-B ", a_chilluje, a_czeka, *num ,b_czeka, b_chilluje);
            pthread_mutex_unlock(&mutex_dane);
        }
        else{
            pthread_mutex_lock(&mutex_dane);
                printf("A-%d %d>>> [<< %d <<] <<<%d %d-B ", a_chilluje, a_czeka, *num ,b_czeka, b_chilluje);
            pthread_mutex_unlock(&mutex_dane);
        }
        strona ^= 1; //zmienia stronę

        pthread_mutex_lock(&mutex_dane);
            if(strona == 0){
                b_czeka--;
                a_chilluje++;
            }
            else{
                a_czeka--;
                b_chilluje++;
            }
        pthread_mutex_unlock(&mutex_dane);
    pthread_mutex_unlock(&mutex_most);
    }
}

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

    //pobieranie liczby aut
    int car_number = atoi(argv[1]);
    int* liczby = (int*)malloc(sizeof(int)*car_number);
    //wszystkie auta czekają ze strony A
    a_chilluje = car_number; 

    pthread_t cars[car_number];

    for(int i = 0; i<car_number; i++){
        liczby[i] = i;
        int result = pthread_create(&cars[i], NULL, Auto, &liczby[i]);
        if (result != 0) { perror("Could not create thread."); }
    }
    for(int i = 0; i<car_number; i++){
        int result = pthread_join(cars[i], NULL);
        if (result != 0) { perror("Could not create thread."); }
    }
    
    printf("Hello\n");
    return 0;
}
