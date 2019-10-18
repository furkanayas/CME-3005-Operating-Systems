//libraries

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>


void *ogrenci(void * num);
void *taksici(void *);
void randwait(int secs);

void *taksikoltuk(void * num);


sem_t waitingInTaxi;

//sem_t takssikoltuksayisi;

sem_t taksicininyatagi;

sem_t kapikilidi;

int hepsibitti = 0;

int main(int argc, char *argv[])
{
	int i, x, sayiOgrenci, sayiKoltuk;
	sayiOgrenci = 8;
	sayiKoltuk = 4;
	pthread_t btid;
	pthread_t tid[sayiOgrenci]; //8 ogrenci

//	pthread_t btid;
//	pthread_t tid[sayiKoltuk]; //4 koltuk
	
	int Sayi1[sayiOgrenci];
	int Sayi2[sayiKoltuk];
	

	printf("Ogrenci sayısı 8, taksi koltugu 4 olarak belirlendi.\n Fazlalık kontrolu yok. \n Semaphore basliyor");
	

	for(i = 0; i < sayiOgrenci; i++)
	{
		Sayi1[i] = i;	
	}

	for(i = 0; i < sayiKoltuk; i++)
	{
		Sayi2[i] = i;	
	}
	
	sem_init(&waitingInTaxi, 0, sayiKoltuk);
	sem_init(&taksikoltuk, 0, 1);
	sem_init(&taksicininyatagi,0 ,0);;
	sem_init(&kapikilidi,0 ,0);


	//sem_init(&taksikoltuksayisi, 0, sayiKoltuk);
	
	
	
	
	pthread_create(&btid, NULL, taksici, NULL);

	for (i = 0; i < sayiOgrenci; i++)
	{
		pthread_create(&tid[i], NULL, ogrenci, (void *)&Sayi1[i]);
	}

	for (i = 0; i < sayiOgrenci; i++)
	{
		pthread_join(tid[i], NULL);
	}

	for (i = 0; i < sayiKoltuk; i++)
	{
		pthread_create(&tid[i], NULL, taksikoltuk, (void *)&Sayi2[i]);
	}

	for (i = 0; i < sayiKoltuk; i++)
	{
		pthread_join(tid[i], NULL);
	}
	
	hepsibitti = 1;
	sem_post(&taksicininyatagi);
	pthread_join(btid,NULL);
	system("PAUSE");
	return 0;
/*
sem_t taksikoltuksayisi;

sem_t taksicininyatagi;

sem_t kapikilidi;

int i, x, sayiOgrenci, sayiKoltuk;
	sayiOgrenci = 8;
	sayiKoltuk = 4;

*/
	
}

void *taksikoltuk(void *number)
{
	int num = *(int *)number;
	printf("Ogrenci %d taksinin %d koltuguna oturuyor \n", num);
	randwait(3);
	sem_post(&waitingInTaxi);
//	sem_post(&taksikoltuksayisi);

}





void *ogrenci(void *number)
{
	int num = *(int *)number;
	printf("Ogrenci %d taksi duragina yuruyor \n", num);
	randwait(5);
	printf("Ogrenci %d duraga ulasti. \n", num);
	sem_wait(&waitingInTaxi);

	//sem_wait(&taksikoltuksayisi);
	printf("Student %d taxi bekleme duraginda bekliyor. \n", num);
	
//	printf("ogrenci %d taksiciyi uyandırıyor\n");
	sem_wait(&taksikoltuk);
	sem_post(&taksicininyatagi);
	printf("ogrenci %d taksiciyi uyandırıyor\n");

	
		sem_wait(&kapikilidi);
		sem_post(&waitingInTaxi);
	//	sem_post(&taksikoltuksayisi);
	
		printf("ogrenci %d aracı terk ediyor??\n");	
}

void *taksici(void *junk)
{
	while(!hepsibitti)
   {
	printf("Taksici uyuyor\n");
	sem_wait(&taksicininyatagi);
	if(!hepsibitti)
	{
	printf("taksici suruyor\n");
	randwait(3);
	printf("Taksici ogrencileri hedefe ulastırdı\n");
	sem_post(&kapikilidi);
	}
	else
	{
	printf("taksici mesaiyi bitirdi");
	}
   }

}


void randwait(int secs)
{
	int len = 1; // generate an arbit number...
	sleep(len);
}

void randstudent(int secs)
{
	int len = 1; // generate an arbit number...
	sleep(len);
}
