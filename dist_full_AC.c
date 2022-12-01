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
	clock_t parameter_resolution_time = clock();
	// Resolució dels paràmetres
    int i,j,rr;
    long long sD;

    assert(np==2);

    nn = atoi(p[1]);
    assert(nn<=N);
    srand(1);

    //printf("Dimensio dades =~ %g Mbytes\n",((double)(nn*(nn+2))*sizeof(double))/(1024*1024)); 
	double final_parameter_resolution_time = (double) (clock() - parameter_resolution_time) / CLOCKS_PER_SEC;
	printf("ParameterResolution Time(s): %f\n", final_parameter_resolution_time);

    // Creacio matrius i vectors
	clock_t array_creation_time = clock();
    apD = calloc(nn*nn,sizeof(double)); assert (apD);
    D[0] = apD;
    for (i=0;i<nn;i++) {
		D[i+1] = D[i] + nn;
    }
    X = calloc(nn,sizeof(double)); assert (X);
    Y = calloc(nn,sizeof(double)); assert (Y);
    Z = calloc(nn,sizeof(double)); assert (Z);
	double final_array_creation_time = (double) (clock() - array_creation_time) / CLOCKS_PER_SEC;
	printf("ArrayCreation Time(s): %f\n", final_array_creation_time);

    // Inicialitzacio
	clock_t initialization_time = clock();
	rr = rand();
    for (i=0;i<nn;i++) {
        X[i]=(rr*i)%100 - 49.0;
        Y[i]=(rr*2*i)%100 - 49.0;
        Z[i]=(rr*3*i)%100 - 49.0;
        //printf("%lg, %lg, %lg \n",X[i],Y[i],Z[i]);
    }
	double final_initialization_time = (double) (clock() - initialization_time) / CLOCKS_PER_SEC;
	printf("Initialization Time(s): %f\n", final_initialization_time);

    clock_t paralel_time = clock();
    // Calcul de distancies
    for (i=0;i<nn;i++) {
        for (j=0;j<nn;j++) {
			D[i][j] = sqrt(pow((X[i] - X[j]),2) 
						 + pow((Y[i] - Y[j]),2)
						 + pow((Z[i] - Z[j]),2));
		}
    }
    double final_paralel_time = (double) (clock() - paralel_time) / CLOCKS_PER_SEC;
    printf("Parallelizable Time(s): %f\n", final_paralel_time);
    
	clock_t verification_time = clock();
	// Comprovacio
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
	//printf("Suma elements de D: %lld \n",sD);
    double final_verification_time = (double) (clock() - verification_time) / CLOCKS_PER_SEC;
	printf("Verification Time(s): %f\n", final_verification_time);

    exit(0);
}
