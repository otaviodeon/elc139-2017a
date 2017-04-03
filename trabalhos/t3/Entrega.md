[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

# T3: Programação Paralela Multithread 

Disciplina: Programação Paralela (ELC139)<br/>
Aluno: Otávio Oliveira Deon

## Pthreads

**Explique como se encontram implementadas as 4 etapas de projeto: particionamento, comunicação, aglomeração, mapeamento (use trechos de código para ilustrar a explicação).** <br/>
O particionamento é feito de acordo com o número de threads que o programa vai executar, criando "limites" nos vetores. A função *dotprod_worker* tem como parâmetro o número da thread que está usando-a. Assim, a primeira thread irá lidar com os vetores em certa região, calculada com as variáveis *start* e *end*. No código: 
```
long offset = (long) arg;
int start = offset*wsize;
int end = start + wsize;
``` 

A comunicação evita que resultados errados sejam obtidos devido à má sincronização das variáveis alteradas pelas threads. No caso, a variável *dotdata.c* é alterada com exclusão mútua para evitar que, por exemplo, uma thread altere a variável e logo depois outra thread use o valor antigo dela. No código: 

```
pthread_mutex_lock (&mutexsum);
dotdata.c += mysum;
pthread_mutex_unlock (&mutexsum);
``` 
Na aglomeração junta-se os valores das somas de cada thread na variável *c* de *dotdata*. O resultado final é a soma das somas parciais das threads. No código:
```
dotdata.c += mysum;
``` 

O mapeamento, mostrado no código abaixo, cria as threads passando o "número" de cada thread como argumento, e depois realiza as junções. 

```
for (i = 0; i < nthreads; i++) {
   pthread_create(&threads[i], &attr, dotprod_worker, (void *) i);
}
for (i = 0; i < nthreads; i++) {
   pthread_join(threads[i], NULL);
}
```
	
**Considerando o tempo (em segundos) mostrado na saída do programa, qual foi a aceleração com o uso de threads?** <br/>
A execução sequencial (1 thread) levou 7629358 usecs. A execução com 2 threads levou 5404262 usec. Logo, a aceleração com o uso de threads foi de aproximadamente **1.41173**.

**A aceleração (speedup) se sustenta para outros tamanhos de vetores, números de threads e repetições? Para responder a essa questão, você terá que realizar diversas execuções, variando o tamanho do problema (tamanho dos vetores e número de repetições) e o número de threads (1, 2, 4, 8..., dependendo do número de núcleos). Cada caso deve ser executado várias vezes, para depois calcular-se um tempo de processamento médio para cada caso. Atenção aos fatores que podem interferir na confiabilidade da medição: uso compartilhado do computador, tempos muito pequenos, etc.** <br/>
Cada execução foi realizada 4 vezes e então foi obtida a média do tempo: <br/>
Execução com 1 thread, vetor tamanho 1.000.000, 1.000 repetições: tempo médio 3651638 usec (3,65 segundos). <br/>
Execução com 2 threads, vetor tamanho 500.000, 1.000 repetições: tempo médio 2863870 usec (2,86 segundos). <br/>
Execução com 4 threads, vetor tamanho 250.000, 1.000 repetições: tempo médio 1734554 usec (1,73 segundos). <br/>
Execução com 8 threads, vetor tamanho 125.000, 1.000 repetições: tempo médio 1752581 usec (1,75 segundos). <br/>
Comparando a execução de 1 thread com 8 threads, o speedup foi de 2.08. 

**Elabore um gráfico/tabela de aceleração a partir dos dados obtidos no exercício anterior.** <br/>
![grafico](https://github.com/otaviodeon/elc139-2017a/tree/master/trabalhos/t3/grafico.png?raw=true) 


**Explique as diferenças entre [pthreads_dotprod.c](pthreads_dotprod/pthreads_dotprod.c) e [pthreads_dotprod2.c](pthreads_dotprod/pthreads_dotprod2.c). Com as linhas removidas, o programa está correto?** <br/>
A diferença entre os programas é que o segundo não utiliza exclusão mútua (mutex) sobre a variável *dotdata.c*. Ou seja, diversas threads podem acessar e modificar a variável ao mesmo tempo, o que *pode* gerar erros no resultado. O programa não está correto com as linhas removidas.

## OpenMP

**Implemente um programa equivalente a [pthreads_dotprod.c] usando OpenMP. Avalie o desempenho do programa em OpenMP, usando os mesmos dados/argumentos do programa com threads POSIX.**<br/>
Foi avaliado o programa *omp_dotprod* utilizando OpenMP. Não foi possível determinar com exatidão os tempos de execução, mas foram visivelmente mais curtos.


## Referências

https://computing.llnl.gov/tutorials/openMP/ <br/>
https://hpc.llnl.gov/training/tutorials <br/>
http://www.openmp.org/wp-content/uploads/omp-hands-on-SC08.pdf
