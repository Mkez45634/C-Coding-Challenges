#include <stdio.h>

int main(void){
	printf("Hello World\n");
	char fileName[4][15] ={{"datasetLR1.txt"}, {"datasetLR2.txt"},{"datasetLR3.txt"},{"datasetLR4.txt"}} ;
	int x, y, z = 0;
	float xsqd, xy, n, a, b ,sigmax, sigmay = 0;
	FILE *fptr;

	printf("Enter an integer value of x\n");
	scanf("%d", &z);
		
	for (int i = 0; i < 4; i++){
		//printf("fileName: %s\n", fileName[i]);
		fptr = fopen(fileName[i], "r");
		if (fptr != NULL){
			while((a = fscanf(fptr, "%d,%d",&x, &y)) != EOF){
				sigmax+=x;
				sigmay+=y;
				xsqd+=(x*x);
				xy+=(x*y);
				n++;
				//printf("n: %f\n", n);
			}
		}
		a = (sigmay*xsqd - sigmax*xy)/(n*xsqd - (sigmax*sigmax)); 
		b = (n*xy - sigmax*sigmay)/(n*xsqd - (sigmax*sigmax));
		fclose(fptr);
		printf("%s -> y = %fx + %f", fileName[i], a, b);
		printf("	Therefore y = %f\n", (a*z + b));
		n = 0;
		xy = 0;
		xsqd = 0;
		sigmax = 0;
		sigmay = 0;
	}
}
