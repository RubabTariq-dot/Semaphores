#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

int potentialCPatients=0;
int coronaPatients=0;
int fluPatients=0;
int recoveredPatients=0;
sem_t coronaPatient;
sem_t fluPatient;

int test(){
return (rand()%2)+(rand()%2);
}

int checkRecovery(){
return (rand()%2)+(rand()%2);
}
 
void* process(void* arg){
potentialCPatients++;

int patient=rand()%2;

if(patient==0){ //has flu
	fluPatients++;
	potentialCPatients--;
	sem_post(&fluPatient);
}

else if(patient==1){ //has corona
	coronaPatients++;
	potentialCPatients--;
	sem_post(&coronaPatient);

	if(checkRecovery()==0){ //has recovered
		recoveredPatients++;
		sem_wait(&coronaPatient);

	}
}


}

int main(){


int N;
printf("Enter the number of patients. \n");
scanf("%d",&N);
printf("----------COVID MANAGEMENT SYSTEM------------- \n");
printf("Total Number of Patients are: %d \n",N);

sem_init(&coronaPatient,0,1);
sem_init(&fluPatient,0,1);

pthread_t id;
int i=0;
for(i=0;i<N;i++){
pthread_create(&id,NULL,&process,NULL);
pthread_join(id,NULL);
}
printf("Number of Flu Patients are: %d \n",fluPatients);
printf("Number of Corona Patients are: %d \n",coronaPatients);
printf("Number of Recovered Patients are: %d \n",recoveredPatients);
sem_destroy(&coronaPatient);
sem_destroy(&fluPatient);

return 0;
}
