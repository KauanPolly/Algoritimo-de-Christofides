#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define MAX 100

// Definindo uma estrutura para representar uma cidade
typedef struct {
    int x, y; // Coordenadas da cidade
} City;

// Função para calcular a distância entre duas cidades
int calculateDistance(City city1, City city2) {
    return (int)round(sqrt(pow(city1.x - city2.x, 2) + pow(city1.y - city2.y, 2)));
}

// Função para encontrar a árvore geradora mínima
int findMinKey(int key[], int mstSet[], int V) {
    int min = INT_MAX, minIndex;

    for (int v = 0; v < V; v++) {
        if (!mstSet[v] && key[v] < min) {
            min = key[v];
            minIndex = v;
        }
    }
    return minIndex;
}

// Função para construir a árvore geradora mínima
void primMST(int graph[MAX][MAX], int V, int parent[]) {
    int key[MAX], mstSet[MAX];

    for (int i = 0; i < V; i++) {
        key[i] = INT_MAX;
        mstSet[i] = 0;
    }

    key[0] = 0;
    parent[0] = -1;

    for (int count = 0; count < V - 1; count++) {
        int u = findMinKey(key, mstSet, V);
        mstSet[u] = 1;

        for (int v = 0; v < V; v++) {
            if (graph[u][v] && !mstSet[v] && graph[u][v] < key[v]) {
                parent[v] = u;
                key[v] = graph[u][v];
            }
        }
    }
}

// Função para encontrar vértices com grau ímpar
void findOddVertices(int V, int parent[], int oddVertices[], int *oddCount) {
    int degree[MAX] = {0};
    for (int i = 1; i < V; i++) {
        degree[i]++;
        degree[parent[i]]++;
    }
    for (int i = 0; i < V; i++) {
        if (degree[i] % 2 == 1) {
            oddVertices[(*oddCount)++] = i;
        }
    }
}

// Função para realizar emparelhamento perfeito mínimo
void perfectMatching(int graph[MAX][MAX], int oddVertices[], int oddCount, int matching[MAX][2]) {
    int used[MAX] = {0}, matchIndex = 0;

    for (int i = 0; i < oddCount; i++) {
        if (!used[oddVertices[i]]) {
            int minEdge = INT_MAX, minVertex = -1;
            for (int j = i + 1; j < oddCount; j++) {
                if (!used[oddVertices[j]] && graph[oddVertices[i]][oddVertices[j]] < minEdge) {
                    minEdge = graph[oddVertices[i]][oddVertices[j]];
                    minVertex = j;
                }
            }
            if (minVertex != -1) {
                used[oddVertices[i]] = used[oddVertices[minVertex]] = 1;
                matching[matchIndex][0] = oddVertices[i];
                matching[matchIndex][1] = oddVertices[minVertex];
                matchIndex++;
            }
        }
    }
}

// Função para encontrar o circuito de Euler
void eulerianCircuit(int graph[MAX][MAX], int V, int parent[], int matching[MAX][2], int matchCount, int circuit[], int *circuitSize) {
    int visited[MAX] = {0};

    for (int i = 0; i < V; i++) {
        visited[i] = 0;
    }

    // Adicionando arestas da MST ao circuito
    for (int i = 1; i < V; i++) {
        if (!visited[parent[i]]) {
            circuit[(*circuitSize)++] = parent[i];
            visited[parent[i]] = 1;
        }
    }

    // Adicionando arestas do emparelhamento perfeito ao circuito
    for (int i = 0; i < matchCount; i++) {
        if (!visited[matching[i][0]] || !visited[matching[i][1]]) {
            circuit[(*circuitSize)++] = matching[i][0];
            circuit[(*circuitSize)++] = matching[i][1];
            visited[matching[i][0]] = visited[matching[i][1]] = 1;
        }
    }
}

