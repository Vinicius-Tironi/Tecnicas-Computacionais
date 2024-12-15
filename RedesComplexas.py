# -*- coding: utf-8 -*-
import networkx as nx
import matplotlib.pyplot as plt
import numpy as np

def rede_complexa(arquivo):
    G = nx.Graph()
    with open(arquivo, "r") as f:
        linhas = f.readlines()

    for i, linha in enumerate(linhas):
        valores = linha.strip().split()
        for j, valor in enumerate(valores):
            peso = int(valor)
            if peso != 0:
                G.add_edge(i + 1, j + 1, weight=peso)

    return G

grafo = rede_complexa("C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\dolphins_matriz_adjacencia.mtx")

# Dados:
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\dolphins_matriz_adjacencia.mtx"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\macaques_matriz_adjacencia_peso.edges"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\insects_matriz_adjacencia_peso.edges"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\kangaroo_matriz_adjacencia_peso.edges"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\jazz_matriz_adjacencia_peso.edges"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\tribes_matriz_adjacencia_peso.edges"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\metabolic_matriz_adjacencia.mtx"
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\enzymes_matriz_adjacencia.mtx"

# Tribes modificado
# "C:\\Users\\Vinicius\\Documents\\CodeBlocks\\Projeto 3\\Matrizes Adjacencia\\mod_tribes_matriz_adjacencia_peso.edges"

pos = nx.spring_layout(grafo, k=0.15, iterations=20, seed=42)
plt.figure(figsize=(10, 10))
cores_nos = plt.cm.viridis(np.linspace(0, 1, len(grafo.nodes)))
nx.draw_networkx_nodes(grafo, pos, node_size=100, node_color="red", edgecolors="black")

# Tribes apresenta pesos negativos
pesos = nx.get_edge_attributes(grafo, "weight")
arestas_positivas = [(u, v) for (u, v, w) in grafo.edges(data="weight") if w > 0]
arestas_negativas = [(u, v) for (u, v, w) in grafo.edges(data="weight") if w < 0]

print('Quantidade de arestas: positivas, negativas' , len(arestas_positivas), len(arestas_negativas))

nx.draw_networkx_edges(grafo, pos, edgelist=arestas_positivas, width=[pesos[aresta] / 5 for aresta in arestas_positivas], style='solid')
nx.draw_networkx_edges(grafo, pos, edgelist=arestas_negativas, width=[abs(pesos[aresta]) / 5 for aresta in arestas_negativas], style='dashed')

plt.title("Rede Complexa - Dados", fontsize=20)
plt.axis("off")
plt.show()

# Matriz de adjacência
plt.figure(figsize=(10, 10))
matriz_adjacencia = nx.to_numpy_array(grafo)
plt.imshow(matriz_adjacencia, cmap="viridis", interpolation="none")
plt.colorbar()

plt.title("Matriz de Adjacência - Dados", fontsize=20)
plt.axis("off")
plt.show()

