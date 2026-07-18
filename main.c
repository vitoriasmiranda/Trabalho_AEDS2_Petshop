#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "petshop.c" 
#include "petshop.h" 
#include "ordenacao.c"
#include "hash.c"

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

int tamanhos_p2[] = {1000, 10000, 100000, 500000};
    int num_tamanhos_qs = 2; 
    int num_tamanhos_ext = 4;
    
    FILE *log_file_p2 = fopen("log_ordenacao.txt", "w");
    if (!log_file_p2) {
        printf("Erro ao criar arquivo de log da Parte 2!\n");
        return 1;
    }
    fprintf(log_file_p2, "============= RELATORIO DE ORDENACAO =============\n\n");

    printf("--- PARTE 2: QUICKSORT NO DISCO ---\n");
    printf("(Limitado a 100.000 registros para evitar travamento)\n");
    
    for (int i = 0; i < num_tamanhos_qs; i++) { 
        int tam = tamanhos_p2[i];
        printf("    -> Ordenando %d registros...\n", tam);
        
        gerar_base_teste(tam, "pets_desord_qs.dat", 0);
        FILE *arq = fopen("pets_desord_qs.dat", "r+b");
        
        if (arq) {
            inicio_tempo = clock();
            quicksort_disco(arq, tam);
            fim_tempo = clock();
            
            tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;
            fprintf(log_file_p2, "[QuickSort Externo] Tamanho: %6d | Tempo: %.4f s\n", tam, tempo_gasto);
            printf("       Tempo: %.4f segundos\n", tempo_gasto);
            fclose(arq);
            remove("pets_desord_qs.dat"); 
        }
    }
    printf("\n");

    printf("--- PARTE 2: CLASSIFICACAO EXTERNA ---\n");
    int M = 1000; 

    for (int i = 0; i < num_tamanhos_ext; i++) {
        int tam = tamanhos_p2[i];
        printf("    -> Ordenando %d registros (Substituicao + Arvore)...\n", tam);
        
        gerar_base_teste(tam, "pets_desord_ext.dat", 0);
        FILE *arq_in = fopen("pets_desord_ext.dat", "rb");
        
        if (arq_in) {
            inicio_tempo = clock();
            
            int particoes = selecao_com_substituicao(arq_in, M);
            fclose(arq_in);

            FILE *arq_out = fopen("pets_ord_ext.dat", "w+b");
            if (arq_out) {
                intercalacao_arvore_vencedores(arq_out, particoes);
                fclose(arq_out);
            }

            fim_tempo = clock();
            
            tempo_gasto = ((double)(fim_tempo - inicio_tempo)) / CLOCKS_PER_SEC;
            fprintf(log_file_p2, "[Classif. Externa]  Tamanho: %6d | Particoes: %2d | Tempo: %.4f s\n", tam, particoes, tempo_gasto);
            printf("       Particoes: %d | Tempo total: %.4f segundos\n", particoes, tempo_gasto);
        }
    }

    fclose(log_file_p2);

    printf("\n====================================================\n");
    printf(" TRABALHO CONCLUIDO COM SUCESSO!\n");
    printf(" Logs gerados: 'log.txt' e 'log_ordenacao.txt'\n");
    printf("====================================================\n");

    printf("\n\n====================================================\n");
    printf("        PARTE III: TESTANDO TABELA HASH \n");
    printf("====================================================\n");
    
    int m = 7; 
    const char *arq_tabHash = "tabHash.dat";
    const char *arq_dadosHash = "pets_hash_dados.dat";
    
    inicializa_hash(arq_tabHash, arq_dadosHash, m);

    TPet *p1 = criar_pet(10, "Rex", "Cachorro", 101);
    TPet *p2 = criar_pet(17, "Mia", "Gato", 102); 
    TPet *p3 = criar_pet(24, "Loro", "Papagaio", 103); 
    
    printf("\n-> [1] Inserindo Pets iniciais...\n");
    insere_hash(p1, arq_tabHash, arq_dadosHash, m);
    insere_hash(p2, arq_tabHash, arq_dadosHash, m);
    insere_hash(p3, arq_tabHash, arq_dadosHash, m);
    printf("Pets inseridos com sucesso!\n");

    printf("\n-> [2] Teste de Seguranca: Insercao Duplicada...\n");
    printf("Tentando inserir a chave 10 (Rex) novamente...\n");
    if (!insere_hash(p1, arq_tabHash, arq_dadosHash, m)) {
        printf("SUCESSO: Chave duplicada rejeitada corretamente pelo sistema!\n");
    }

    int comp_hash = 0;
    TPet *pet_buscado = busca_hash(17, arq_tabHash, arq_dadosHash, m, &comp_hash);
    if(pet_buscado) {
        printf("\n-> [3] Busca (Chave 17): Encontrado '%s' em %d comparacoes.\n", pet_buscado->nome, comp_hash);
        free(pet_buscado);
    }

    printf("\n-> [4] Teste de Seguranca: Remover chave inexistente...\n");
    printf("Tentando remover a chave 999...\n");
    if (!remove_hash(999, arq_tabHash, arq_dadosHash, m)) {
        printf("SUCESSO: Remocao falhou graciosamente (Chave nao existe)!\n");
    }

    printf("\n-> [5] Removendo chave 17...\n\n");
    if (remove_hash(17, arq_tabHash, arq_dadosHash, m)) {
        printf("Chave 17 removida. Espaco entrou na Pilha de Reaproveitamento.\n");
    }

    pet_buscado = busca_hash(17, arq_tabHash, arq_dadosHash, m, &comp_hash);
    if(!pet_buscado) {
        printf("-> Busca pos-remocao (Chave 17): Nao encontrado!\n");
    }

    TPet *p4 = criar_pet(30, "Bolinha", "Hamster", 104);
    printf("\n-> [6] Gerenciamento de Espaco: Inserindo chave 30 (Bolinha)...\n");
    printf("Ele deve ocupar EXATAMENTE o espaco reaproveitado da chave 17.\n\n");
    insere_hash(p4, arq_tabHash, arq_dadosHash, m);

    free(p1); free(p2); free(p3); free(p4);

    return 0;
}