[Programação Paralela](https://github.com/otaviodeon/elc139-2017a) > Trabalhos

<br>
Aluno: Otávio Deon

<br><br>
# T9: Paralelização do Algoritmo de Busca A-Estrela com MPI(Python)
Disciplina: Programação Paralela <br>

<br><br>
# Código
Disponível em *grid.py*. Executar com o comando mpiexec -n 2 python grid.py; digitar um número largura do grid; digitar o percentual de obstáculos (0 a 1); posicão x y inicial; posição x y objetivo. Exemplo: <br> 
100<br>
0.3<br>
2 2 <br>
30 40 <br>

<br><br>
# Estratégia
O programa foi paralelizado para execução com 2 processos. A estratégia de paralelização utilizada teve a ideia de cada
processo começar a executar em um ponto do grid (um na célula inicial, e o outro na célula objetivo). Os dois executam
buscando o menor caminho entre si e o ponto oposto. A execução para quando as posições atuais dos 2 processos se encontram.

Os resultados mostrados são: 
posição atual de cada processo no grid, processo em que termina a execução, a rota resultado, e o nº de passos até a solução. 

<br><br>
# Referências
http://pythonhosted.org/mpi4py/ <br>
https://mpi4py.readthedocs.io/en/stable/ <br>
https://mpi4py.scipy.org/docs/usrman/tutorial.html
