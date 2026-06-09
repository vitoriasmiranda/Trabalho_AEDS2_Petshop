#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "petshop.c"

void gerar_base_teste(int tamanho, const char *nome_arquivo, int ordenado) {
    FILE *arq = fopen(nome_arquivo, "w+b");
    if (!arq) return;

    if (ordenado == 1) {
        for (int i = 1; i <= tamanho; i++) {
            TPet *p = criar_pet(i, "Pet Teste", "Especie", 101);
            salva_pet(p, arq);
            free(p);
        }
    } else {
        int vet[tamanho]; 
        for(int i = 0; i < tamanho; i++) {
            vet[i] = i + 1;
        }
        embaralha(vet, tamanho); 
        for (int i = 0; i < tamanho; i++) {
            TPet *p = criar_pet(vet[i], "Pet Teste", "Especie", 101);
            salva_pet(p, arq);
            free(p);
        }
    }
    fclose(arq);
}

int main() {
    FILE *arq_clientes, *arq_pets, *arq_servicos;
    TCliente *c;
    TServico *s;
    
    int comparacoes;
    clock_t inicio_tempo, fim_tempo;
    double tempo_gasto;

    printf("====================================================\n");
    printf("        SISTEMA DE PETSHOP \n");
    printf("====================================================\n\n");

    printf("Interacoes entre Entidades...\n");
    arq_clientes = fopen("clientes_demo.dat", "w+b");
    arq_pets = fopen("pets_demo.dat", "w+b");
    arq_servicos = fopen("servicos_demo.dat", "w+b");

    c = criar_cliente(101, "Vitoria", "31-9999-0001");
    salva_cliente(c, arq_clientes);
    free(c);

    TPet *p = criar_pet(1, "Zeus", "Cachorro", 101);
    salva_pet(p, arq_pets);
    free(p);
    
    TCliente *cliente_upd = criar_cliente(101, "Vitoria Miranda", "31-9888-7777");
    atualiza_cliente(arq_clientes, 1, cliente_upd);
    free(cliente_upd);

    printf("    -> Usando Busca Binaria para verificar o Pet antes de atualizar...\n");
    int comparacoes_bin_interacao = 0;
    
    int total_pets = tamanho_arquivo_pet(arq_pets); 
    
    TPet *pet_para_atualizar = busca_binaria_pet(1, arq_pets, 1, total_pets, &comparacoes_bin_interacao);

    if (pet_para_atualizar != NULL) {
        printf("    Pet encontrado na Busca Binaria: %s. Procedendo com atualizacao...\n", pet_para_atualizar->nome);
        TPet *pet_upd = criar_pet(1, "Zeus O Grande", "Cachorro", 101);
        atualiza_pet(arq_pets, 1, pet_upd);
        free(pet_upd);
        free(pet_para_atualizar);
    } else {
        printf("    Pet nao encontrado para atualizacao.\n");
    }

    printf("    -> Buscando Pet para cadastrar servico...\n");
    int comparacoes_interacao = 0;
    
    rewind(arq_pets); 
    
    TPet *pet_encontrado = busca_sequencial_pet(1, arq_pets, &comparacoes_interacao);

    if (pet_encontrado != NULL) {
        printf("    Pet encontrado! Ficha do animal:\n");
        imprime_pet(pet_encontrado); 
        printf("    Registrando Banho...\n");
        s = criar_servico(1, pet_encontrado->cod, "Banho", 50.0);
        salva_servico(s, arq_servicos);
        free(s);
        free(pet_encontrado);
    } else {
        printf("    Pet nao encontrado. Servico cancelado.\n");
    }

    fclose(arq_clientes);
    fclose(arq_pets);
    fclose(arq_servicos);
    printf("    -> Interacoes, atualizacoes e vinculos realizados!\n\n");

    printf("Bateria de Buscas, Tamanhos e Log...\n");
    
    FILE *log_file = fopen("log.txt", "w");
    if (!log_file) {
        printf("Erro ao criar arquivo de log!\n");
        return 1;
    }

    fprintf(log_file, "================ RELATORIO DE BUSCAS =================\n");

    int tamanhos[] = {100, 1000, 10000};
    int num_tamanhos = 3;

    for (int i = 0; i < num_tamanhos; i++) {
        int tam = tamanhos[i];
        printf("    -> Testando com base de %d registros...\n", tam);
        fprintf(log_file, "\n--- BASE DE TAMANHO: %d REGISTROS ---\n", tam);

        int chave_busca = tam+5; 

        gerar_base_teste(tam, "pets_desordenado.dat", 0);
        FILE *arq_desord = fopen("pets_desordenado.dat", "rb");
        
        inicio_tempo = clock();
        TPet *res_seq = busca_sequencial_pet(chave_busca, arq_desord, &comparacoes);
        fim_tempo = clock();
        tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC * 1000;
        
        fprintf(log_file, "BUSCA SEQUENCIAL (Base Desordenada):\n");
        fprintf(log_file, "Chave buscada: %d | Comparacoes: %d | Tempo: %.4f ms\n", chave_busca, comparacoes, tempo_gasto);
        
        if (res_seq) free(res_seq);
        fclose(arq_desord);

        gerar_base_teste(tam, "pets_ordenado.dat", 1);
        FILE *arq_ord = fopen("pets_ordenado.dat", "rb");
        
        inicio_tempo = clock();
        TPet *res_bin = busca_binaria_pet(chave_busca, arq_ord, 1, tam, &comparacoes);
        fim_tempo = clock();
        tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC * 1000;

        fprintf(log_file, "\nBUSCA BINARIA (Base Ordenada):\n");
        fprintf(log_file, "Chave buscada: %d | Comparacoes: %d | Tempo: %.4f ms\n", chave_busca, comparacoes, tempo_gasto);
        
        if (res_bin) free(res_bin);
        fclose(arq_ord);
    }

    fclose(log_file);
    printf("\n====================================================\n");
    printf(" TESTES CONCLUIDOS! Arquivo 'log.txt' gerado.\n");
    printf("====================================================\n");

    return 0;
}