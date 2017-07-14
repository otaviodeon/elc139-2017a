from math import sqrt, pow

'''
/** Classe para objetos do tipo no, que contem metodos de auxilio para o algoritmo a-estrela e valores.
 *  @author Otavio Deon
 */
 '''

class No(object):

    def __init__(self, x, y):
        self.x = x
        self.y = y

    '''
    /** Metodo para achar possiveis estados a partir de uma posicao no grid, sendo que os estados retornados
     *  nao podem ser obstaculos e devem respeitar as dimensoes do grid.
     *  @param  grid Matrix - configuracao do cenario do programa, representado numa matriz dim x dim
     *  @param  x, y Int - valores referentes a posicao (x,y) no grid a partir da qual se quer achar novos estados
     *  @param  dim Int - dimensoes do grid (linhas e colunas)
     *  @return list - lista de proximos estados possiveis
     */
    '''
    def novosEstados(self, grid, x, y, dim):
        estados = []
        if (x > 0 and grid[x-1][y] != 1):   #vai para cima
            estados.append((x-1, y))
        if (y > 0 and grid[x][y-1] != 1):   #vai para esquerda
            estados.append((x, y-1))
        if (x < dim-1 and grid[x+1][y] != 1):   #vai para baixo
            estados.append((x+1, y))
        if (y < dim-1 and grid[x][y+1] != 1):   #vai para direita
            estados.append((x, y+1))
        return estados

    '''
     /**Metodo que calcula distancia euclidiana entre dois pontos, usado como auxilio na funcao heuristica.
     * @param  inicio Tuple - tupla com valores (x, y) relativos ao ponto inicial do calculo da distancia
     * @param  fim Tuple - tupla com valores (x, y) relativos ao ponto final do calculo da distancia
     * @return Double - valor da distancia
     */
    '''
    def medeDistanciaObjetivo(self, inicio, fim):
        return sqrt(pow(fim[0]-inicio[0], 2) + pow(fim[1]-inicio[1], 2))       #distancia euclidiana

    '''
     /**Metodo que retorna o proximo estado a ser explorado, usando o criterio de menor heuristica.
     * @param  fringe List - lista com os estados ainda nao explorados
     * @param  h List - lista com valores de heuristica que serao comparados
     * @return Int - indice do proximo estado
     */
    '''
    def proxEstado(self, fringe, h):
        custoMinimo = 99999999
        i = 0
        for e in fringe:
            if (h[e]) < custoMinimo:
                custoMinimo = h[e]
                indiceProxEstado = i
            i = i + 1
        return indiceProxEstado

    '''
     /**Metodo que retorna a rota a ser tomada para alcancar o objetivo no menor caminho. Tem como base a posicao
     * final, e a partir dela, vai pegando seus pais (quem chamou) ate chegar na raiz da arvore.
     * @param  posicao Tuple - posicao atual (final)
     * @param  pais List - lista dos nos pai dos estados
     * @return List - lista de tuplas, onde cada tupla contem uma posicao (x,y) na ordem da rota a ser tomada
     */
    '''
    def solucao(self, posicao, pais):
        rota = []
        rota.append(posicao)
        while (pais[posicao] != None):
            rota.append(pais[posicao])
            posicao = pais[posicao]
        return rota[::-1]          #ordem inicio->fim

    '''
     /**Metodo para calcular o custo atual do desenvolvimento do a-estrela
     * @param  posicao Tuple - posicao atual em que se encontra
     * @param  pais List - lista dos nos pai dos estados
     * @param  h List - lista das heuristicas dos estados
     * @return Double - valor do custo ate o estado atual do grafo
     */
    '''
    def calculaCustoCorrente(self, posicao, pais, h):
        rota = []
        rota.append(posicao)
        while (pais[posicao] != None):
            rota.append(pais[posicao])
            posicao = pais[posicao]
        custo = 0
        for r in rota:
            custo = custo + h[r]
        return custo
