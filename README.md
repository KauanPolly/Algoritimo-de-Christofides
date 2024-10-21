Programa de Distância entre Cidades e Árvore Geradora Mínima (MST)
Este programa calcula a árvore geradora mínima (MST) para um conjunto de cidades representadas por suas coordenadas em um plano 2D. A MST é calculada usando o algoritmo de Prim. O programa também calcula a distância entre as cidades utilizando a fórmula da distância Euclidiana.

Funcionalidades
Cálculo de Distância: Calcula a distância Euclidiana entre duas cidades, dadas suas coordenadas (x, y).
Árvore Geradora Mínima (MST): Usa o algoritmo de Prim para encontrar a MST, conectando todas as cidades com a menor soma de distâncias.
Estrutura do Código
Estrutura de Cidade: As cidades são representadas por uma estrutura que contém as coordenadas x e y.
Função de Cálculo de Distância: Calcula a distância entre duas cidades usando a fórmula da distância Euclidiana.
Função para Encontrar a MST: Implementa o algoritmo de Prim para gerar a MST, minimizando as distâncias entre as cidades.
Como Usar
Compile o código C usando um compilador como gcc:

bash
Copiar código
gcc -o mst_program main.c -lm
Execute o programa:

bash
Copiar código
./mst_program
Insira os dados das cidades conforme solicitado.

Dependências
Biblioteca padrão do C (stdio.h, stdlib.h)
Biblioteca math.h para funções matemáticas
Exemplo de Execução
O programa solicitará as coordenadas das cidades e calculará a árvore geradora mínima, exibindo a ordem das cidades conectadas e as distâncias.
