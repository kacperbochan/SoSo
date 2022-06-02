#include <stdio.h>
#include <pthread.h>

int a_chilluje, a_czeka, b_czeka, b_chilluje;  

void * Auto(void *numer){

	int num = (int *) numer
	Bool strona = 0; //0-A 1-B
	while(1){
		//faza nic nie robienia
    sleep(rand()%20+5)
        //faza nic nie robienia - koniec



        //faza czekania
    lock(mutex_dane);
        if(strona == 0){
			a_chilluje–;
			a_czeka++;
		}
		else{
			b_chilluje–;
			b_czeka++;
		}
    unlock(mutex_dane);

    lock(mutex_most);
	    sleep(rand()%4+1);
	    if(strona==0){
            lock(mutex_dane);
                printf(“A-%d %d>>> [>> %d >>] <<<%d %d-B ”, a_chilluje, a_czeka, *num ,b_czeka, b_chilluje);
            unlock(mutex_dane);
        }
        else{
            lock(mutex_dane);
                printf(“A-%d %d>>> [<< %d <<] <<<%d %d-B ”, a_chilluje, a_czeka, *num ,b_czeka, b_chilluje);
            unlock(mutex_dane);
        }
        strona ^= 1; //zmienia stronę

        lock(mutex_dane);
            if(strona == 0){
                b_czeka–;
                a_chilluj++;
            }
            else{
                a_czeka–;
                b_chilluj++;
            }
        unlock(mutex_dane);
    unlock(mutex_most);
}

int main(int argc, char** argv)
{
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