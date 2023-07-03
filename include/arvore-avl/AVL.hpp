
#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

template <typename T>
class No {
 public:
  T chave;
  No* esquerda;

  No* direita;
  int altura;

  // Construtor
  No(const T& chave)
      : chave(chave), esquerda(nullptr), direita(nullptr), altura(1) {}
};

template <typename T>
class ArvoreAVL {
 private:
  No<T>* raiz;

  // Função para obter a altura de um nó
  int altura_No(No<T>* no) {
    if (no == nullptr) return 0;
    return no->altura;
  }

  // Função para obter o fator de balanceamento de um nó
  int fator_B(No<T>* no) {
    if (no == nullptr) return 0;
    return altura_No(no->esquerda) - altura_No(no->direita);
  }

  // Função para atualizar a altura de um nó
  void att_Altura(No<T>* no) {
    if (no == nullptr) return;
    no->altura = std::max(altura_No(no->esquerda), altura_No(no->direita)) + 1;
  }

  // Função para fazer uma rotação simples à esquerda
  No<T>* gira_esquerda(No<T>* no) {
    No<T>* raizNova = no->direita;
    no->direita = raizNova->esquerda;
    raizNova->esquerda = no;
    att_Altura(no);
    att_Altura(raizNova);
    return raizNova;
  }

  // Função para fazer uma rotação simples à direita
  No<T>* gira_direita(No<T>* no) {
    No<T>* raizNova = no->esquerda;
    no->esquerda = raizNova->direita;
    raizNova->direita = no;
    att_Altura(no);
    att_Altura(raizNova);
    return raizNova;
  }

  // Função para fazer uma rotação dupla esquerda-direita
  No<T>* gira_esquerdadireita(No<T>* no) {
    no->esquerda = gira_esquerda(no->esquerda);
    return gira_direita(no);
  }

  // Função para fazer uma rotação dupla direita-esquerda
  No<T>* gira_direitaesquerda(No<T>* no) {
    no->direita = gira_direita(no->direita);
    return gira_esquerda(no);
  }

  // Função para balancear um nó
  No<T>* balance(No<T>* no) {
    if (no == nullptr) return nullptr;

    att_Altura(no);

    int balanceFactor = fator_B(no);

    if (balanceFactor > 1) {
      if (fator_B(no->esquerda) >= 0) {
        // std::cout << "rota direita" << std::endl;
        return gira_direita(no);
      } else {
        // std::cout << "dupla esquerda direita" << std::endl;
        return gira_esquerdadireita(no);
      }
    } else if (balanceFactor < -1) {
      if (fator_B(no->direita) <= 0) {
        // std::cout << "rota esquerda" << std::endl;
        return gira_esquerda(no);
      } else {
        // std::cout << "dupla direita esquerda" << std::endl;
        return gira_direitaesquerda(no);
      }
    }
    // std::cout << " | fator balanceado = " << fator_B(no) << " | " <<
    // std::endl;
    return no;
  }

  // Função auxiliar para inserir um nó na árvore
  No<T>* inserir_No(No<T>* no, const T& chave) {
    if (no == nullptr) {
      // std::cout << "no vazio, criar um" << std::endl;
      return new No<T>(chave);
    }
    if (chave < no->chave) {
      no->esquerda = inserir_No(no->esquerda, chave);
      // std::cout << no->chave << "<-" << chave << std::endl;
    }
    if (chave > no->chave) {
      no->direita = inserir_No(no->direita, chave);
      // std::cout << no->chave << "->" << chave << std::endl;
    }
    if (chave == no->chave) {
      // std::cout << no->chave << "==" << chave << std::endl;
      return no;
    }

    return balance(no);
  }

  // Função auxiliar para buscar um nó na árvore
  No<T>* buscar_No(No<T>* no, const T& chave) {
    if (no == nullptr || no->chave == chave) {
      // std::cout << chave << " = " << no->chave << std::endl;
      return no;
    }
    if (chave < no->chave) {
      // std::cout << chave << " < " << no->chave << std::endl;
      return buscar_No(no->esquerda, chave);
    }
    // std::cout << chave << " > " << no->chave << std::endl;
    return buscar_No(no->direita, chave);
  }

  // Função auxiliar para encontrar o nó com menor valor
  No<T>* encontrar_Minimo(No<T>* no) {
    if (no == nullptr || no->esquerda == nullptr) return no;

    return encontrar_Minimo(no->esquerda);
  }

