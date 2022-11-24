#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define N 50000

int nn;
double *D[N],*apD,*X, *Y, *Z;

int main(int np, char*p[])
{
    int i,j,rr;
    long long sD;

    assert(np==2);

    nn = atoi(p[1]);
    assert(nn<=N);
    srand(1);

    printf("Dimensio dades =~ %g Mbytes\n",((double)(nn*(nn+2))*sizeof(double))/(1024*1024)); 

    //creacio matrius i vectors
    apD = calloc(nn*nn,sizeof(double)); assert (apD);
    D[0] = apD;
    for (i=0;i<nn;i++) {
		D[i+1] = D[i] + nn;
    }
    X = calloc(nn,sizeof(double)); assert (X);
    Y = calloc(nn,sizeof(double)); assert (Y);
    Z = calloc(nn,sizeof(double)); assert (Z);

    // Inicialitzacio
	rr = rand();
    for (i=0;i<nn;i++) {
        X[i]=(rr*i)%100 - 49.0;
        Y[i]=(rr*2*i)%100 - 49.0;
        Z[i]=(rr*3*i)%100 - 49.0;
        //printf("%lg, %lg, %lg \n",X[i],Y[i],Z[i]);
    }
    clock_t paralel_time = clock();
    // calcul de distancies
    for (i=0;i<nn;i++) {
        for (j=0;j<nn;j++) {
			D[i][j] = sqrt(pow((X[i] - X[j]),2) 
						 + pow((Y[i] - Y[j]),2)
						 + pow((Z[i] - Z[j]),2));
		}
    }
    double final_paralel_time = (double) (clock() - paralel_time) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución paralela: %f segundos\n", final_paralel_time);
    // comprovacio
    sD = 0;
    for (i=0;i<nn;i++) {
        for (j=i+1;j<nn;j++) {
            //printf("%lg ",D[i][j]);
	    	sD += (long long) (D[i][j]);
	    	if (D[i][j] != D[j][i]) {
				printf("diff in %d,%d: %g != %g",i,j,D[i][j],D[j][i]);
				abort();
			}
    	}
		//printf("\n");
    }

    printf("Suma elements de D: %lld \n",sD);
    exit(0);
}