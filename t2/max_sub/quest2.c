#include<stdlib.h>
#include<stdio.h>
#include<limits.h>
 


/*MATRIX-CHAIN-ORDER(p) segundo o Cormen
1 n <- length[p] - 1
// let m [1..n,1..n] and s[1..n-1,2..n] be new tables
2 for i <- 1 to n
3 	do m[i, i ] <- 0
4 for l <- 2 to n ( l is the chain length)
5 	for i <- 1 to n - l + 1
6 		j <- i + l - 1
7 		m[i, j ] <- infinite
8 		for k <- i to j - 1
9 			q <- m[i, k] + m[k + 1, j ] + pi-1 pk pj
10 			if q < m[i, j ]
11 				m[i, j ] <- q
12 				s[i, j ] <- k
13 return m and s

*/
#define tam 10
/* tamanho max que cria o vetor que precisa pra gerar o caminho  otimo ,poderia ter feito com ponteiro de ponteiro mas preferi assim */

/* algoritimo do cormen para printar o caminho otimo */
void print_optimal(int S[][tam],int i,int j)
{
if (i == j)
    printf(" A%d ",i);
else{
	printf(" ( ");
	print_optimal(S,i, S[i][j]);
    print_optimal(S,S[i][j] + 1, j);
    printf(" ) ");
	}
} 

int Matrixmultiplic(int vet[], int n)
{
 
 	int i, j, l, k, q=0;
	int m[n][n];
	int S[tam][tam];
	/* se passado uma matriz só retorna 0;*/
    for (i=1; i<=n; i++)
        m[i][i] = 0;
 	/*laco pra percorrer todas as matrizes*/
    for (l=2; l<=n; l++)
    {
        for (i=1; i<=n-l+1; i++)
        {
            j = i+l-1;
            m[i][j] = INT_MAX;
            for (k=i; k<=j-1; k++)
            {           
                q = m[i][k] + m[k+1][j] + vet[i-1]*vet[k]*vet[j];
                if (q < m[i][j]){
                    m[i][j] = q;
            		S[i][j]= k;
            	}
			}
        }
    }
	
	i=1;j=n;
	print_optimal(S, i,j);
    return m[1][n];
}
  
/*  matrizes  -> 4x3 , 3x2, 2x1
	possibilidade 1 =  (4x3x2) + (4x2x1) = 32 multiplicacoes  com (AB)C
	possibilidade 2 =  (3x2x1) + (4x3x1) = 18 multiplicacoes com A(BC)  == resposta certa


*/
int main()
{
    int arr[] = {9,8,7,6,5,4,3,2,1}; 
    int size = sizeof(arr)/sizeof(arr[0]);
	size = size -1;
 	

    printf("numero minimo de multiplicoes obtidos foi \n %d ",Matrixmultiplic(arr, size));
 
    
    return 0;
}