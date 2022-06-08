#include "mutex.h"

void display()
{
    if(car_on_bridge == -1)
    {
        printf("A-%d %d>>> [>><<] <<<%d %d-B\n", a_chilluje, a_czeka, b_czeka, b_chilluje);
        return;    
    }
    if(bridge_way == 0)
        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);
    else
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);
}

void * Auto(void *numer){

	int* num = (int *) numer;

    printf("dolacza -%d \n ", *num);
	char strona = 0; //0-A 1-B
	while(1){
		//faza nic nie robienia
        sleep(rand()%10+1);
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
        display();
        pthread_mutex_unlock(&mutex_dane);

        pthread_mutex_lock(&mutex_most);
        
        if(strona==0){
            pthread_mutex_lock(&mutex_dane);
            a_czeka--;
            car_on_bridge = *num;
            bridge_way = strona;
            display();
            pthread_mutex_unlock(&mutex_dane);
        }
        else{
            pthread_mutex_lock(&mutex_dane);
            b_czeka--;
            car_on_bridge = *num;
            bridge_way = strona;
            display();
            pthread_mutex_unlock(&mutex_dane);
        }
        
        sleep(rand()%4+1);

        strona ^= 1; //zmienia stronę

        pthread_mutex_lock(&mutex_dane);
        car_on_bridge = -1;
        if(strona == 0){
            a_chilluje++;
        }
        else{
            b_chilluje++;
        }
        pthread_mutex_unlock(&mutex_dane);

        pthread_mutex_unlock(&mutex_most);
    }
}