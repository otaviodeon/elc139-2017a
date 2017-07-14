import pygame
import cProfile
from pygame.locals import *
import random
from no import No
from math import sqrt, pow
from mpi4py import MPI


'''
 /** Classe principal do programa. Implementa o algoritmo a-estrela para achar o melhor caminho numa matriz.
 * Executar com o comando: python grid.py
 */
'''

'''
 /**Metodo para montar o grid (configuracao do cenario com valores inteiros representando cada celula da matriz)
 * @param  inicio Tuple - tupla com posicao (x, y) da celula inicial
 * @param  fim Tuple - tupla com posicao (x, y) da celula objetivo
 * @param  obstaculos List - lista de posicoes que contem obstaculo
 * @param  dim Int - numero de linhas/colunas do grid
 * @return Matrix - matriz de inteiros representando o cenario do programa, onde cada valor representa o significado da celula
 */
'''
def montaGrid(inicio, fim, obstaculos, dim):
    grid = [[0 for i in range(dim)] for j in range(dim)]
    grid[inicio[0]][inicio[1]] = 2      #celula inicial
    grid[fim[0]][fim[1]] = 3      #celula objetivo
    for o in obstaculos:
        grid[o[0]][o[1]] = 1    #celula com obstaculo
    for i in range(dim):
        for j in range(dim):
            if (grid[i][j] != 1 and grid[i][j] != 2 and grid[i][j] != 3):       #caminhos validos
                grid[i][j] = 0
    return grid

