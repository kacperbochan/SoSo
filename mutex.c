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
    
    if(car_on_bridge == -1)
    {
        printf("A-%d %d>>> [>><<] <<<%d %d-B\n", a_chilluje, a_czeka, b_czeka, b_chilluje);
        return;    
    }
    if(bridge_way == 0)
        printf("A-%d %d>>> [>> %d >>] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);
    else
        printf("A-%d %d>>> [<< %d <<] <<<%d %d-B\n", a_chilluje, a_czeka, car_on_bridge, b_czeka, b_chilluje);

     printf("\033[0m");//reset koloru
}

//funkcja nadzorująca przebywanie samochodu w mieście po podanej stronie,
// odczekuje czas pobytu w mieście,
// po zakończeniu pobytu zmniejsza ilość osób w mieście,
// a zwiększa ilość osób w kolejce
void city(char strona){

    //przez 1-10 sekund śpi.
    sleep(rand()%10+1);

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

    //zwiększa ilość aut w mieście docelowym i wyświetlamy obecny stan rzeczy
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
    char strona = 0; //0-A 1-B

    printf("dolacza -%d\n", *num);
	
	while(1){
		
        city(strona);

        //do momentu aż nie wejdzie do poniższej sekcji, stoi w kolejce
        pthread_mutex_lock(&mutex_most);//wejście tu oznacza rozpoczęcie przejazdu konkretnego samochodu przez most
            car_on_bridge = *num;//oznaczamy który samochód przejeżdża
            bridge_way = strona;//oznaczamy w jakim kierunku przejeżdża (na bazie tego obierany jest kierunek strzałek w statusie oraz określa które dane będą edytowane)
            przejazd(strona);//pojazd przejeżdża prze most
            strona ^= 1; //pojazd zmienił stronę
        pthread_mutex_unlock(&mutex_most);//koniec przejazdu - most się zwolnił
    }
}

void *CondMost(){
    while(1){//most działa zawsze
        if(car_on_bridge !=-1){//wykona się tylko jeśli mamy kogoś próbującego przejechać
            pthread_mutex_lock(&mutex_most);//gdy tu wchodzimy oznacza to, że coś jedzie mostem
                przejazd();
            pthread_mutex_unlock(&mutex_most);//zakończyliśmy przejazd mostem
            pthread_cond_signal(&cond_most);//informujemy jeden z oczekujących wątków o dostępności mostu
        }
    }
}

void *CondAuto(void *numer){

	int* num = (int *) numer;
    char strona = 0; //0-A 1-B

    printf("dolacza -%d\n", *num);
	
	while(1){
		
        city(strona);

        //do momentu aż nie wejdzie do poniższej sekcji, stoi w kolejce
        pthread_mutex_lock(&mutex_most);//wejście tu oznacza rozpoczęcie przejazdu konkretnego samochodu przez most
            while(car_on_bridge !=-1){//dopóki most nie jest wolny
                pthread_cond_wait(&cond_most,&mutex_most);//czekaj na sygnał od innego wątku który oznaczał by zwolnienie się mostu
            }
            car_on_bridge = *num;//oznaczamy który samochód przejeżdża
            bridge_way = strona;//oznaczamy w jakim kierunku przejeżdża (na bazie tego obierany jest kierunek strzałek w statusie oraz określa które dane będą edytowane)
            
            strona ^= 1; //pojazd zmienił stronę
        pthread_mutex_unlock(&mutex_most);//koniec przejazdu - most się zwolnił
    }
}
