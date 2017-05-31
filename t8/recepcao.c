#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define CONTROLE 0

int main(int argc, char **argv)
{
   int idProcesso, nProcessos;
   int msg_tag;
   MPI_Status status;
   int idVolta, i;
   int msg;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);
   MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);

   if (idProcesso== CONTROLE) {
       for (i=1; i<nProcessos; i++) {     //mestre distribui primeiras tarefas
            msg = i;
            MPI_Send(&msg, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        //fica recebendo dos trabalhadores a mensagem identificada
        for (i=1; i<nProcessos; i++) {          //recebe de volta
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            idVolta = status.MPI_SOURCE;
            printf("Sou mestre e recebi %d do processo %d\n", msg, idVolta);
        }
   }
   //trabalhadores recebem e mandam de volta com identificacao
   else {
       MPI_Recv(&msg, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
       printf("Sou processo %d e recebi mensagem %d\n", idProcesso, msg);
       MPI_Send(&msg, 1, MPI_INT, 0, idProcesso, MPI_COMM_WORLD);       //manda de volta pro mestre, com sua tag
   }

   MPI_Finalize();
   return 0;
}
