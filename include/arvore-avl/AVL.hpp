

#pragma once

#include <chrono> // NOLINT [build/c++11]
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>


template <typename T> class No {
public:
  T chave;
  No *esquerda;
  No *direita;
  int altura;

  // Construtor
  No(const T &chave)
      : chave(chave), esquerda(nullptr), direita(nullptr), altura(1) {}
};

template <typename T> class ArvoreAVL {
private:
  No<T> *raiz;

  // Função para obter a altura de um nó
  int altura_No(No<T> *no) {
    if (no == nullptr)
      return 0;
    return no->altura;
  }

  // Função para obter o fator de balanceamento de um nó
  int fator_B(No<T> *no) {
    if (no == nullptr)
      return 0;
    return altura_No(no->esquerda) - altura_No(no->direita);
  }

  // Função para atualizar a altura de um nó
  void att_Altura(No<T> *no) {
    if (no == nullptr)
      return;
    no->altura = std::max(altura_No(no->esquerda), altura_No(no->direita)) + 1;
  }

  // Função para fazer uma rotação simples à esquerda
  No<T> *gira_esquerda(No<T> *no) {
    No<T> *raizNova = no->direita;
    no->direita = raizNova->esquerda;
    raizNova->esquerda = no;
    att_Altura(no);
    att_Altura(raizNova);
    return raizNova;
  }

  // Função para fazer uma rotação simples à direita
  No<T> *gira_direita(No<T> *no) {
    No<T> *raizNova = no->esquerda;
    no->esquerda = raizNova->direita;
    raizNova->direita = no;
    att_Altura(no);
    att_Altura(raizNova);
    return raizNova;
  }

  // Função para fazer uma rotação dupla esquerda-direita
  No<T> *gira_esquerdadireita(No<T> *no) {
    no->esquerda = gira_esquerda(no->esquerda);
    return gira_direita(no);
  }

  // Função para fazer uma rotação dupla direita-esquerda
  No<T> *gira_direitaesquerda(No<T> *no) {
    no->direita = gira_direita(no->direita);
    return gira_esquerda(no);
  }

  // Função para balancear um nó
  No<T> *balance(No<T> *no) {
    if (no == nullptr)
      return nullptr;

    att_Altura(no);

    int balanceFactor = fator_B(no);

    if (balanceFactor > 1) {
      if (fator_B(no->esquerda) >= 0)
        return gira_direita(no);
      else
        return gira_esquerdadireita(no);
    } else if (balanceFactor < -1) {
      if (fator_B(no->direita) <= 0)
        return gira_esquerda(no);
      else
        return gira_direitaesquerda(no);
    }

    return no;
  }

  // Função auxiliar para inserir um nó na árvore
  No<T> *inserir_No(No<T> *no, const T &chave) {
    if (no == nullptr)
      return new No<T>(chave);

    if (chave < no->chave)
      no->esquerda = inserir_No(no->esquerda, chave);
    else if (chave > no->chave)
      no->direita = inserir_No(no->direita, chave);
    else
      return no;

    return balance(no);
  }

  // Função auxiliar para buscar um nó na árvore
  No<T> *buscar_No(No<T> *no, const T &chave) {
    if (no == nullptr || no->chave == chave)
      return no;

    if (chave < no->chave)
      return buscar_No(no->esquerda, chave);

    return buscar_No(no->direita, chave);
  }

  // Função auxiliar para encontrar o nó com menor valor
  No<T> *encontrar_Minimo(No<T> *no) {
    if (no == nullptr || no->esquerda == nullptr)
      return no;

    return encontrar_Minimo(no->esquerda);
  }

  // Função auxiliar para remover um nó da árvore
  No<T> *remover_No(No<T> *no, const T &chave) {
    if (no == nullptr)
      return no;

    if (chave < no->chave)
      no->esquerda = remover_No(no->esquerda, chave);
    else if (chave > no->chave)
      no->direita = remover_No(no->direita, chave);
    else {
      if (no->esquerda == nullptr || no->direita == nullptr) {
        No<T> *temp = no->esquerda ? no->esquerda : no->direita;

        if (temp == nullptr) {
          temp = no;
          no = nullptr;
        } else
          *no = *temp;

        delete temp;
      } else {
        No<T> *temp = encontrar_Minimo(no->direita);
        no->chave = temp->chave;
        no->direita = remover_No(no->direita, temp->chave);
      }
    }

    if (no == nullptr)
      return no;

    return balance(no);
  }

  // Função auxiliar para percorrer a árvore em ordem
  void percorrer_Ordem(No<T> *no) {
    if (no == nullptr)
      return;

    percorrer_Ordem(no->esquerda);
    // std::cout << no->chave << " ";
    percorrer_Ordem(no->direita);
  }

public:
  // Construtor
  ArvoreAVL() : raiz(nullptr) {}

  // Função para inserir uma chave na árvore
  void inserir(const T &chave) { raiz = inserir_No(raiz, chave); }

  // Função para remover uma chave da árvore
  void remover(const T &chave) { raiz = remover_No(raiz, chave); }

  // Função para buscar uma chave na árvore
  bool buscar(const T &chave) { return buscar_No(raiz, chave) != nullptr; }

  // Função para percorrer a árvore em ordem
  void AVL_em_Ordem(No<T> *no) {
    if (no == nullptr)
      return;
    AVL_em_Ordem(no->esquerda);
    // std::cout << no->chave << " ";
    AVL_em_Ordem(no->direita);
  }

  // Função para percorrer a árvore em ordem
  void percorrer_Ordem() { AVL_em_Ordem(raiz); }

  double medirTempoBusca(std::ofstream &arquivo) {
    const int numExecucoes =
        10000;  // Número de execuções para calcular a média de tempo
    double tempoTotal = 0.0;

    arquivo << std::fixed
            << std::setprecision(
                   10);  // Configura a precisão decimal para 10 casas

    for (int i = 0; i < numExecucoes; ++i) {
      std::chrono::high_resolution_clock::time_point startTime =
          std::chrono::high_resolution_clock::now();

      buscar(std::to_string(i));

      std::chrono::high_resolution_clock::time_point endTime =
          std::chrono::high_resolution_clock::now();

      std::chrono::duration<double, std::milli> elapsedTime =
          endTime - startTime;
        
      tempoTotal += elapsedTime.count();
      arquivo << i << ";" << elapsedTime.count()
              << "\n";  // Grava o tempo com todas as casas decimais
    }

    double tempoMedio = tempoTotal / numExecucoes;
    return tempoMedio;
  }

  double medirTempoInsercao(std::ofstream &arquivo) {
    const int numExecucoes =
        10000;  // Número de execuções para calcular a média de tempo
    double tempoTotal = 0.0;

    arquivo << std::fixed
            << std::setprecision(
                   10);  // Configura a precisão decimal para 10 casas

    for (int i = 0; i < numExecucoes; ++i) {
      std::chrono::high_resolution_clock::time_point startTime =
          std::chrono::high_resolution_clock::now();
      inserir(std::to_string(i));
      std::chrono::high_resolution_clock::time_point endTime =
          std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsedTime =
          endTime - startTime;
      tempoTotal += elapsedTime.count();
      arquivo << i << ";" << elapsedTime.count() << "\n";
    }
    double tempoMedio = tempoTotal / numExecucoes;
    return tempoMedio;
  }

  double medirTempoRemocao(std::ofstream &arquivo) {
    const int numExecucoes =
        10000;  // Número de execuções para calcular a média de tempo
    double tempoTotal = 0.0;

    arquivo << std::fixed
            << std::setprecision(
                   10);  // Configura a precisão decimal para 10 casas

    for (int i = 0; i < numExecucoes; ++i) {
      std::chrono::high_resolution_clock::time_point startTime =
          std::chrono::high_resolution_clock::now();
      remover(std::to_string(i));
      std::chrono::high_resolution_clock::time_point endTime =
          std::chrono::high_resolution_clock::now();
      std::chrono::duration<double, std::milli> elapsedTime =
          endTime - startTime;
      tempoTotal += elapsedTime.count();
      arquivo << i << ";" << elapsedTime.count() << "\n";
    }
    double tempoMedio = tempoTotal / numExecucoes;
    return tempoMedio;
  }
};
