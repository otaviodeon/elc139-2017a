[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

# T2: Experiências com Profilers

Disciplina: Programação Paralela (ELC139)
Aluno: Otávio Oliveira Deon

## PARTE 1

Pergunta: **O perfil é afetado pelas opções de configuração?**
Sim, a mudança dos parâmetros implica em execuções diferentes, e consequentemente são gerados perfis diferentes. As diferenças sempre estão relacionadas com o tempo de execução do programa. A visualização da árvore de execução gerada também muda, pois ela é ordenada pelo tempo gasto em cada função. Por exemplo, com a entrada [300000, 10], a função init-vectors teve tempo desprezível, ficando abaixo da função wtime na árvore. Já com a entrada [3000000, 10], a função init-vectors tomou mais tempo (24% do tempo do programa).
Pergunta: **Pelo perfil de execução, há alguma função que poderia ser candidata a paralelização? Por quê?**
Sim, a escolha óbvia seria a função dot_product. Nas diferentes execuções do programa, essa função sempre é a que tem maior porcentagem de tempo de execução, mesmo sendo chamada apenas uma vez. Caso fosse alterada de forma a ser processada paralelamente, o profiling do programa poderia ter mudanças consideráveis devido a um menor tempo de execução.

## PARTE 2	

**Observações:**
Outro profiler escolhido foi o Callgrind/KCacheGrind. Callgrind é uma ferramenta semelhante ao Valgrind. Para fazer o profiling, o Callgrind faz com que o programa-alvo seja transformado numa linguagem intermediária e rodado num processador virtual emulado pelo valgrind (ver referência). Como consequência, há bastante overhead (o programa pode levar de 10 a 50 vezes mais tempo para ser executado do que sem o profiler). O KCacheGrind é usado para visualizar as informações do profiling.
A instalação foi simples e rápida (as aplicações fazem parte de pacotes da distribuição Linux usada para o trabalho). 
A execução do profiler permite diferentes parâmetros. Eles podem ser relativos ao formato da saída das informações, execução de ações no meio do profiling (por exemplo, agir ao entrar ou sair de certa função do programa), etc.

Programa escolhido: problema do subarray máximo. Linguagem: C.
Na execução do max_subarray.c com entrada 10000000 com o callgrind, observou-se um grande aumento no tempo de execução, como previsto (relacionado à execução sem o profiler). Porém, comparado ao gprof, o resultados são mais ricos (ver screenshots). A visão mais detalhada do callgrind permite ver que funções são chamadas e por quem, além da contagem de chamadas. Ao selecionar uma função, podemos ver em gráficos quem a chamou e que outras funções ela eventualmente invocou, sempre mostrando porcentagem de tempo gasto, assim facilitando uma possível escolha de qual parte do programa paralelizar. Assim como é possível ver um grafo de funções de todo o programa, também podemos ver o grafo particular de uma função, com o escopo de visão limitado (graph2.png). Em callers-callees.png, selecionamos a função maxCrossingSum da pilha de funções e vemos quem a chamou e quantas vezes (o profiler vai "rastreando" até o início do programa, mostrando a distância em relação às funções que a chamaram). Também vemos quem a função maxCrossingSum chamou. 
Com o callgrind, ao executar o programa com a entrada 5000000 (metade), as funções que gastam mais tempo executando são as mesmas (maxSubArraySum), sendo a candidata à paralelização. 
Mas com a entrada 500000, no gprof, foi diferente. Segundo o gprof, a maxCrossingSum usou 88% do tempo do programa, enquanto a maxSubArraySum usou apenas 3%. Uma hipótese para explicar essa grande diferença é que é usada recursão na maxSubArraySum - os contadores nos profilers podem ter agido de maneiras diferentes frente à isso.

## REFERÊNCIAS

https://baptiste-wicht.com/posts/2011/09/profile-c-application-with-callgrind-kcachegrind.html
http://valgrind.org/docs/manual/cl-manual.html
https://web.stanford.edu/class/cs107/guide_callgrind.html