  // Função auxiliar para remover um nó da árvore
  No<T>* remover_No(No<T>* no, const T& chave) {
    if (no == nullptr) return no;

    if (chave < no->chave)
      no->esquerda = remover_No(no->esquerda, chave);
    else if (chave > no->chave)
      no->direita = remover_No(no->direita, chave);
    else {
      if (no->esquerda == nullptr || no->direita == nullptr) {
        No<T>* temp = no->esquerda ? no->esquerda : no->direita;

        if (temp == nullptr) {
          temp = no;
          no = nullptr;
        } else {
          *no = *temp;
        }

        delete temp;

      } else {
        No<T>* temp = encontrar_Minimo(no->direita);
        no->chave = temp->chave;
        no->direita = remover_No(no->direita, temp->chave);
      }
    }

    if (no == nullptr) return no;

    return balance(no);
  }

  // Função auxiliar para percorrer a árvore em ordem
  void percorrer_Ordem(No<T>* no) {
    if (no == nullptr) return;

    percorrer_Ordem(no->esquerda);
    // std::cout << no->chave << " ";
    percorrer_Ordem(no->direita);
  }

 public:
  // Construtor
  ArvoreAVL() : raiz(nullptr) {}

  // Função para inserir uma chave na árvore
  void inserir(const T& chave) { raiz = inserir_No(raiz, chave); }

  // Função para remover uma chave da árvore
  void remover(const T& chave) { raiz = remover_No(raiz, chave); }

  // Função para buscar uma chave na árvore
  bool buscar(const T& chave) { return buscar_No(raiz, chave) != nullptr; }

  // Função para percorrer a árvore em ordem
  void AVL_em_Ordem(No<T>* no) {
    if (no == nullptr) return;
    AVL_em_Ordem(no->esquerda);
    // std::cout << no->chave << " ";
    AVL_em_Ordem(no->direita);
  }

  // Função para percorrer a árvore em ordem
  void percorrer_Ordem() { AVL_em_Ordem(raiz); }

  auto medirTempoBusca(std::ofstream& arquivo) {
    const int numExecucoes =
        5000000;  // Número de execuções para calcular a média de tempo
    // std::array<int, numExecucoes> arr{};
    // for (int i = 0; i < arr.size(); i++) arr.at(i) = i;
    // std::random_shuffle(arr.begin(), arr.end());

    arquivo << "TamanhoArvore"
            << ";"
            << "NanoSeg"
            << "\n";

    for (int i = 0; i < numExecucoes; i++) {
      auto chave = std::to_string(i);
      inserir(chave);

      auto inicio = std::chrono::steady_clock::now();
      buscar(chave);

      auto fim = std::chrono::steady_clock::now();
      auto duracao = fim - inicio;
      auto NanoSeg =
          std::chrono::duration_cast<std::chrono::nanoseconds>(duracao).count();
      // tempoTotal += duracao;
      arquivo << i << ";" << NanoSeg << "\n";
    }

    return 0;
  }

  double medirTempoInsercao(std::ofstream& arquivo) {
    // Número de execuções para calcular a média de tempo
    const int numExecucoes = 5000000;
    // std::array<int, numExecucoes> arr{};

    // for (auto i = 0; i < arr.size(); i++) arr.at(i) = i;
    // std::random_shuffle(arr.begin(), arr.end());

    arquivo << "TamanhoArvore"
            << ";"
            << "NanoSeg"
            << "\n";

    for (int i = 0; i < numExecucoes; i++) {
      auto chave = std::to_string(i);

      auto inicio = std::chrono::steady_clock::now();  // NOLINT

      inserir(chave);

      // buscar(std::to_string(arr.at(i)));
      auto fim = std::chrono::steady_clock::now();
      auto duracao = fim - inicio;
      auto NanoSeg =
          std::chrono::duration_cast<std::chrono::nanoseconds>(duracao)
              .count();
      arquivo << i << ";" << NanoSeg << "\n";
    }

    return 0;
  }

  double medirTempoRemocao(std::ofstream& arquivo) {
    const int numExecucoes = 5000000;
    // std::array<int, numExecucoes> arr{};

    // for (auto i = 0; i < arr.size(); i++) arr.at(i) = i;
    // std::random_shuffle(arr.begin(), arr.end());

    arquivo << "TamanhoArvore"
            << ";"
            << "NanoSeg"
            << "\n";

    for (int i = 0; i < numExecucoes; i++) {
      auto chave = std::to_string(i);

      auto inicio = std::chrono::steady_clock::now();
      remover(chave);
      // buscar(std::to_string(arr.at(i)));
      auto fim = std::chrono::steady_clock::now();

      auto duracao = fim - inicio;
      auto NanoSeg =
          std::chrono::duration_cast<std::chrono::nanoseconds>(duracao)
              .count();

      arquivo << i << ";" << NanoSeg << "\n";
    }
  

    return 0;
  }
};
