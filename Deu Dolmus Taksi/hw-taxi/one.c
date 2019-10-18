/*
driver -> barber
taxiStation -> waitingRoom
taxi's 4 seats -> barber's 1 seat
taxidriverPillow -> barberPillow
Student -> Customer

*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <time.h>

// max number of student threads
#define MAX_STUDENT 10

void *student(void * num);
void *driver(void *);
void randwait(int secs);


//define the semaphores
//waitingStation Limits the #of student allowed to waiting taxi station at same time
sem_t waitingStation;

//define the semaphores
//taxiSeats Limits the #of student allowed to enter the waiting taxi's seats at one time
sem_t taxiSeats;

//define driverPillow
// when there is no student then driver would like to take a nap
sem_t driverPillow;

//define there is no avaliable seat, student have to wait at the station, drivers done
sem_t seatBelt;

// flag to stop a driver threads when all students delivered faculty or library
int allDone = 0;

int main(int argc, char *argv[])
{
	pthread_t btid;
	pthread_t tid[MAX_STUDENT];

	int i, x, numStudents, numSeats; int Number[MAX_STUDENT];
	printf("Maximum number of student can only be 25.\n Enter of student number");
// \n Station volume.\n");
	scanf("%d",&x);
	numStudents = x;
	//scanf("%d",&x);
	numSeats = 4;
	//numSeats = x;

	if(numStudents > MAX_STUDENT)
 	{
		printf("The maximum number of Student is %d. \n",MAX_STUDENT);
		system("PAUSE");
		return 0;
 	}
	printf("A solution to the sleeping taxi driver problem using semaphores. \n");

	for(i = 0; i < MAX_STUDENT; i++)
	{
		Number[i] = i;	
	}
	// Initiaşize the semaphores with initial values...
	sem_init(&waitingStation, 0, numSeats);
	sem_init(&taxiSeats, 0, 1);
	sem_init(&driverPillow, 0, 0);
	sem_init(&seatBelt,0 ,0);

	//create a driver.
	pthread_create(&btid, NULL, driver, NULL);

	//create students.
	for (i = 0; i < numStudents; i++)
	{
		pthread_create(&tid[i], NULL, student, (void *)&Number[i]);
	}

	//Join each of the threads to wait for them to finish.
	for (i = 0; i < numStudents; i++)
	{
		pthread_join(tid[i], NULL);
	}

	//When all students in the station done, kill the driver thread.
	allDone = 1;
	sem_post(&driverPillow);
	pthread_join(btid,NULL);
	system("PAUSE");
	return 0;

}

void *student(void *number)
{
	int num = *(int *)number; //leave for the station with taxi and take some random amount of time to arrive.
	printf("Student %d taksi duragina gidiyor \n", num);
	randwait(5);
	printf("Student %d suan duraga geldi. \n", num);

	//space to open up in station...
	sem_wait(&waitingStation);

	printf("Student %d taxi bekleme duraginda bekliyor. \n", num);
	//printf("Student %d entering at station. \n", num);// wait for the taxi seat to become free
	sem_wait(&taxiSeats); //the seats are / seat is free so give up your spot in the waitingStation
	sem_post(&waitingStation); //wake up driver...

	
	printf("Student %d driver ı uyandırıyor.\n",num);
	sem_post(&driverPillow);  //wait to driver finish driving to your destination.

	sem_wait(&seatBelt); //give up seat.
	sem_post(&taxiSeats);
	printf("Student %d taksiyi terk ediyor. \n",num);//duragi?
}

void *driver(void *junk)
{
	while (!allDone) 
	{
		//sleep until someone arrives and wakes you..
		printf("Taksici uyuyor. \n");	
		int i = 0;
		//for (i = 0; i < 4; i++)
		//{
		sem_wait(&driverPillow); 
		//}
		
		//sem_wait(&driverPillow); // skip this stuff at the end..
		if (!allDone)
		{
			printf("Taksici taksiyi suruyor. \n");
			randwait(3);
			printf("Taksici hedefe ulasti.\n"); // release the students when done driving..
			sem_post(&seatBelt);
		}
		else{ printf("Taksici uyumaya gidiyor.\n");}
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
































