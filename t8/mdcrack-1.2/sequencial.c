#include <stdio.h>
#include <stdlib.h>

//FUNCAO DE LEITURA DO ARQUIVO E ARMAZENAMENTO DAS SENHAS NO VETOR 'SENHAS' (POR REFERENCIA)
void lerSenhas(char *senhas[30][33]) {
    int i=0;
    char linhaLote[33];
    FILE *lote;
    lote = fopen("crackme.txt", "r");
    if (lote == NULL)
         printf("Erro ao abrir o arquivo.\n");
    else
         while ((fgets(linhaLote, sizeof(linhaLote), lote)) != NULL) {    //le linha por linha
             strcpy(senhas[i], linhaLote);
             i++;
             if ((fgets(linhaLote, sizeof(linhaLote), lote)) == '\n')
                 i--;
         }
}

int main(int argc, char **argv) {
   int i;

   char *senhas[30][33];    //vetor de strings (cada elemento eh uma senha criptografada)
   lerSenhas(&senhas);

   for (i=0; i<29; i++) {       //29 senhas
       char *s = senhas[i];
       char arg[80] = "bin/mdcrack -M MD5 -s abcdefghijklmnopqrstuvwxyz ";     //comando para chamar mdcrack c/ seus argumentos
       strcat (arg, s);     //ultimo argumento é a senha criptografada, muda a cada iteracao do laço
       system(arg);         //chama mdcrack
   }
   return 0;
}