// Função para converter o circuito Euleriano em um caminho Hamiltoniano
void eulerianToHamiltonian(int circuit[], int circuitSize, int path[], int *pathSize, int V, int graph[MAX][MAX]) {
    int visited[MAX] = {0};

    for (int i = 0; i < circuitSize; i++) {
        if (!visited[circuit[i]]) {
            visited[circuit[i]] = 1;
            path[(*pathSize)++] = circuit[i];
        }
    }

    // Verificar se todos os vértices foram visitados
    for (int i = 0; i < V; i++) {
        if (!visited[i]) {
            // Encontrar a posição para inserir o vértice ausente
            int pos = 0;
            while (pos < *pathSize - 1 && graph[path[pos]][i] + graph[i][path[pos + 1]] <= graph[path[pos]][path[pos + 1]]) {
                pos++;
            }
            // Deslocar os vértices à direita para inserir o vértice ausente
            for (int j = *pathSize; j > pos; j--) {
                path[j] = path[j - 1];
            }
            path[pos] = i;
            (*pathSize)++;
            visited[i] = 1;
        }
    }
}

// Função para calcular o fatorial de um número
unsigned long long factorial(int n) {
    if (n == 0 || n == 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Função para o algoritmo Twice Around the Tree
void twiceAroundTree(int graph[MAX][MAX], int V, int parent[], int path[], int *pathSize) {
    int visited[MAX] = {0};
    *pathSize = 0;

    // Função auxiliar para realizar a DFS na árvore
    void dfs(int v) {
        visited[v] = 1;
        path[(*pathSize)++] = v;
        for (int i = 0; i < V; i++) {
            if (!visited[i] && parent[i] == v) {
                dfs(i);
                path[(*pathSize)++] = v; // Voltar para v após visitar i
            }
        }
    }

    dfs(0); // Iniciar a DFS a partir do vértice 0

    // Remover duplicatas para converter em caminho Hamiltoniano
    int finalPath[MAX] = {0}, finalPathSize = 0;
    for (int i = 0; i < *pathSize; i++) {
        if (!finalPath[path[i]]) {
            finalPath[path[i]] = 1;
            path[finalPathSize++] = path[i];
        }
    }
    *pathSize = finalPathSize;
}

// Função principal para encontrar a solução do TSP usando o algoritmo de Christofides e Twice Around the Tree
void christofides(City cities[], int V) {
    // Construir o grafo completo das distâncias entre as cidades
    int graph[MAX][MAX];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = calculateDistance(cities[i], cities[j]);
        }
    }

    int parent[MAX];
    primMST(graph, V, parent);

    int oddVertices[MAX], oddCount = 0;
    findOddVertices(V, parent, oddVertices, &oddCount);

    int matching[MAX][2] = {0};

    perfectMatching(graph, oddVertices, oddCount, matching);

    int circuit[MAX * 2], circuitSize = 0;
    eulerianCircuit(graph, V, parent, matching, oddCount / 2, circuit, &circuitSize);

    int path[MAX], pathSize = 0;
    eulerianToHamiltonian(circuit, circuitSize, path, &pathSize, V, graph);

    // Imprimindo o caminho resultante do algoritmo de Christofides
    printf("Caminho Hamiltoniano (Christofides): ");
    for (int i = 0; i < pathSize; i++) {
        printf("%d ", path[i]);
    }
    printf("\n");

    // Calculando e imprimindo o número de soluções possíveis
    unsigned long long totalSolutions = factorial(V - 1);
    printf("Numero de solucoes possiveis (caminhos): %llu\n", totalSolutions);

    // Algoritmo Twice Around the Tree
    int twicePath[MAX];
    int twicePathSize = 0;
    twiceAroundTree(graph, V, parent, twicePath, &twicePathSize);

    // Imprimindo o caminho resultante do algoritmo Twice Around the Tree
    printf("Caminho Hamiltoniano (Twice Around the Tree): ");
    for (int i = 0; i < twicePathSize; i++) {
        printf("%d ", twicePath[i]);
    }
    printf("\n");
}

int main() {
    int V; // Número de cidades
    printf("Digite o numero de cidades: ");
    scanf("%d", &V);

    // Lendo as coordenadas das cidades
    City cities[MAX];
    printf("Digite as coordenadas das cidades:\n");
    for (int i = 0; i < V; i++) {
        printf("Cidade %d (x y): ", i + 1);
        scanf("%d %d", &cities[i].x, &cities[i].y);
    }

    christofides(cities, V);
    return 0;
}
