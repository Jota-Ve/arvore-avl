

#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include "../include/arvore-avl/AVL.hpp"

int main() {
  ArvoreAVL<std::string> arvore;
  std::ofstream arquivo_i("../benchmark/inserir.csv", std::ios::out);
  std::ofstream arquivo_b("../benchmark/buscar.csv", std::ios::out);
  std::ofstream arquivo_r("../benchmark/remover.csv", std::ios::out);

  // Número de execuções para calcular a média de tempo
  const int numExecucoes = 500000;

  // Inserir elementos na árvore
  std::cout << "Medindo inserção\n";
  double tempoInsercao = arvore.medirTempoInsercao(arquivo_i);
  arquivo_i.close();

  // Remover um elemento da árvore
  std::cout << "Medindo Remoção\n";
  double tempoRemocao = arvore.medirTempoRemocao(arquivo_r);
  arquivo_r.close();

  // Realizar uma busca
  std::cout << "Medindo Busca\n";
  double tempoBusca = arvore.medirTempoBusca(arquivo_b);
  arquivo_b.close();

  return 0;
}