// SPDX-License-Identifier:  MIT
// Copyright (C) 2023 - Prof. Igor Machado Coelho
// ?(jota-ve): Devo atualizar o copyright acima para os integrantes do grupo

#include <cassert>
#include <iostream>
#include <memory>
//
#include <fmt/core.h>  // NOLINT

// #include <../tests/test.hpp>
#include "../include/arvore-avl/AVL.hpp"

using fmt::print;


int main() {
    ArvoreAVL<std::string> arvore;
    std::ofstream arquivo_i("tempos_Insercao.txt");
    std::ofstream arquivo_b("tempos_Busca.txt");
    std::ofstream arquivo_r("tempos_Remocao.txt");
    // Inserir elementos na árvore
    double tempoInsercao = arvore.medirTempoInsercao(arquivo_i);
    arquivo_i.close();
    // Inprimir a árvore
    std::cout << "Árvore em ordem: ";
    arvore.percorrer_Ordem();
    std::cout << std::endl;
    // Realizar uma busca
    double tempoBusca = arvore.medirTempoBusca(arquivo_b);
    arquivo_b.close();
    // Remover um elemento da árvore
    double tempoRemocao = arvore.medirTempoRemocao(arquivo_r);
    arquivo_r.close();

    std::cout << "Tempo medio de insercao: " << tempoInsercao << " milisegundos" << std::endl;
    std::cout << "Tempo medio de remocao: " << tempoRemocao << " milisegundos" << std::endl;
    std::cout << "Tempo medio de busca: " << tempoBusca << " milisegundos" << std::endl;

    return 0;
}

// Ctrl+Shift+P
// CMake: build
// CMake: configure
// CMake: debug
// [aplicacao]
