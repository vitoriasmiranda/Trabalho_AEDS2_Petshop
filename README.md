# 🐾 Sistema de Petshop: Busca, Ordenação e Hashing em Disco

Este repositório contém o projeto final da disciplina de Algoritmos e Estruturas de Dados II. O sistema simula o gerenciamento de um Petshop utilizando persistência de dados em arquivos binários (`.dat`) para aplicar e analisar na prática o desempenho de métodos de ordenação, busca e estruturas de dados avançadas diretamente em memória secundária (disco).

---

## 🚀 Funcionalidades

O projeto foi desenvolvido de forma estruturada e incremental, dividido em três grandes etapas de complexidade:

### 🔍 Parte I: Estruturas Básicas e Buscas
* **CRUD em Disco**: Gerenciamento de `Clientes`, `Pets` e `Serviços` com leitura, gravação e atualização de registros operando nativamente em arquivos binários.
* **Análise de Busca**: Implementação e comparação de métricas (tempo e contagem de comparações) entre:
  * **Busca Sequencial** (em arquivos desordenados).
  * **Busca Binária** (em arquivos ordenados).

### 📊 Parte II: Ordenação Externa
* Estudo comparativo de desempenho lidando com bases de dados maiores do que a memória principal (RAM) suporta:
  * **QuickSort Externo**: Adaptação do algoritmo tradicional manipulando ponteiros físicos (`fseek`) no disco.
  * **Classificação Externa (Otimizada)**: Método avançado de banco de dados utilizando **Seleção com Substituição** (para geração de partições baseadas no limite de memória) e **Árvore Binária de Vencedores** (para intercalação eficiente dos arquivos parciais).

### ⚡ Parte III: Tabela Hash e Gerenciamento de Espaço
* **Hashing em Disco (Encadeamento Exterior)**: Acesso ultrarrápido aos dados da entidade `Pet` utilizando o Método da Divisão para calcular o endereço base, com resolução de colisões tratada por encadeamento de nós diretamente no arquivo de dados.
* **Gerenciamento e Reaproveitamento de Espaço**: Implementação de uma **Pilha de Espaços Livres (LIFO)**. Quando um registro é removido, ele é marcado logicamente e seu byte-offset entra no `topo_livre` do cabeçalho do arquivo. Novas inserções reaproveitam inteligentemente esses "buracos", evitando a expansão desnecessária do arquivo e garantindo eficiência máxima de armazenamento.

---

## 🛠️ Tecnologias Utilizadas

* **Linguagem**: C
* **Manipulação de Dados**: Arquivos Binários (`FILE*`, `fread`, `fwrite`, `fseek`, `rewind`)
* **Estruturas Implementadas**: Listas Encadeadas em Disco, Árvores Binárias de Vencedores, Tabelas Hash, Pilha de Reaproveitamento.
* **Ambiente de Execução**: Terminal/Console
