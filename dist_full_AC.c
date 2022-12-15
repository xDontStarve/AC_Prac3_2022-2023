#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include <stdint.h>

#define N 50000

int nn, nt, lines, modulus;
double *D[N],*apD,*X, *Y, *Z;

void* partial_calculation(void* args);

int main(int np, char*p[])
{
    int i,j,rr;
    long long sD;
    pthread_t *threads;

    assert(np==3);

    nn = atoi(p[1]);
    assert(nn<=N);
    srand(1);

    // getting the number of threads from programm arguments
    nt = atoi(p[2]);

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
    
    threads = malloc(sizeof(pthread_t) * nt);
    lines = nn / nt;
    modulus = nn % nt;
    printf("Lines:%d, Mod:%d\n", lines, modulus);
    // calculate the portion of code for each thread
    for (long i = 0; i < nt; i++){ 
        pthread_create(&threads[i], NULL, partial_calculation, (intptr_t *) i);
    }

    for (int i = 0; i < nt; i++) {
        pthread_join(threads[i], NULL);
    }

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
    pthread_exit(NULL);
    return 0;
}

void *partial_calculation(void *args) {
    long id = (intptr_t) args;
    int start = lines * id;
    int end = lines * (id+1);

    for (int i = start; i < end; i++) {
        for (int j=0;j<nn;j++) {
			D[i][j] = sqrt(pow((X[i] - X[j]),2) 
						 + pow((Y[i] - Y[j]),2)
						 + pow((Z[i] - Z[j]),2));
		}
    }
//Finish line of current thread is previus thread's last line + number of lines per thread (lines), +1 if thread is assigned an extra line (modulus) 
    if (id<modulus){
        int i = nt * lines + id;
        for (int j=0;j<nn;j++) {
			D[i][j] = sqrt(pow((X[i] - X[j]),2) 
						 + pow((Y[i] - Y[j]),2)
						 + pow((Z[i] - Z[j]),2));
		}
    }
    printf("[%03li]start=%d, end=%d\n", id, start, end);
    return NULL;
}