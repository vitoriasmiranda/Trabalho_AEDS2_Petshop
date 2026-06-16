# 🐾 Sistema de Petshop: Busca e Ordenação em Disco

Este repositório contém o projeto final da disciplina de Algoritmos e Estruturas de Dados II. O sistema simula o gerenciamento de um Petshop utilizando persistência de dados em arquivos binários (`.dat`) para aplicar e analisar na prática o desempenho de métodos de ordenação e busca em memória secundária (disco).

---

## 🚀 Funcionalidades

O projeto é dividido em duas frentes principais: gerenciamento de entidades e análise de algoritmos.

* **CRUD em Disco**: Gerenciamento completo de `Clientes`, `Pets` e `Serviços` com leitura, gravação e atualização de registros diretamente em arquivos binários.
* **Algoritmos de Busca**: Implementação e comparação de métricas (tempo e comparações) entre:
    * Busca Sequencial (em arquivos desordenados).
    * Busca Binária (em arquivos ordenados).
* **Ordenação Externa**: Estudo comparativo de desempenho entre:
    * **QuickSort Externo**: Adaptação do algoritmo tradicional manipulando ponteiros no disco.
    * **Classificação Externa**: Método avançado utilizando **Seleção com Substituição** (para gerar partições) e **Árvore Binária de Vencedores** (para intercalação).

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem**: C
* **Manipulação de Dados**: Arquivos Binários (`FILE*`, `fread`, `fwrite`, `fseek`)
* **Ambiente de Execução**: Terminal/Console
