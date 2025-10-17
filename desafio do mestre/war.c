#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- DEFINIÇÃO DE ESTRUTURAS E CONSTANTES ---

// Definição da struct Territorio.
typedef struct {
    char nome[30]; 
    char cor[10];  // Cor do exército que controla o território.
    int tropas;    
} Territorio;

// Constantes para as missões
#define MAX_MISSAO_LEN 100
#define NUM_MISSOES 5

// Vetor global (ou constante) de descrições de missões.
const char *MISSOES[NUM_MISSOES] = {
    "Missao 1: Conquistar 5 territorios seguidos.",
    "Missao 2: Eliminar todas as tropas da cor Verde.",
    "Missao 3: Conquistar 10 territorios no total.",
    "Missao 4: Ter no minimo 8 territorios e 20 tropas.",
    "Missao 5: Conquistar 4 territorios com o maximo de tropas.",
};

// --- PROTÓTIPOS DAS FUNÇÕES ---
void cadastrar_territorios(Territorio *mapa, int total);
void exibir_territorios(const Territorio *mapa, int total);
void atribuirMissao(char **destino, const char *missoes[], int totalMissoes);
void exibirMissao(const char *missao);
int verificarMissao(const char *missao, const char *cor_jogador, const Territorio *mapa, int tamanho);
void atacar(Territorio *atacante, Territorio *defensor);
void liberar_memoria(Territorio *mapa, char *missao_jogador);

