#include<stdio.h>
#include<semaphore.h>
#include<sys/shm.h>
#include<pthread.h>
#include<fcntl.h>


char ptr[30];
char ptr2[30];

int ind=0;
FILE* fp;
FILE* fp2;

int main() {
	sem_t* reader1;
	sem_t* reader2;
	sem_t* writer1;
	sem_t* writer2;
	writer1=sem_open("sem1",O_CREAT|O_EXCL,0644,1); //writes in buffer 1 in process A&B
	reader1=sem_open("sem2",O_CREAT|O_EXCL,0644,0);; //reads in buffer 1 in process C 
	reader2=sem_open("sem3",O_CREAT|O_EXCL,0644,0);; // reads in buffer 2 in process D
	writer2=sem_open("sem4",O_CREAT|O_EXCL,0644,0);; // writes in buffer 2 process C 
	int pid = fork();
	if (pid == 0) {
		//process A

		fp = fopen("file1.txt", "r");
		sem_wait(writer1); //block writer 1 in process B
		int i = 0;
		while (i < 10) {
			ptr[ind++] = fgetc(fp);
			i++;
		printf("%c",ptr[ind-1]);
		}

		fclose(fp);
		sem_post(writer1);
		sem_post(writer2);
		printf("posting writer 2 ");
		//int val;
		//sem_getvalue(&writer2,&val);
		//printf("%d",val);

	}
	else if (pid > 0) {
		int pid2 = fork();
		if (pid2 == 0) {
			//process B
			sem_wait(writer1);

			fp2 = fopen("file2.txt", "r");
			int i = 0;
			while (i < 10) {
				ptr[ind++] = fgetc(fp2);
				i++;
			printf("%c",ptr[ind-1]);
			}
			
			sem_post(writer1);
			sem_post(reader1);
			printf("posting reader 1 ");
			int val;
			sem_getvalue(reader1,&val);
			printf("%d",val);
			
		fclose(fp2);
		}
		else if (pid2 > 0) {
			int pid3 = fork();
			if (pid3 == 0) {
			//process C
			int val;
				sem_wait(reader1);
				sem_wait(writer2);
				ptr[ind] = '\0';
				int i = 0;
				while (ptr[i] != '\0') {
					ptr2[i] = ptr[i];
					printf("%c",ptr2[i]);
					i++;
				} 

				sem_post(reader2);

			}
			else if (pid3 > 0) {
			printf("waiting... in D   ");
			int pid4 = fork();
				if (pid4 == 0) {
					//process D
					sem_wait(reader2);
					int i = 0;
					printf("%s", ptr2);
					sem_post(reader2);
				}
				else if(pid4>0){		
				wait(NULL);
				printf(" \nexiting... \n");}


			}
		}
	}
sem_destroy(reader1);
sem_destroy(reader2);
sem_destroy(writer1);
sem_destroy(writer2);
	return 0;
}
