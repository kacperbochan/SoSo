@@ - 1, 7 + 1, 60 @@
#include <stdio.h>
#include <pthread.h>

int main
int a_chilluje, a_czeka, b_czeka, b_chilluje;

void* Auto(void* numer) {

    int num = (int*)numer
        Bool strona = 0; //0-A 1-B
    while (1) {
        //faza nic nie robienia
        sleep(rand() % 20 + 5)
            //faza nic nie robienia - koniec



            //faza czekania
            lock(mutex_dane);
        if (strona == 0) {
            a_chilluje�;
            a_czeka++;
        }
        else {
            b_chilluje�;
            b_czeka++;
        }
        unlock(mutex_dane);

        lock(mutex_most);
        sleep(rand() % 4 + 1);
        if (strona == 0) {
            lock(mutex_dane);
            printf(�A - % d % d >> > [>> % d >> ] << < % d % d - B �, a_chilluje, a_czeka, *num, b_czeka, b_chilluje);
            unlock(mutex_dane);
        }
        else {
            lock(mutex_dane);
            printf(�A - % d % d >> > [<< % d << ] << < % d % d - B �, a_chilluje, a_czeka, *num, b_czeka, b_chilluje);
            unlock(mutex_dane);
        }
        strona ^= 1; //zmienia stron�

        lock(mutex_dane);
        if (strona == 0) {
            b_czeka�;
            a_chilluj++;
        }
        else {
            a_czeka�;
            b_chilluj++;
        }
        unlock(mutex_dane);
        unlock(mutex_most);
    }

    int main(int argc, char** argv)
    {

        //pobieranie liczby aut
        car_number = atoi(argv[1]);
        
        
        printf("Hello");
        return 0;
    }