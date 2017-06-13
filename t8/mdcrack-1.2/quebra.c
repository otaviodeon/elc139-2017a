#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

#define CONTROLE 0

//FUNCAO DE LEITURA DO ARQUIVO E ARMAZENAMENTO DAS SENHAS NO VETOR 'SENHAS' (E GUARDA NUMERO DE SENHAS EM numSenhas)
char **lerSenhas(int *numSenhas) {
    int nSenhas=0;
    char linhaLote[33];
    char **senhas;    //vetor de strings (cada elemento eh uma senha criptografada)
    FILE *lote;
    int i=0;
    lote = fopen("crackme.txt", "r");
    if (lote == NULL)
         printf("Erro ao abrir o arquivo.\n");
    else {
         while ((fgets(linhaLote, sizeof(linhaLote), lote)) != NULL)      //conta senhas
             nSenhas++;
         rewind (lote);
         senhas = malloc(nSenhas * sizeof(char*));
         while ((fgets(linhaLote, sizeof(linhaLote), lote)) != NULL) {      //escreve cada senha no vetor senhas
             if (! (strstr(linhaLote, "\n") != NULL)) {
                 senhas[i] = malloc(33 * sizeof(char));
                 strcpy(senhas[i], linhaLote);
                 i++;
             }
         }
     }
     *numSenhas = i;          //muda variavel nSenhas(na main) por referencia
     return senhas;
}

int minimo (int x, int y) {
    if (x < y) return x; else return y;
}

int main(int argc, char **argv) {
    int idProcesso, nProcessos;
    int msg_tag;
    MPI_Status status;
    int idVolta, i;
    char **senhas;    //vetor de strings (cada elemento eh uma senha criptografada)
    char msg[32];
    char msgAcabei[30];       //trabalhador manda quando terminou de quebrar sua senha atual

    double t1, t2;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &idProcesso);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcessos);

    if (idProcesso == CONTROLE) {
       int nSenhas;
       senhas = lerSenhas(&nSenhas);
       int trabalhadorSenha[nProcessos-1];          //guarda vinculo de trabalhador com o indice da senha que esta quebrando no momento
       int senhasEnviadas = nProcessos-1;     //ou nsenhas          //quantia de senhas atualmente sendo quebradas
       int senhaAtual=0;
       int processosAcabados = 0;

       t1 = MPI_Wtime();

       if (nProcessos > nSenhas) {                  //se houver mais processos do que senhas, mata processos que estao sobrando
           for (int i=nProcessos-1; i>nSenhas; i--) {
               msg[0] = '9';
               processosAcabados++;
               MPI_Send(&msg, 33, MPI_BYTE, i, 0, MPI_COMM_WORLD);
               if (processosAcabados >= nProcessos-1)       //se todos trabalhadores morreram, mestre para também
                   break;
           }
       }

       for (i=1; i<nProcessos; i++) {     //mestre distribui primeiras tarefas
            for (int j=0; j<33; j++)
                msg[j] = senhas[senhaAtual][j];        //mensagem temporaria - definida letra por letra
            trabalhadorSenha[i-1] = i-1;
            MPI_Send(&msg, 33, MPI_BYTE, i, 0, MPI_COMM_WORLD);
            trabalhadorSenha[i-1] = i-1;
            senhasEnviadas++;
            senhaAtual++;
       }

       int senhasQuebradas = 0;
       char loteSenhasQuebradas[nSenhas][30];

       while (1) {            //enquanto houverem senhas a serem quebradas
           MPI_Recv(&msgAcabei, 30, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
           int *tamSenhaQuebrada;
           MPI_Get_count(&status, MPI_BYTE, &tamSenhaQuebrada);

           for (int i=0; i<tamSenhaQuebrada; i++)      //salva senha quebrada na posiçao certa para imprimir no final da execuçao
               loteSenhasQuebradas[trabalhadorSenha[status.MPI_SOURCE-1]][i] = msgAcabei[i];
           senhasQuebradas++;
           senhasEnviadas--;

           if (senhasEnviadas+senhasQuebradas-nProcessos+1 < nSenhas) {           //se ainda houverem senhas que nao foram quebradas nem mandadas
               for (int j=0; j<33; j++)
                   msg[j] = senhas[senhaAtual][j];        //mensagem temporaria - definida letra por letra
               MPI_Send(&msg, 33, MPI_BYTE, status.MPI_SOURCE, 0, MPI_COMM_WORLD);      //manda proxima senha pro trabalhador vago
               trabalhadorSenha[status.MPI_SOURCE-1] = senhaAtual;
               senhasEnviadas++;
               senhaAtual++;
           } else {         //senao, o trabalhador que acabou de quebrar uma senha nao tem mais o que fazer; morre
               msg[0] = '9';
               processosAcabados++;
               MPI_Send(&msg, 1, MPI_BYTE, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
               if (processosAcabados >= nProcessos-1)       //se todos trabalhadores morreram, mestre para também
                   break;
           }
       }

       for (int i=0; i<nSenhas; i++){
           printf("%s", loteSenhasQuebradas[i]);
       }

       t2 = MPI_Wtime();
       printf("Tempo = %f segundos\n", t2 - t1 );
   }

   else {
       while (1) {      //trabalhador fica esperando/quebrando senhas até receber mensagem do mestre para parar de trabalhar
           MPI_Recv(&msg, 33, MPI_BYTE, CONTROLE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
           if (msg[0] == '9')
               break;
           else {
               char arg[80] = "bin/mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ";     //comando para chamar mdcrack c/ seus argumentos
               strcat (arg, msg);     //ultimo argumento é a senha criptografada
               FILE *ls = popen(arg, "r");      //chama mdcrack
               char buf[50];
               int indiceFimLinha;
               int tamanhoSenha;            //numero de caracteres da senha já quebrada
               while (fgets(buf, sizeof(buf), ls) != 0) {
                   int tamBuf = (sizeof(buf)) / (sizeof(char));
                   if (strstr(buf, "found") != NULL) {        //quando for a linha de saida que contem a senha quebrada
                       for (int i=0; i<sizeof(buf); i++) {
                           if (buf[i]=='\0') {               //acabou a linha
                               indiceFimLinha = i;
                               tamanhoSenha = i-20;
                               break;
                           }
                       }
                       for (int i=21; i < indiceFimLinha; i++)
                           msgAcabei[i-21] = buf[i];
                   }
               }
               pclose(ls);
               char msgVolta[tamanhoSenha-1];                 //senha quebrada
               for (int i=0; i<tamanhoSenha-1; i++)
                   msgVolta[i] = msgAcabei[i];
               MPI_Send(&msgVolta, tamanhoSenha, MPI_BYTE, 0, idProcesso, MPI_COMM_WORLD);      //manda senha descriptografada
           }
       }
   }

   MPI_Finalize();
   return 0;
}
