#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h> 
#include<string.h>

pthread_mutex_t lock;

struct threadArguments{
	int start;
    	int end;
	int numOfPrimes;
	char fileName[15];
	char fileOutName[15];	
};

void *primeThread(void *p){
	struct threadArguments *primeArgs = p;
	FILE *fptr, *fptro;	
	/* read the file from lines start to end
	for each of those lines, check if the number is prime or not (it's factors are 1 and its self)
	if it is prime, add it to the array and increase the counter*/
	int y, lineNum = 0;
	bool primeFlag = true;
	//printf("end is %d start is %d\n", primeArgs->end, primeArgs->start);
	int primeArr[primeArgs->end-primeArgs->start];
	for(int z = 0; z < (primeArgs->end-primeArgs->start); z++){
		//printf("z is %d\n", z);
		primeArr[z] = 0;	
	}
	/* to read from line start to finish, read the whole file till you read line start */
	fptr = fopen(primeArgs->fileName, "r");	
	if(fptr != NULL){
		while((fscanf(fptr, "%d", &y)) != EOF){
			if (lineNum >= primeArgs->start && lineNum <= primeArgs->end){
				primeFlag = true;
				printf("Vroom noises! %d\n", lineNum);
				for(int i = 2; i < y; i++){
					if(y%i==0){ /* the number is divisible by something other than 1 or itself, thus isn't a prime number */
						primeFlag = false;
					}
				}
				if(primeFlag == true){ /* the number is a prime */
					/* write the prime to file here */
					primeArgs->numOfPrimes++;
					primeArr[lineNum] = y;
					
				}
			}
			lineNum++;
		}
		fclose(fptr);
		pthread_mutex_lock(&lock);
		fptro = fopen(primeArgs->fileOutName, "a+");
		for(int z = 0; z< (primeArgs->end-primeArgs->start); z++){
			if(primeArr[z] != 0){
				fprintf(fptro, "%d\n", primeArr[z]);
			}
		}
		fclose(fptro);
		pthread_mutex_unlock(&lock);
	}
	pthread_exit(0);	
}


int main( int argc, char *argv [] )
{
	int numOfThreads = atoi(argv[1]);
	//printf("numOfThreads %d argv[1] %s\n", numOfThreads, argv[1]);
	
	char fileName[3][15] ={{"PrimeData1.txt"}, {"PrimeData2.txt"},{"PrimeData3.txt"}} ;
	char fileOutName[3][15] ={{"PrimeOut1.txt"}, {"PrimeOut2.txt"},{"PrimeOut3.txt"}} ;
	int x, fileLength = 0; 
	long long primes = 0;
	
	FILE *fptr;
	pthread_mutex_init(&lock, NULL);
	
	
	for(int i = 0; i<3; i++){
	/* for each file, get the length of the file, then close it to 'reset' to the top of the file */
		printf("opening %s\n", fileName[i]);
		fptr = fopen(fileName[i], "r");
		if (fptr != NULL){
			while((fscanf(fptr, "%d",&x)) != EOF){
				fileLength+=x;	
			}
			fclose(fptr);
		}
		double divvyUp = fileLength%numOfThreads;
		//printf("divvyUp is %f\n", divvyUp);
		fptr = fopen(fileOutName[i], "w"); /* creates an empty file, or empties the file if it already exists */
		if (fptr != NULL){fclose(fptr);}
		/* argv[1] aka how many threads. 
		fileLength/argv[1] tells us how many lines each thread reads */
		struct threadArguments *tas = malloc(sizeof(struct threadArguments) * numOfThreads);
		pthread_t *thrID = malloc(numOfThreads*sizeof(pthread_t));
		//printf("let's a go\n");
		int chunk = fileLength/numOfThreads;
		if(divvyUp == 0){
			/* they divide perfectly */
			//printf("a\n");
			for(int j = 0; j < numOfThreads; j++){
				//printf("attempt %d goes\n", j);
				tas[j].start = j*(chunk);
                		tas[j].end = j*(chunk) + (chunk);
				strcpy(tas[j].fileName, fileName[i]);
				strcpy(tas[j].fileOutName, fileOutName[i]);
				tas[j].numOfPrimes = 0;
		                pthread_create(&thrID[j], NULL, &primeThread, (void*)&tas[j]);	
			}
		}else{ /* We need to add the remainder to the final thread */
			//printf("b\n");
			for(int j = 0; j < (numOfThreads-1); j++){
				//printf("attempt %d goes\n", j);
				tas[j].start = j*(chunk);
                		tas[j].end = j*(chunk) + (chunk);
				strcpy(tas[j].fileName, fileName[i]);
				strcpy(tas[j].fileOutName, fileOutName[i]);
				tas[j].numOfPrimes = 0;
		                pthread_create(&thrID[j], NULL, &primeThread, (void*)&tas[j]);	
			}
			//printf("attempt %d goes\n", numOfThreads-1);
			tas[numOfThreads-1].start = numOfThreads*(chunk);
               		tas[numOfThreads-1].end = numOfThreads*(chunk) + chunk + (fileLength%numOfThreads);
			tas[numOfThreads-1].numOfPrimes = 0;
	                pthread_create(&thrID[numOfThreads-1], NULL, &primeThread, (void*)&tas[numOfThreads-1]);	
		}
		/* now join the threads, then do maths with the results */
		printf("Working, this bit hangs for some reason, please be patient");
		for(int j = 0; j < numOfThreads; j++){
			//printf("joining number %d\n", j);
	        	pthread_join(thrID[j], NULL);
      		}
		/* add up numOfPrimes and combine the primesArr */
		primes = 0;
		for(int j = 0; j < numOfThreads; j++){
			primes+= tas[j].numOfPrimes;
		}
		pthread_mutex_lock(&lock);
		fptr = fopen(fileOutName[i], "a+");
		if(fptr != NULL){
			fprintf(fptr, "totalling: %lld\n", primes);
			fclose(fptr);
		}
		pthread_mutex_unlock(&lock);
		free(thrID);
		free(tas);		
	}
	pthread_mutex_destroy(&lock);
	return 0;
}
