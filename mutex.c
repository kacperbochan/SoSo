#include "mutex.h"

//wyświetla obecny stan miast i mostu
void display(char color)
{
    switch(color){//dodano możliwość kolorowania tekstu w celu rozróżnienia 
        case 0://czarny
           printf("\033[1;30m");
        break;
        case 1://czerwony
           printf("\033[1;31m");
        break;
        case 2://zielony
           printf("\033[1;32m");
        break;
        case 3://żółty
           printf("\033[1;33m");
        break;
        case 4://niebieski
           printf("\033[1;34m");
        break;
        case 5://fioletowy
           printf("\033[1;35m");
        break;
        case 6://cyjan
           printf("\033[1;36m");
        break;
        case 7://biały
           printf("\033[1;37m");
        break;        
    }
    
    //jeżeli nie ma samochodu na moście
    if(car_on_bridge == -1)
    {
        printf("A-%d %d>>> [>><<] <<<%d %d-B\n", a_chilluje, a_czeka, b_czeka, b_chilluje);
        return;    
    }
    //jeśli jedzie z A do B
    if(bridge_way == 0)
        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);
    //jeśli jedzie z B do A
    else
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);

     printf("\033[0m");//reset koloru
}

//funkcja nadzorująca przebywanie samochodu w mieście po podanej stronie,
// odczekuje czas pobytu w mieście,
// po zakończeniu pobytu zmniejsza ilość osób w mieście,
// a zwiększa ilość osób w kolejce
void city(char strona){

    //przez 5-15 sekund śpi.
    sleep(rand()%11+5);

    //zmieniamy ilość czekających i miastowych po odpowiedniej stronie po czym wyświetlamy.
    //Jako że są to dane potencjalnie edytowane w innych miejscach, został użyty mutex
    pthread_mutex_lock(&mutex_dane);
        if(strona == 0){
            a_chilluje--;
            a_czeka++;
        }
        else{
            b_chilluje--;
            b_czeka++;
        }
        display(3);//informacja w kolorze żółtym
    pthread_mutex_unlock(&mutex_dane);
}

//funkcja nadzorująca przejazd samochodu,
// zmniejsza ilość czekających aut,
// odczekuje czas przejazdu,
// zwiększa ilość aut w mieście docelowym
// prezentuje rezultaty przy zmianach
void przejazd(){

    //zmieniamy ilość aut czekających i wyświetlamy obecny stan rzeczy
    pthread_mutex_lock(&mutex_dane);
        if(bridge_way==0)
            a_czeka--;
        else
            b_czeka--;
        display(6);//informacja w kolorze cyjan
    pthread_mutex_unlock(&mutex_dane);
    
    //funkcja symuluje czas przejazdu samochodu od 1 do 4 sekund
    sleep(rand()%4+1);

    //zwiększa ilość aut w mieście docelowym, sygnalizuje, że most jest wolny zmieniając car_on_bridge i wyświetla obecny stan rzeczy
    pthread_mutex_lock(&mutex_dane);
        car_on_bridge = -1;
        if(bridge_way == 0)
            b_chilluje++;
        else
            a_chilluje++;
        display(2);//informacja w kolorze zielonym
    pthread_mutex_unlock(&mutex_dane);
}

void *Auto(void *numer){

	int* num = (int *) numer;
    //losuje, z którego miasta startuje
    char strona = rand()%2; //0-A 1-B
    if(strona == 0)
        a_chilluje++;
    else
        b_chilluje++;

    //wypisanie informacji, o dołączeniu (mutex oznacza, że jest to wersja programu działająca jedynie na mutexach)
    printf("mutex dolacza - %d po stronie %c\n", *num, strona==0?'A':'B');
	
	while(1){
		
        city(strona);//samochód jest w mieście i nie czeka jeszcze w kolejce

        //do momentu aż nie wejdzie do poniższej sekcji, stoi w kolejce
        pthread_mutex_lock(&mutex_most);//wejście tu oznacza rozpoczęcie przejazdu konkretnego samochodu przez most
            car_on_bridge = *num;//oznaczamy który samochód przejeżdża
            bridge_way = strona;//oznaczamy w jakim kierunku przejeżdża (na bazie tego obierany jest kierunek strzałek w statusie oraz określa które dane będą edytowane)
            przejazd(strona);//pojazd przejeżdża przez most
            strona ^= 1; //pojazd zmienił stronę
        pthread_mutex_unlock(&mutex_most);//koniec przejazdu - most się zwolnił
    }
}

void *CondAuto(void *numer){

	int* num = (int *) numer;
    char strona = rand()%2; //0-A 1-B
    //losuje, z którego miasta startuje
    if(strona == 0)
        a_chilluje++;
    else
        b_chilluje++;

    //wypisanie informacji, o dołączeniu (conditional oznacza, że jest to wersja programu działająca na zmiennych warunkowych)
    printf("conditional dolacza - %d po stronie %c\n", *num, strona==0?'A':'B');
	
	while(1){
		
        city(strona);//samochód jest w mieście i nie czeka jeszcze w kolejce

        //do momentu aż nie wejdzie do poniższej sekcji, stoi w kolejce
        pthread_mutex_lock(&mutex_most);//wejście tu oznacza rozpoczęcie przejazdu konkretnego samochodu przez most
            while(car_on_bridge !=-1){//dopóki most nie jest wolny
                pthread_cond_wait(&cond_most,&mutex_most);//czekaj na sygnał od innego wątku który oznaczał by zwolnienie się mostu
            }
            car_on_bridge = *num;//oznaczamy który samochód przejeżdża
            bridge_way = strona;//oznaczamy w jakim kierunku przejeżdża (na bazie tego obierany jest kierunek strzałek w statusie oraz określa które dane będą edytowane)
            przejazd(); //samochód przejeżdża most
            strona ^= 1; //pojazd zmienił stronę
        pthread_mutex_unlock(&mutex_most);//koniec przejazdu - most się zwolnił
        pthread_cond_signal(&cond_most);//informujemy jeden z oczekujących wątków o dostępności mostu
    }
}
