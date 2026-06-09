#include "petshop.h"
#include <math.h>

TPet *criar_pet(int cod, char *nome, char *especie, int cod_cliente) {
    TPet *pet = (TPet *) malloc(sizeof(TPet));
    if (pet) memset(pet, 0, sizeof(TPet));
    pet->cod = cod;
    strcpy(pet->nome, nome);
    strcpy(pet->especie, especie);
    pet->cod_cliente = cod_cliente;
    return pet;
}

void salva_pet(TPet *pet, FILE *out) {
    fwrite(&pet->cod, sizeof(int), 1, out);
    fwrite(pet->nome, sizeof(char), sizeof(pet->nome), out);
    fwrite(pet->especie, sizeof(char), sizeof(pet->especie), out);
    fwrite(&pet->cod_cliente, sizeof(int), 1, out);
}

TPet *le_pet(FILE *in) {
    TPet *pet = (TPet *) malloc(sizeof(TPet));
    if (fread(&pet->cod, sizeof(int), 1, in) != 1) {
        free(pet);
        return NULL;
    }
    fread(pet->nome, sizeof(char), sizeof(pet->nome), in);
    fread(pet->especie, sizeof(char), sizeof(pet->especie), in);
    fread(&pet->cod_cliente, sizeof(int), 1, in);
    return pet;
}

void imprime_pet(TPet *pet) {
    printf("**********************************************\n");
    printf("Pet: [%d] %s (%s) | Dono: %d\n", pet->cod, pet->nome, pet->especie, pet->cod_cliente);
    printf("**********************************************\n");
}

int tamanho_registro_pet() {
    return sizeof(int) + (sizeof(char) * 50) + (sizeof(char) * 30) + sizeof(int);
}

TCliente *criar_cliente(int cod, char *nome, char *telefone) {
    TCliente *cliente = (TCliente *) malloc(sizeof(TCliente));
    if (cliente) memset(cliente, 0, sizeof(TCliente));
    cliente->cod = cod;
    strcpy(cliente->nome, nome);
    strcpy(cliente->telefone, telefone);
    return cliente;
}

void salva_cliente(TCliente *cliente, FILE *out) {
    fwrite(&cliente->cod, sizeof(int), 1, out);
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(cliente->telefone, sizeof(char), sizeof(cliente->telefone), out);
}

int tamanho_registro_cliente() {
    return sizeof(int) + (sizeof(char) * 50) + (sizeof(char) * 15);
}

void atualiza_cliente(FILE *in_out, int posicao_do_cliente, TCliente *cliente_novo) {
    fseek(in_out, (posicao_do_cliente - 1) * tamanho_registro_cliente(), SEEK_SET);
    salva_cliente(cliente_novo, in_out);
}

TServico *criar_servico(int cod, int cod_pet, char *descricao, double preco) {
    TServico *s = (TServico *) malloc(sizeof(TServico));
    if (s) memset(s, 0, sizeof(TServico));
    s->cod = cod;
    s->cod_pet = cod_pet;
    strcpy(s->descricao, descricao);
    s->preco = preco;
    return s;
}

void salva_servico(TServico *servico, FILE *out) {
    fwrite(&servico->cod, sizeof(int), 1, out);
    fwrite(&servico->cod_pet, sizeof(int), 1, out);
    fwrite(servico->descricao, sizeof(char), sizeof(servico->descricao), out);
    fwrite(&servico->preco, sizeof(double), 1, out);
}

void atualiza_pet(FILE *in_out, int posicao_do_pet, TPet *pet_novo) {
    fseek(in_out, (posicao_do_pet - 1) * tamanho_registro_pet(), SEEK_SET);
    salva_pet(pet_novo, in_out);
}

int tamanho_arquivo_pet(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    return (int) trunc(ftell(arq) / tamanho_registro_pet());
}

TPet *busca_sequencial_pet(int chave, FILE *in, int *comparacoes) {
    TPet *pet;
    *comparacoes = 0;
    rewind(in);
    while ((pet = le_pet(in)) != NULL) {
        (*comparacoes)++;
        if (pet->cod == chave) return pet;
        free(pet);
    }
    return NULL;
}

TPet *busca_binaria_pet(int chave, FILE *in, int inicio, int fim, int *comparacoes) {
    TPet *pet = NULL;
    int cod_atual = -1;
    *comparacoes = 0;
    while (inicio <= fim && cod_atual != chave) {
        (*comparacoes)++;
        int meio = (inicio + fim) / 2;
        fseek(in, (meio - 1) * tamanho_registro_pet(), SEEK_SET);
        pet = le_pet(in);
        if (pet) {
            cod_atual = pet->cod;
            if (cod_atual > chave) { fim = meio - 1; free(pet); }
            else if (cod_atual < chave) { inicio = meio + 1; free(pet); }
        }
    }
    return (cod_atual == chave) ? pet : NULL;
}

void embaralha(int *vet, int tam) {
    int tmp;
    srand(time(NULL));
    int trocas = (tam*60)/100;
    
    for (int t = 1; t<trocas; t++) {
        int i = rand() % tam;
        int j = rand() % tam;
        tmp = vet[i];
        vet[i] = vet[j];
        vet[j] = tmp;
    }
}