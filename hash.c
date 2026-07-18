#ifndef HASH_C
#define HASH_C
#include "hash.h"

int hash_f(int chave, int m) {
    return chave % m;
}

void inicializa_hash(const char *arq_hash, const char *arq_dados, int m) {
    
    FILE *fh = fopen(arq_hash, "wb");
    if (!fh) {
        printf("Erro ao inicializar arquivo de índices (hash).\n");
        return;
    }
    int vazio = -1;
    for (int i = 0; i < m; i++) {
        fwrite(&vazio, sizeof(int), 1, fh);
    }
    fclose(fh);

    FILE *fd = fopen(arq_dados, "wb");
    if (!fd) {
        printf("Erro ao inicializar arquivo de dados.\n");
        return;
    }
    int topo_livre = -1; 
    fwrite(&topo_livre, sizeof(int), 1, fd);
    fclose(fd);
}

int insere_hash(TPet *pet, const char *arq_hash, const char *arq_dados, int m) {
    int end_hash = hash_f(pet->cod, m);
    FILE *fh = fopen(arq_hash, "r+b");
    FILE *fd = fopen(arq_dados, "r+b");

    if (!fh || !fd) {
    if (fh) fclose(fh);
    if (fd) fclose(fd);
    return 0;
    }

    int pos_inicio;
    fseek(fh, end_hash * sizeof(int), SEEK_SET);
    fread(&pos_inicio, sizeof(int), 1, fh);

    int pos_atual = pos_inicio;
    TPetHash no;
    while (pos_atual != -1) {
        fseek(fd, sizeof(int) + pos_atual * sizeof(TPetHash), SEEK_SET);
        fread(&no, sizeof(TPetHash), 1, fd);
        if (no.ocupado == 1 && no.pet.cod == pet->cod) {
            fclose(fh); fclose(fd);
            return 0; 
        }
        pos_atual = no.prox;
    }

    int topo_livre;
    fseek(fd, 0, SEEK_SET);
    fread(&topo_livre, sizeof(int), 1, fd);

    int pos_nova;
    if (topo_livre != -1) {
        pos_nova = topo_livre;
        fseek(fd, sizeof(int) + pos_nova * sizeof(TPetHash), SEEK_SET);
        fread(&no, sizeof(TPetHash), 1, fd);
        
        topo_livre = no.prox; 
        fseek(fd, 0, SEEK_SET);
        fwrite(&topo_livre, sizeof(int), 1, fd);

        printf("    [LOG DISCO] Pet [%d] %s inserido na posicao reaproveitada: %d\n", pet->cod, pet->nome, pos_nova);
        
    } else {
        fseek(fd, 0, SEEK_END);
        pos_nova = (ftell(fd) - sizeof(int)) / sizeof(TPetHash);
        printf("    [LOG DISCO] Pet [%d] %s inserido em nova posicao no disco: %d\n", pet->cod, pet->nome, pos_nova);
    }

    no.pet = *pet;
    no.ocupado = 1; 
    no.prox = pos_inicio; 

    fseek(fd, sizeof(int) + pos_nova * sizeof(TPetHash), SEEK_SET);
    fwrite(&no, sizeof(TPetHash), 1, fd);
    fseek(fh, end_hash * sizeof(int), SEEK_SET);
    fwrite(&pos_nova, sizeof(int), 1, fh);

    fclose(fh); fclose(fd);
    return 1;
}

TPet *busca_hash(int cod, const char *arq_hash, const char *arq_dados, int m, int *comparacoes) {
    *comparacoes = 0;
    int end_hash = hash_f(cod, m);
    FILE *fh = fopen(arq_hash, "rb");
    FILE *fd = fopen(arq_dados, "rb");

    if (!fh || !fd) {
    if (fh) fclose(fh);
    if (fd) fclose(fd);
    return 0;
    }

    int pos_atual;
    fseek(fh, end_hash * sizeof(int), SEEK_SET);
    fread(&pos_atual, sizeof(int), 1, fh);

    TPetHash no;
    while (pos_atual != -1) {
        (*comparacoes)++;
        fseek(fd, sizeof(int) + pos_atual * sizeof(TPetHash), SEEK_SET);
        fread(&no, sizeof(TPetHash), 1, fd);
        
        if (no.ocupado == 1 && no.pet.cod == cod) {
            TPet *encontrado = (TPet *)malloc(sizeof(TPet));
            *encontrado = no.pet;
            fclose(fh); fclose(fd);
            return encontrado;
        }
        pos_atual = no.prox;
    }

    fclose(fh); fclose(fd);
    return NULL; 
}

int remove_hash(int cod, const char *arq_hash, const char *arq_dados, int m) {
    int end_hash = hash_f(cod, m);
    FILE *fh = fopen(arq_hash, "r+b");
    FILE *fd = fopen(arq_dados, "r+b");

    if (!fh || !fd) {
    if (fh) fclose(fh);
    if (fd) fclose(fd);
    return 0;
    }

    int pos_atual;
    fseek(fh, end_hash * sizeof(int), SEEK_SET);
    fread(&pos_atual, sizeof(int), 1, fh);

    int pos_anterior = -1;
    TPetHash no, no_ant;

    while (pos_atual != -1) {
        fseek(fd, sizeof(int) + pos_atual * sizeof(TPetHash), SEEK_SET);
        fread(&no, sizeof(TPetHash), 1, fd);

        if (no.ocupado == 1 && no.pet.cod == cod) {
            if (pos_anterior == -1) {
                fseek(fh, end_hash * sizeof(int), SEEK_SET);
                fwrite(&no.prox, sizeof(int), 1, fh);
            } else { 
                no_ant.prox = no.prox;
                fseek(fd, sizeof(int) + pos_anterior * sizeof(TPetHash), SEEK_SET);
                fwrite(&no_ant, sizeof(TPetHash), 1, fd);
            }

            int topo_livre;
            fseek(fd, 0, SEEK_SET);
            fread(&topo_livre, sizeof(int), 1, fd);

            no.ocupado = 0; 
            no.prox = topo_livre;
            fseek(fd, sizeof(int) + pos_atual * sizeof(TPetHash), SEEK_SET);
            fwrite(&no, sizeof(TPetHash), 1, fd);

            topo_livre = pos_atual;
            fseek(fd, 0, SEEK_SET);
            fwrite(&topo_livre, sizeof(int), 1, fd);

            printf("    [LOG DISCO] Pet [%d] %s removido. Posicao %d liberada para reuso.\n", no.pet.cod, no.pet.nome, pos_atual);

            fclose(fh); fclose(fd);
            return 1; 
        }
        pos_anterior = pos_atual;
        no_ant = no;
        pos_atual = no.prox;
    }

    fclose(fh); fclose(fd);
    return 0; 
}

#endif