#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long wtime() {
   struct timeval t;
   gettimeofday(&t, NULL);
   return t.tv_sec*1000000 + t.tv_usec;
}

//FUNCAO DE LEITURA DO ARQUIVO E ARMAZENAMENTO DAS SENHAS NO VETOR 'SENHAS' (POR REFERENCIA)
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

int main(int argc, char **argv) {
   int i;
   int nSenhas;
   long start_time, end_time;
   char **senhas;    //vetor de strings (cada elemento eh uma senha criptografada)
   senhas = lerSenhas(&nSenhas);

   start_time = wtime();

   for (i=0; i<nSenhas; i++) {       //29 senhas
       char *s = senhas[i];
       char arg[80] = "bin/mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ";     //comando para chamar mdcrack c/ seus argumentos
       strcat (arg, s);     //ultimo argumento é a senha criptografada, muda a cada iteracao do laço
       FILE *ls = popen(arg, "r");
       char buf[60];
       char senhaQuebrada[30];
       int indiceFimLinha;
       while (fgets(buf, sizeof(buf), ls) != 0) {
           int tamBuf = (sizeof(buf)) / (sizeof(char));
           if (strstr(buf, "found") != NULL) {        //quando for a linha de saida que contem a senha quebrada
               for (int i=0; i<sizeof(buf); i++) {
                   if (buf[i]=='\0') {               //acabou a linha
                       indiceFimLinha = i;
                       break;
                   }
               }
               for (int i=21; i < indiceFimLinha; i++)
                   senhaQuebrada[i-21] = buf[i];
           }
       }
       printf("%s", senhaQuebrada);
       memset(senhaQuebrada, 0, sizeof(senhaQuebrada));
       pclose(ls);
   }

   end_time = wtime();
   printf("Tempo = %ld usec\n", (long) (end_time - start_time));

   return 0;
}