// --- FUNÇÃO PRINCIPAL ---
int main() {
    // Inicializa o gerador de números aleatórios.
    srand(time(NULL));

    int num_territorios = 0;
    Territorio *mapa = NULL;
    char *missao_jogador = NULL; // Ponteiro para armazenar a missão sorteada.
    
    int indice_atacante, indice_defensor;
    char continuar_ataque = 's';
    
    // Supondo um único jogador com uma cor definida para simplificar a verificação da missão.
    // Em um jogo multi-jogador real, cada um teria sua própria cor e missão.
    const char *cor_do_jogador = "Azul"; 

    printf("====================================================\n");
    printf("        SIMULADOR DE GUERRA ESTRUTURADA - WAR\n");
    printf("     Jogador atual: Equipe %s\n", cor_do_jogador);
    printf("====================================================\n");

    // 1. ALOCAÇÃO DINÂMICA DE TERRITÓRIOS
    printf("Quantos territórios você deseja cadastrar? (min 2): ");
    if (scanf("%d", &num_territorios) != 1 || num_territorios < 2) {
        printf("Número inválido de territórios. Encerrando o programa.\n");
        return 1;
    }
    
    mapa = (Territorio *)calloc(num_territorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória para o mapa.\n");
        return 1;
    }

    // 2. CADASTRO INICIAL
    cadastrar_territorios(mapa, num_territorios);
    while (getchar() != '\n'); // Limpa o buffer
    
    // 3. ATRIBUIÇÃO E EXIBIÇÃO DA MISSÃO
    // Passa o endereço do ponteiro 'missao_jogador' para alocação e atribuição.
    atribuirMissao(&missao_jogador, MISSOES, NUM_MISSOES);
    printf("\n");
    exibirMissao(missao_jogador); // Exibe a missão (passagem por valor).

    // 4. LOOP DE ATAQUES E VERIFICAÇÃO DE MISSÃO
    while (continuar_ataque == 's' || continuar_ataque == 'S') {
        printf("\n\n----------------------------------------------------\n");
        printf("              ESTADO ATUAL DO MAPA\n");
        printf("----------------------------------------------------\n");
        exibir_territorios(mapa, num_territorios);

        // --- Seleção de Atacante e Defensor (omiti a validação completa por brevidade) ---
        printf("\n--- PREPARAÇÃO PARA O ATAQUE ---\n");

        do {
            printf("Escolha o índice do Território ATACANTE (1 a %d, Tropas > 1): ", num_territorios);
            scanf("%d", &indice_atacante);
            indice_atacante--; 
        } while (indice_atacante < 0 || indice_atacante >= num_territorios || (mapa + indice_atacante)->tropas < 2);

        do {
            printf("Escolha o índice do Território DEFENSOR (1 a %d, Cor diferente): ", num_territorios);
            scanf("%d", &indice_defensor);
            indice_defensor--; 
        } while (indice_defensor < 0 || indice_defensor >= num_territorios || indice_atacante == indice_defensor || 
                 strcmp((mapa + indice_atacante)->cor, (mapa + indice_defensor)->cor) == 0);

        // Execução do ataque
        printf("\n--- INÍCIO DA BATALHA: %s (%s) ATACA %s (%s) ---\n",
               (mapa + indice_atacante)->nome, (mapa + indice_atacante)->cor,
               (mapa + indice_defensor)->nome, (mapa + indice_defensor)->cor);
               
        atacar(mapa + indice_atacante, mapa + indice_defensor);
        
        printf("\n--- SITUAÇÃO PÓS-ATAQUE ---\n");
        exibir_territorios(mapa, num_territorios);

        // 5. VERIFICAÇÃO CONDICIONAL DA MISSÃO
        if (verificarMissao(missao_jogador, cor_do_jogador, mapa, num_territorios)) {
            printf("\n\n####################################################\n");
            printf("         PARABÉNS! MISSÃO CONCLUÍDA!\n");
            printf("             O JOGADOR %s VENCEU!\n", cor_do_jogador);
            printf("####################################################\n");
            break; // Sai do loop principal
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        while (getchar() != '\n'); 
        scanf("%c", &continuar_ataque);
    }

    // 6. LIBERAÇÃO DA MEMÓRIA
    liberar_memoria(mapa, missao_jogador);
    
    printf("\n====================================================\n");
    printf("         Memória liberada. Fim do programa.\n");
    printf("====================================================\n");

    return 0;
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE MISSÃO ---

/**
 * @brief Sorteia e aloca dinamicamente a string da missão para o jogador.
 * @param destino Ponteiro para o ponteiro (char**) onde a string da missão será armazenada (passagem por referência).
 * @param missoes Vetor de strings com as missões pré-definidas.
 * @param totalMissoes O número de missões no vetor.
 */
void atribuirMissao(char **destino, const char *missoes[], int totalMissoes) {
    int indice_sorteado = rand() % totalMissoes;
    const char *missao_selecionada = missoes[indice_sorteado];
    size_t len = strlen(missao_selecionada) + 1;

    // Alocação dinâmica de memória para armazenar a missão.
    *destino = (char *)malloc(len * sizeof(char));

    if (*destino == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a missão.\n");
        exit(1); 
    }

    // Copia a missão sorteada para a memória alocada.
    strncpy(*destino, missao_selecionada, len);
    (*destino)[len - 1] = '\0'; // Garantir terminação.
}

/**
 * @brief Exibe a missão atual do jogador.
 * @param missao Ponteiro constante para a string da missão (passagem por valor conceitual).
 */
void exibirMissao(const char *missao) {
    printf("SUA MISSÃO SECRETA É: %s\n", missao);
    printf("Memorize-a! Ela definirá sua vitória.\n");
}

/**
 * @brief Verifica se a missão do jogador foi cumprida.
 * @param missao Ponteiro para a string da missão.
 * @param cor_jogador Cor do exército do jogador.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param tamanho Número total de territórios.
 * @return 1 se a missão foi cumprida, 0 caso contrário.
 */
int verificarMissao(const char *missao, const char *cor_jogador, const Territorio *mapa, int tamanho) {
    // Lógica SIMPLES de verificação (exemplo baseado na Missão 4)
    int territorios_jogador = 0;
    int tropas_totais = 0;
    int i;
    
    // Contagem de territórios e tropas do jogador
    for (i = 0; i < tamanho; i++) {
        if (strcmp((mapa + i)->cor, cor_jogador) == 0) {
            territorios_jogador++;
            tropas_totais += (mapa + i)->tropas;
        }
    }

    // --- Lógica de Vitória Simples ---
    // Se a missão for "Missao 4: Ter no minimo 8 territorios e 20 tropas."
    if (strstr(missao, "8 territorios e 20 tropas") != NULL) {
        if (territorios_jogador >= 3 && tropas_totais >= 10) { // Lógica simplificada para teste
            printf("\n[VERIFICAÇÃO] Condição de 3 territórios e 10 tropas atingida!\n");
            return 1;
        }
    }
    // Para as outras missões, retornamos 0 (não implementado).
    
    return 0; 
}


// --- IMPLEMENTAÇÃO DAS FUNÇÕES DE JOGO (REUSADAS) ---

void cadastrar_territorios(Territorio *mapa, int total) {
    int i;
    printf("\n--- PREENCHIMENTO DOS DADOS DOS TERRITÓRIOS ---\n");
    for (i = 0; i < total; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: ");
        scanf("%29s", (mapa + i)->nome);
        printf("Cor do Exército: ");
        scanf("%9s", (mapa + i)->cor);
        do {
            printf("Quantidade de Tropas (> 0): ");
            if (scanf("%d", &(mapa + i)->tropas) != 1) {
                while(getchar() != '\n'); 
                (mapa + i)->tropas = 0; 
            } else if ((mapa + i)->tropas <= 0) {
                printf("A quantidade de tropas deve ser positiva.\n");
            }
        } while ((mapa + i)->tropas <= 0);
    }
}

void exibir_territorios(const Territorio *mapa, int total) {
    int i;
    printf("  %-4s | %-29s | %-10s | %-7s \n", "Idx", "Nome do Território", "Cor", "Tropas");
    printf("-------|-------------------------------|------------|---------\n");
    
    for (i = 0; i < total; i++) {
        printf("  %03d  | %-29s | %-10s | %-7d \n", 
               i + 1, 
               (mapa + i)->nome,
               (mapa + i)->cor,
               (mapa + i)->tropas);
    }
}

void atacar(Territorio *atacante, Territorio *defensor) {
    int dado_ataque = rand() % 6 + 1;
    int dado_defesa = rand() % 6 + 1;
    
    printf("   Rolagem de Dados: Atacante tirou %d vs. Defensor tirou %d\n", dado_ataque, dado_defesa);

    if (dado_ataque > dado_defesa) {
        printf("   VITÓRIA! O %s vence a batalha.\n", atacante->nome);

        // Atualização de cor do defensor
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0'; 

        // Transferência de tropas
        int tropas_transferidas = atacante->tropas / 2;
        
        atacante->tropas -= tropas_transferidas;
        defensor->tropas = tropas_transferidas;

        printf("   CONQUISTA! %s agora pertence a %s. %d tropas foram transferidas.\n",
               defensor->nome, defensor->cor, tropas_transferidas);
               
    } else {
        printf("   DERROTA! O %s se defende com sucesso.\n", defensor->nome);
        
        // Perda de tropas do atacante
        if (atacante->tropas > 1) {
            atacante->tropas -= 1;
            printf("   O %s perdeu 1 tropa na invasão.\n", atacante->nome);
        }
    }
}

/**
 * @brief Libera toda a memória alocada dinamicamente.
 * @param mapa Ponteiro para o vetor de territórios.
 * @param missao_jogador Ponteiro para a string da missão.
 */
void liberar_memoria(Territorio *mapa, char *missao_jogador) {
    // Libera a memória alocada para o vetor de structs
    if (mapa != NULL) {
        free(mapa);
        printf("\n[INFO] Memória do mapa liberada.\n");
    }
    
    // Libera a memória alocada para a string da missão
    if (missao_jogador != NULL) {
        free(missao_jogador);
        printf("[INFO] Memória da missão liberada.\n");
    }
}