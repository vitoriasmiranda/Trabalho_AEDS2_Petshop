#include "ordenacao.h"

#ifndef ORDENACAO_C
#define ORDENACAO_C

void quicksort_disco_recursivo(FILE *arq, int esq, int dir) {
    if (esq >= dir) return;

    int i = esq;
    int j = dir;
    int meio = (esq + dir) / 2;

    fseek(arq, (meio - 1) * tamanho_registro_pet(), SEEK_SET);
    TPet *pivo = le_pet(arq);
    int pivo_cod = pivo->cod;
    free(pivo);

    while (i <= j) {
        TPet *pi = NULL;
        fseek(arq, (i - 1) * tamanho_registro_pet(), SEEK_SET);
        pi = le_pet(arq);
        while (pi != NULL && pi->cod < pivo_cod && i < dir) {
            free(pi);
            i++;
            fseek(arq, (i - 1) * tamanho_registro_pet(), SEEK_SET);
            pi = le_pet(arq);
        }

        TPet *pj = NULL;
        fseek(arq, (j - 1) * tamanho_registro_pet(), SEEK_SET);
        pj = le_pet(arq);
        while (pj != NULL && pj->cod > pivo_cod && j > esq) {
            free(pj);
            j--;
            fseek(arq, (j - 1) * tamanho_registro_pet(), SEEK_SET);
            pj = le_pet(arq);
        }

        if (i <= j) {
            fseek(arq, (i - 1) * tamanho_registro_pet(), SEEK_SET);
            salva_pet(pj, arq);
            fseek(arq, (j - 1) * tamanho_registro_pet(), SEEK_SET);
            salva_pet(pi, arq);
            i++;
            j--;
        }
        
        if (pi) { free(pi); pi = NULL; }
        if (pj) { free(pj); pj = NULL; }
    }

    if (esq < j) quicksort_disco_recursivo(arq, esq, j);
    if (i < dir) quicksort_disco_recursivo(arq, i, dir);
}

void quicksort_disco(FILE *arq, int tam) {
    if (tam > 1) {
        quicksort_disco_recursivo(arq, 1, tam);
        fflush(arq);
    }
}

int selecao_com_substituicao(FILE *in, int M) {
    rewind(in);
    TPet *mem[M];
    int congelado[M];
    
    int elementos_memoria = 0; 

    for (int i = 0; i < M; i++) {
        mem[i] = le_pet(in);
        congelado[i] = 0;
        if (mem[i]) elementos_memoria++;
    }

    if (elementos_memoria == 0) return 0;

    int num_particoes = 0;
    char nomeParticao[30];

    while (elementos_memoria > 0) {
        sprintf(nomeParticao, "particao_%d.dat", num_particoes);
        FILE *out = fopen(nomeParticao, "wb");
        int ultimo_gravado = -1;

        while (1) {
            int menor_idx = -1;
            int menor_cod = INT_MAX;

            for (int i = 0; i < M; i++) {
                if (mem[i] != NULL && !congelado[i] && mem[i]->cod < menor_cod) {
                    menor_cod = mem[i]->cod;
                    menor_idx = i;
                }
            }

            if (menor_idx == -1) break;

            salva_pet(mem[menor_idx], out);
            ultimo_gravado = mem[menor_idx]->cod;
            free(mem[menor_idx]);
            mem[menor_idx] = NULL;
            elementos_memoria--; 

            TPet *novo = le_pet(in);
            if (novo) {
                mem[menor_idx] = novo;
                elementos_memoria++; 
                if (novo->cod < ultimo_gravado) {
                    congelado[menor_idx] = 1;
                }
            }
        }

        fclose(out);
        num_particoes++;

        for (int i = 0; i < M; i++) {
            congelado[i] = 0;
        }
    }
    
    return num_particoes;
}

int get_cod(TPet *p) { 
    return p ? p->cod : INT_MAX; 
}

void intercalacao_arvore_vencedores(FILE *out, int num_particoes) {
    if (num_particoes <= 0) return;

    int tam_folhas = 1;
    while (tam_folhas < num_particoes) tam_folhas *= 2;

    FILE *arquivos[tam_folhas];
    TPet *folhas[tam_folhas];
    int arvore[tam_folhas * 2]; 

    for (int i = 0; i < tam_folhas; i++) {
        arquivos[i] = NULL; 
        folhas[i] = NULL;
        
        if (i < num_particoes) {
            char nomeParticao[30];
            sprintf(nomeParticao, "particao_%d.dat", i);
            arquivos[i] = fopen(nomeParticao, "rb");
            if (arquivos[i]) {
                folhas[i] = le_pet(arquivos[i]);
            }
        }
    }

    for (int i = 0; i < tam_folhas; i++) {
        arvore[tam_folhas + i] = i; 
    }
    
    for (int i = tam_folhas - 1; i > 0; i--) {
        int esq = arvore[2 * i];
        int dir = arvore[2 * i + 1];
        arvore[i] = (get_cod(folhas[esq]) <= get_cod(folhas[dir])) ? esq : dir;
    }

    while (1) {
        int vencedor = arvore[1]; 
        TPet *pet_vencedor = folhas[vencedor];

        if (!pet_vencedor) break; 

        salva_pet(pet_vencedor, out);
        free(pet_vencedor);

        folhas[vencedor] = le_pet(arquivos[vencedor]);

        int pos = tam_folhas + vencedor;
        while (pos > 1) {
            pos /= 2; 
            int esq = arvore[2 * pos];
            int dir = arvore[2 * pos + 1];
            arvore[pos] = (get_cod(folhas[esq]) <= get_cod(folhas[dir])) ? esq : dir;
        }
    }

    for (int i = 0; i < num_particoes; i++) {
        if (arquivos[i]) fclose(arquivos[i]);
        char nomeParticao[30];
        sprintf(nomeParticao, "particao_%d.dat", i);
        remove(nomeParticao); 
    }
}

#endif