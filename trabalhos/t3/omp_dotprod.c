#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <omp.h>

typedef struct
 {
   double *a;
   double *b;
   double c;
   int wsize;
   int repeat;
 } dotdata_t;

// Variaveis globais, acessiveis por todas threads
dotdata_t dotdata;

/*
 * Funcao executada por uma thread
 */
//omp_set_num_threads(1);

void *dotprod_worker() {
   #pragma omp parallel num_threads(2)
   {
   int i, k;
   //long offset = (long) arg;
   double *a = dotdata.a;
   double *b = dotdata.b;
   int wsize = dotdata.wsize;
   //int start = offset*wsize;
   //int end = start + wsize;
   double mysum;

   #pragma omp for
   for (k = 0; k < dotdata.repeat; k++) {
      mysum = 0.0;
      for (i = 0; i < wsize; i++)  {
         mysum += (a[i] * b[i]);
      }
   }

   #pragma omp critical
   dotdata.c += mysum;
   }
}


/*
 * Distribui o trabalho entre nthreads
 */

/*void dotprod_threads(int nthreads)
{
   int i;

   threads = (pthread_t *) malloc(nthreads * sizeof(pthread_t));
   pthread_mutex_init(&mutexsum, NULL);

   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for (i = 0; i < nthreads; i++) {
      pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
   }
   pthread_attr_destroy(&attr);
   for (i = 0; i < nthreads; i++) {
      pthread_join(threads[i], NULL);
   }
   free(threads);
}*/


/*
 * Tempo (wallclock) em microssegundos
 */
long wtime()
{
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

/*
 * Preenche vetor
 */
void fill(double *a, int size, double value)
{
   int i;
   for (i = 0; i < size; i++) {
      a[i] = value;
   }
}

/*
 * Funcao principal
 */
int main(int argc, char **argv)
{
   int nthreads, wsize, repeat;
   long start_time, end_time;

   if ((argc != 3)) {
      printf("Uso: %s <worksize> <repetitions>\n", argv[0]);
      exit(EXIT_FAILURE);
   }

   //nthreads = atoi(argv[1]);
   nthreads = 2;
   wsize = atoi(argv[1]);  // worksize = tamanho do vetor de cada thread
   repeat = atoi(argv[2]); // numero de repeticoes dos calculos (para aumentar carga)

   // Cria vetores
   dotdata.a = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.a, wsize*nthreads, 0.01);
   dotdata.b = (double *) malloc(wsize*nthreads*sizeof(double));
   fill(dotdata.b, wsize*nthreads, 1.0);
   dotdata.c = 0.0;
   dotdata.wsize = wsize;
   dotdata.repeat = repeat;

   // Calcula c = a . b em nthreads, medindo o tempo
   //start_time = wtime();
   start_time = omp_get_wtime();
   dotprod_worker();
   end_time = omp_get_wtime();
   //end_time = wtime();

   // Mostra resultado e estatisticas da execucao
   printf("%f\n", dotdata.c);
   printf("%ld usec\n", nthreads, (long) (end_time - start_time));
   //fflush(stdout);

   free(dotdata.a);
   free(dotdata.b);

   return EXIT_SUCCESS;
}
