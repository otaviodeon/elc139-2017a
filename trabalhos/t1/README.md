[Programação Paralela](https://github.com/AndreaInfUFSM/elc139-2017a) > Trabalhos

T1: TOP500 & me - Comparativo de Arquiteturas Paralelas
-------------------------------------------------------

1. Antes de começar, faça um fork no GitHub do repositório da disciplina, criando um novo repositório contendo os arquivos fornecidos pela professora. Você entregará seus trabalhos nesse repositório.
2. Neste trabalho, você vai preencher uma tabela comparativa entre um supercomputador do [TOP500](http://top500.org) e um outro computador. Esse segundo computador deverá estar acessível para fazer alguns trabalhos da disciplina e deverá ter uma arquitetura paralela (p.ex.: seu notebook ou desktop com processador multicore, servidor do NCC, etc.)
3. O modelo de tabela é fornecido [aqui](comparativo.md). Você pode modificá-lo, mas os dados solicitados na primeira coluna devem ser mantidos. 
4. Para preencher a tabela, você vai ter que fazer o seguinte:
  - Buscar informações de fabricantes, tanto para o computador do TOP500 como para seu computador.
  - Executar um benchmark **Linpack** no seu computador, para preencher o item sobre Desempenho (Flop/s) na tabela comparativa.
5. Existem várias distribuições do Linpack, por exemplo: HPL (High Performance Linpack), usado no TOP500; em netlib.org, com fontes disponíveis; no site da Intel; etc. O HPL é um Linpack com paralelismo e exige instalação de algumas bibliotecas. O da Intel é distribuído junto com outros benchmarks executáveis. Você pode escolher qualquer distribuição, mas deverá informar onde foi obtida.
