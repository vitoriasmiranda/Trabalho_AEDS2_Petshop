#ifndef ORDENACAO_H_INCLUDED
#define ORDENACAO_H_INCLUDED

#include "petshop.h"
#include <limits.h>

void quicksort_disco(FILE *arq, int tam);

int selecao_com_substituicao(FILE *in, int M);

void intercalacao_arvore_vencedores(FILE *out, int num_particoes);

#endif 