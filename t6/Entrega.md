[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

# T6: Experiências com MPI
Disciplina: Programação Paralela <br>
Aluno: Otávio Oliveira Deon <br>
Computador utilizado:  Os testes locais foram realizados utilizando um processador Intel Core i5-4200U, possuindo 2 núcleos, 4 threads, e frequência de 1.6GHz ou 2.6GHz.

<br><br>
## Parte 1
Particionamento:
```
part_size = n / ntasks;
```
Comunicação (não em ordem):
```
MPI_Send(&mydot, 1, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD);
MPI_Recv(&dot, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
```
Aglomeração:
```
if (taskid == MASTER)
   {
      for (i = 0; i < ntasks-1; i++)
      {
         MPI_Recv(&dot, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         printf("Valor parcial recebido do processo %d = %f\n", status.MPI_SOURCE, dot);
         mydot += dot;
      }
      [...]
   }
```
Mapeamento:
```
else
{
    MPI_Send(&mydot, 1, MPI_DOUBLE, MASTER, msg_tag, MPI_COMM_WORLD);
}
```

<br><br>
Tempos de execução (média): <br>
Caso A (lsc4): 15553888 usec (15,5s). <br>
Caso B (lsc4): 7907604 usec (7,9s). Speedup de 1.96695. <br>
Caso C: 
Caso D (lsc4): 4163840 usec (4,16s). Speedup de 3.73546. <br>
Caso E:

<br><br>
## Parte 2
Programa do MPI Pipeline disponível em *mpi_pipeline.c*. <br>
No programa *mpi_errado1.c*, o erro está na definição da tag ao enviar/receber as mensagens. Como a variável tag recebe o valor de rank (linhas 27 e 36), apesar do processo 0 mandar a mensagem pro processo 1, este não a recebe pois a tag na sua função MPIRecv tem valor 1, diferente do esperado. O processo 1 então fica esperando receber a mensagem do processo 0, sem sucesso, e não executa mais nada. Mesmo se mandasse alguma mensagem pro processo 0, este não a receberia, pois esperaria uma tag 0. A solução é tratar adequadamente as tags, dando o valor de 0 para todas, por exemplo, ou usar MPIANYTAG. <br>
Em *mpi_errado2.c*, o erro está em não chamar a função MPI_Finalize() no fim dos processos. A solução é chamar a função no fim do código. <br>
As soluções estão em *mpi_certo1.c* e *mpi_certo2.c*. 

<br><br>
## Referências	
