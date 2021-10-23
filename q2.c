#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>

struct threadArguments{
    int start;
    int end;
    double result;
};

void *piThread(void *p){
    struct threadArguments *piArgs = p;
    int n;
    double piCalc = 0;
    for(n = piArgs->start; n <=piArgs->end; n++){
        piCalc+= (pow(-1, n))/(2*n+1);
    }
    piArgs->result = piCalc;
}

int main( int argc, char *argv [] )
{
  if (argc == 3  ){
      /* argv[1] is number of iterations argv[2] is number of threads */
      int a = atoi(argv[1]);
      int b = atoi(argv[2]);
      int i;
      struct threadArguments *tas = malloc(b*sizeof(struct threadArguments));
      pthread_t *thrID = malloc(b*sizeof(pthread_t));
      double calcValue = 0;
      if (a%b == 0){ /* the number of threads divides nicely */
            for(i = 0; i < b; i++){
                tas[i].start = i*(a/b);
                tas[i].end = i*(a/b) + (a/b);
                pthread_create(&thrID[i], NULL, &piThread, (void*)&tas[i]);

            }
      }else{ /* we need to do some extra maths for the structs. Notably the remainder */

            for(i = 0; i < b-1; i++){
                tas[i].start = i*(a/b);
                tas[i].end = i*(a/b) + (a/b);
                pthread_create(&thrID[i], NULL, &piThread, (void*)&tas[i]);

            }
            /* include the remainder */
            tas[b-1].start = i*(a/b);
            tas[b-1].end = i*(a/b) + (a/b) + a%b;
            pthread_create(&thrID[b-1], NULL, &piThread, (void*)&tas[b-1]);
            

      }
      for(i = 0; i < b; i++){
     	 pthread_join(thrID[i], NULL);
      }
      for(i = 0; i < b; i++){
         calcValue+= 4 * tas[i].result;
      }
      printf("Tada pi is %f\n" ,calcValue);
      free(thrID);
      free(tas);
  }else{
      printf("Wrong number of command line arguments given, please give two.");
  }
}
