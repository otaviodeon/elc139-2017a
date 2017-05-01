//Funcionalidade:
//1º processo começa com a mensagem valor 10 e manda pro proximo
//cada processo intermediário recebe uma mensagem, incrementa em 10 e passa adiante
//ultimo processo (np-1) recebe a ultima mensagem


#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int ntasks, rank;
    int msg = 10;
    int msg_rec;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ntasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {        //primeiro processo só manda
        MPI_Send(&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
        printf("Processo %d mandei %d\n", rank, msg);
    }
    //processos 1 até np-1: recebem; incrementam; mandam
    else if (rank < ntasks-1) {
            MPI_Recv(&msg_rec, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Processo %d recebi %d\n", rank, msg_rec);
            msg = msg_rec+10;
            MPI_Send(&msg, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD);
            printf("Processo %d mandei %d\n", rank, msg);
    }

    if (rank == ntasks-1) {     //ultimo processo só recebe
        MPI_Recv(&msg_rec, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Processo %d recebi %d\n", rank, msg_rec);
    }

    MPI_Finalize();

}
