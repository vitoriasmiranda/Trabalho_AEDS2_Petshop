#ifndef PETSHOP_H_INCLUDED
#define PETSHOP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cliente {
    int cod;
    char nome[50];
    char telefone[15];
} TCliente;

typedef struct Pet {
    int cod;
    char nome[50];
    char especie[30];
    int cod_cliente; 
} TPet;

typedef struct Servico {
    int cod;
    int cod_pet;
    char descricao[50];
    double preco;
} TServico;

TPet *criar_pet(int cod, char *nome, char *especie, int cod_cliente);
void salva_pet(TPet *pet, FILE *out);
TPet *le_pet(FILE *in);
void imprime_pet(TPet *pet);
int tamanho_registro_pet();

TCliente *criar_cliente(int cod, char *nome, char *telefone);
void salva_cliente(TCliente *cliente, FILE *out);
int tamanho_registro_cliente();
void atualiza_cliente(FILE *in_out, int posicao_do_cliente, TCliente *cliente_novo);

TServico *criar_servico(int cod, int cod_pet, char *descricao, double preco);
void salva_servico(TServico *servico, FILE *out);
void atualiza_pet(FILE *in_out, int posicao_do_pet, TPet *pet_novo);

int tamanho_arquivo_pet(FILE *arq);
TPet *busca_sequencial_pet(int chave, FILE *in, int *comparacoes);
TPet *busca_binaria_pet(int chave, FILE *in, int inicio, int fim, int *comparacoes);

void embaralha(int *vet, int tam);
#endif