[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

# T4: Bug em OpenMP

Disciplina: Programação Paralela (ELC139)<br/>
Aluno: Otávio Oliveira Deon

**1. Resposta:** <br>
O bug do programa é não especificar a variável *total* como privada para cada thread. Como há paralelismo de threads justamente em cima desta
variável, alterando seu valor com base em seu valor anterior, é necessário que haja essa proteção. Da maneira como o programa está escrito,
mais de uma thread podem usar a variável concorrentemente, gerando um resultado errado no final. 

**Correção do código** <br>
Na linha 33, na declaração do for em paralelo, adiciona-se a cláusula private(total) para garantir exclusividade de uso da variável em cada
thread.

