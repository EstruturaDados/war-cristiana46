/**
 * @file cadastro_territorios_v2.c
 * @brief Programa para cadastrar e exibir informações de territórios, usando scanf.
 *
 * Implementa uma struct 'Territorio' e utiliza um vetor para armazenar
 * 5 instâncias. O código utiliza scanf para todas as entradas e inclui
 * limpeza de buffer para evitar problemas de leitura.
 */

#include <stdio.h>
#include <string.h>

// Definindo a constante para o número de territórios (melhora a manutenibilidade)
#define MAX_TERRITORIOS 5

/**
 * @struct Territorio
 * @brief Estrutura que armazena as informações de um território.
 *
 * Campos:
 * - nome: Nome do território (máx. 29 caracteres).
 * - cor: Cor do exército que o ocupa (máx. 9 caracteres).
 * - tropas: Quantidade de tropas (número inteiro).
 */
typedef struct {
    char nome[30];  // Campo para o nome
    char cor[10];   // Campo para a cor do exército
    int tropas;     // Campo para a quantidade de tropas
} Territorio;

/**
 * @brief Função principal do programa.
 */
int main() {
    // Declaração do vetor de structs:
    // Cria um vetor com capacidade para armazenar 5 estruturas do tipo Territorio.
    Territorio lista_territorios[MAX_TERRITORIOS];
    int i; // Variável de controle para os laços 'for'

    printf("===========================================\n");
    printf("   SISTEMA DE CADASTRO DE TERRITÓRIOS\n");
    printf("===========================================\n");
    printf("Preencha os dados de %d territórios.\n\n", MAX_TERRITORIOS);

    // Lógica do laço de entrada de dados:
    // O laço 'for' itera MAX_TERRITORIOS vezes, permitindo o cadastro de cada território.
    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("--- Território %d de %d ---\n", i + 1, MAX_TERRITORIOS);

        // 1. Leitura do NOME
        // Uso de scanf com especificação de largura: "%29s" lê no máximo 29 caracteres, 
        // evitando buffer overflow (requisito de manutenibilidade).
        printf("Digite o NOME do território (sem espaços, máx. 29 caracteres): ");
        if (scanf("%29s", lista_territorios[i].nome) != 1) {
             printf("Erro na leitura do nome. Encerrando.\n");
             return 1;
        }

        // 2. Leitura da COR
        // Uso de scanf com especificação de largura: "%9s" lê no máximo 9 caracteres.
        printf("Digite a COR do exército (sem espaços, máx. 9 caracteres): ");
        if (scanf("%9s", lista_territorios[i].cor) != 1) {
             printf("Erro na leitura da cor. Encerrando.\n");
             return 1;
        }

        // 3. Leitura das TROPAS
        // Uso de scanf para ler o número inteiro.
        printf("Digite a QUANTIDADE de tropas (número inteiro): ");
        if (scanf("%d", &lista_territorios[i].tropas) != 1) {
             printf("Erro na leitura das tropas. Encerrando.\n");
             return 1;
        }

        // Limpeza do buffer de entrada após a leitura numérica:
        // ESSENCIAL: Se não for feita, o '\n' (Enter) deixado no buffer após o scanf("%d")
        // seria lido pelo próximo scanf("%s") ou fgets, causando um comportamento incorreto.
        while (getchar() != '\n'); 
        
        printf("\n");
    }

    // Exibição de dados (Saída):
    // Percorre o vetor de structs e imprime os dados de forma clara (requisito de usabilidade).
    printf("===========================================\n");
    printf("   DADOS DOS TERRITÓRIOS CADASTRADOS\n");
    printf("===========================================\n");

    for (i = 0; i < MAX_TERRITORIOS; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", lista_territorios[i].nome);
        printf("  Cor do Exército: %s\n", lista_territorios[i].cor);
        printf("  Tropas: %d\n", lista_territorios[i].tropas);
        printf("-------------------------------------------\n");
    }

    printf("Cadastro e exibição concluídos com sucesso.\n");

    return 0;
}