'''
 /**Metodo para busca a-estrela num cenario onde o usuario deve informar a posicao inicial, a posicao objetivo,
 * e a porcentagem de obstaculos que serao colocados aleatoriamente no cenario. Este e representado por uma matriz
 * de inteiros, onde o valor de cada celula diz se e posicao inicial/objetivo/valida/obstaculo. O algoritmo mantem
 * uma lista (fringe) de nos ainda nao explorados, e a cada iteracao pega o no filho (algum movimento possivel, sendo
 * que e possivel se mover para cima/baixo/direita/esquerda) com o valor mais promissor, ou seja, o de menor custo que
 * mais se aproxima do objetivo. No fim, pode-se ver a rota tomada pelo algoritmo (caso a solucao tenha sido achada)
 * e o custo e o numero de movimentos. Para ver a animacao, apertar qualquer tecla na janela da aplicacao (cada tecla
 * pressionada corresponde a um movimento, a cor da celula mudara).
 */
'''
def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank()

    a = 5
    if rank==0:
        comm.send(a, dest = 1)
        dim = input("Digite o valor (um numero inteiro) de linhas e colunas do grid: ")
        percentObstaculo = input("Digite o percentual de obstaculos (numero decimal): ")
        x, y = raw_input("Digite a posicao x y inicial (2 numeros inteiros): ").split()
        inicio = (int(x),int(y))
        x, y = raw_input("Digite a posicao x y objetivo (2 numeros inteiros): ").split()
        fim = (int(x),int(y))

        numObstaculos = int(dim*dim*percentObstaculo)
        obstaculos = []
        random.seed(10)
        i = 0
        while (1):
            x = random.randint(0, dim-1)
            y = random.randint(0, dim-1)
            novo = (x, y)
            if (novo not in obstaculos and novo != inicio and novo != fim):
                obstaculos.append(novo)
                i = i + 1
            if (i == numObstaculos):
                break

        grid = montaGrid(inicio, fim, obstaculos, dim)

        pygame.init()
        tela = pygame.display.set_mode((600, 600), 0, 32)
        pygame.display.set_caption("Busca A*")
        clock = pygame.time.Clock()
        clock.tick(1000000)

        no = No(inicio[0] , inicio[1])
        fringe = []             #armazena estados nao explorados
        fringe.append(inicio)
        distanciaPercorrida = {}
        distanciaPercorrida[inicio] = 0
        distanciaObjetivo = {}
        distanciaObjetivo[inicio] = no.medeDistanciaObjetivo(inicio, fim)
        heuristicas = {}
        heuristicas[inicio] = distanciaPercorrida[inicio] + distanciaObjetivo[inicio]
        pais = {}
        pais[inicio] = None         #estado inicial nao possui pais

        estadosExpandidos = []
        sucesso = False
        while len(fringe) > 0:
            posicaoAtual = fringe.pop(no.proxEstado(fringe, heuristicas))
            #print("Atual: " + str(posicaoAtual))
            #print("Custo: " + str(no.calculaCustoCorrente(posicaoAtual, pais, heuristicas)))
            if (posicaoAtual == fim):
                sucesso = True
                break
            proxPosicoes = no.novosEstados(grid, posicaoAtual[0], posicaoAtual[1], dim)
            #print("Proximos estados: " + str(proxPosicoes))
            estadosExpandidos.append(posicaoAtual)
            for i in range(len(proxPosicoes)):
                novaPosicao = proxPosicoes[i]
                if ((novaPosicao not in estadosExpandidos) and (novaPosicao not in fringe)):
                    fringe.append(novaPosicao)
                    if (novaPosicao not in heuristicas.keys()):
                        distanciaObjetivo[novaPosicao] = no.medeDistanciaObjetivo(novaPosicao, fim)
                        distanciaPercorrida[novaPosicao] = distanciaPercorrida[posicaoAtual] + 1    #cada avanco tem custo 1
                        heuristicas[novaPosicao] = distanciaObjetivo[novaPosicao] + distanciaPercorrida[novaPosicao]
                        pais[novaPosicao] = posicaoAtual

        if (sucesso):
            rota = no.solucao(posicaoAtual, pais)
            custoTotal = 0
            for p in rota:
                custoTotal = custoTotal + heuristicas[p]
            print ("Rota (posicoes x y): " + str(rota))
            print("Custo total: " + str(custoTotal))
            print("Passos: " + str(len(rota)))
        else:
            print("Nao foi possivel achar a solucao.")

        cont = 0
        while True:
            for event in pygame.event.get():
                if event.type == QUIT:
                    exit()
                if event.type == pygame.KEYDOWN:
                    e = rota[cont]
                    grid[e[0]][e[1]] = 4
                    if (cont < len(rota)-1):
                        cont = cont + 1
            tela.blit(pygame.Surface(tela.get_size()), (0, 0))
            for i in range(dim):
                for j in range(dim):
                    if (grid[i][j] == 2):      #posicao inicial
                        pygame.draw.rect(tela, (50, 50, 255), pygame.Rect(10+j*600/dim-10, 10+i*600/dim-10, 600/dim - (600/dim)/10, 600/dim - (600/dim)/10))
                    elif (grid[i][j] == 3):     #posicao final
                        pygame.draw.rect(tela, (50, 255, 50), pygame.Rect(10+j*600/dim-10, 10+i*600/dim-10, 600/dim - (600/dim)/10, 600/dim - (600/dim)/10))
                    elif (grid[i][j] == 1):     #obstaculo
                        pygame.draw.rect(tela, (255, 50, 50), pygame.Rect(10+j*600/dim-10, 10+i*600/dim-10, 600/dim - (600/dim)/10, 600/dim - (600/dim)/10))
                    elif (grid[i][j] == 4):     #caminho tomado
                        pygame.draw.rect(tela, (50, 255, 255), pygame.Rect(10+j*600/dim-10, 10+i*600/dim-10, 600/dim - (600/dim)/10, 600/dim - (600/dim)/10))
                    else:               #posicao valida
                        pygame.draw.rect(tela, (200, 200, 200), pygame.Rect(10+j*600/dim-10, 10+i*600/dim-10, 600/dim - (600/dim)/10, 600/dim - (600/dim)/10))

            pygame.display.update()

    else:
        comm.recv(source=MPI.ANY_SOURCE)
        #print(string)

main()
