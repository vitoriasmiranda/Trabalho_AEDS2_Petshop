#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED
#include "petshop.h"

typedef struct {
    TPet pet;
    int prox;       
    int ocupado;    
} TPetHash;

int hash_f(int chave, int m);

void inicializa_hash(const char *arq_hash, const char *arq_dados, int m);
int insere_hash(TPet *pet, const char *arq_hash, const char *arq_dados, int m);
TPet *busca_hash(int cod, const char *arq_hash, const char *arq_dados, int m, int *comparacoes);
int remove_hash(int cod, const char *arq_hash, const char *arq_dados, int m);

#endif 