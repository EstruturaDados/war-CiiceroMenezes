#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * ===========================================
 * PROJETO: WAR ESTRUTURADO - NÍVEL AVANÇADO
 * ===========================================
 *
 * Descrição:
 *   Este programa simula uma versão simplificada e estruturada do jogo WAR.
 *   Inclui:
 *     - Cadastro dinâmico de territórios.
 *     - Sistema de ataque entre territórios.
 *     - Atribuição aleatória de missões estratégicas a cada jogador.
 *     - Verificação automática de vitória com base nas missões.
 *
 * Conceitos aplicados:
 *   - Ponteiros e passagem por referência.
 *   - Alocação e liberação dinâmica de memória.
 *   - Modularização (várias funções com responsabilidades específicas).
 *   - Aleatoriedade com srand() e rand().
 *   - Manipulação de strings com strcpy() e fgets().
 */

// Estrutura principal dos territórios do jogo
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ======================
// Protótipos das funções
// ======================

// Funções principais do jogo
Territorio* cadastrarTerritorios(int* quantidade);
void exibirMapa(Territorio* territorios, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);

// Funções relacionadas às missões
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void exibirMissao(const char* missao);

// Gerenciamento de memória
void liberarMemoria(Territorio* territorios, char* missaoJogador1, char* missaoJogador2);

// Funções auxiliares
int escolherTerritorio(int quantidade, const char* tipoEscolha);

// ======================
// Função principal (main)
// ======================

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int quantidade;
    printf("=== GUERRA DE TERRITÓRIOS - JOGO WAR (NÍVEL AVANÇADO) ===\n\n");

    // Cadastro dinâmico dos territórios
    Territorio* territorios = cadastrarTerritorios(&quantidade);

    // Vetor de missões pré-definidas
    char* missoes[] = {
        "Conquistar 3 territórios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Controlar pelo menos 2 territórios com mais de 10 tropas.",
        "Dominar todos os territórios de cor azul.",
        "Possuir o dobro de tropas do inimigo."
    };
    int totalMissoes = 5;

    // Atribuição dinâmica das missões aos dois jogadores
    char* missaoJogador1 = (char*) malloc(100 * sizeof(char));
    char* missaoJogador2 = (char*) malloc(100 * sizeof(char));

    atribuirMissao(missaoJogador1, missoes, totalMissoes);
    atribuirMissao(missaoJogador2, missoes, totalMissoes);

    printf("\n=== MISSÕES ATRIBUÍDAS ===\n");
    printf("Jogador 1: ");
    exibirMissao(missaoJogador1);
    printf("Jogador 2: ");
    exibirMissao(missaoJogador2);

    // Exibe o estado inicial do mapa
    printf("\n=== ESTADO INICIAL DO MAPA ===\n");
    exibirMapa(territorios, quantidade);

    // -------------------
    // FASE DE BATALHAS
    // -------------------
    char continuar;
    int turno = 1;
    do {
        printf("\n=== TURNO %d ===\n", turno);

        // Define qual jogador está atacando
        char* corAtacante = (turno % 2 != 0) ? "Vermelho" : "Azul";
        printf("Vez do jogador (%s)\n", corAtacante);

        // Escolha dos territórios
        int idxAtacante = escolherTerritorio(quantidade, "atacante");
        int idxDefensor = escolherTerritorio(quantidade, "defensor");

        // Validação: impedir ataque ao próprio exército
        if (strcmp(territorios[idxAtacante].cor, territorios[idxDefensor].cor) == 0) {
            printf("\nVocê não pode atacar um território do seu próprio exército!\n");
        } else {
            atacar(&territorios[idxAtacante], &territorios[idxDefensor]);
        }

        // Exibe mapa atualizado após a batalha
        printf("\n--- MAPA APÓS O TURNO ---\n");
        exibirMapa(territorios, quantidade);

        // Verifica se algum jogador cumpriu sua missão
        if (verificarMissao(missaoJogador1, territorios, quantidade)) {
            printf("\n🎉 Jogador 1 venceu! Missão cumprida: %s\n", missaoJogador1);
            break;
        }
        if (verificarMissao(missaoJogador2, territorios, quantidade)) {
            printf("\n🎉 Jogador 2 venceu! Missão cumprida: %s\n", missaoJogador2);
            break;
        }

        printf("\nDeseja continuar a guerra? (s/n): ");
        scanf(" %c", &continuar);
        getchar();
        turno++;

    } while (continuar == 's' || continuar == 'S');

    // Libera toda a memória alocada dinamicamente
    liberarMemoria(territorios, missaoJogador1, missaoJogador2);
    printf("\nJogo encerrado. Memória liberada com sucesso.\n");
    return 0;
}

// ======================
// Implementação das funções
// ======================

/*
 * Função: cadastrarTerritorios
 * ----------------------------
 * Solicita ao usuário o número de territórios e preenche os dados de cada um.
 * Retorna o ponteiro para o vetor alocado dinamicamente.
 */
Territorio* cadastrarTerritorios(int* quantidade) {
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", quantidade);
    getchar();

    Territorio* territorios = (Territorio*) malloc((*quantidade) * sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    for (int i = 0; i < *quantidade; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: ");
        fgets(territorios[i].nome, 30, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do exército (ex: Vermelho, Azul, Verde): ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Tropas iniciais: ");
        scanf("%d", &territorios[i].tropas);
        getchar();
    }

    return territorios;
}

/*
 * Função: exibirMapa
 * ------------------
 * Exibe o estado atual de todos os territórios.
 */
void exibirMapa(Territorio* territorios, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n[%d] %s - Cor: %s - Tropas: %d\n",
               i + 1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }
}

/*
 * Função: atacar
 * --------------
 * Simula um ataque entre dois territórios com base em rolagem de dados.
 * Atualiza tropas e cores conforme o resultado.
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n%s (%s) ataca %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Vitória do atacante!\n");
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas /= 2;
    } else {
        printf("O defensor resistiu!\n");
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/*
 * Função: atribuirMissao
 * ----------------------
 * Sorteia uma missão aleatória entre o vetor de missões e copia
 * o texto para o ponteiro de destino (missão do jogador).
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

/*
 * Função: exibirMissao
 * --------------------
 * Mostra a missão de um jogador de forma amigável.
 */
void exibirMissao(const char* missao) {
    printf("%s\n", missao);
}

/*
 * Função: verificarMissao
 * -----------------------
 * Avalia se a missão do jogador foi cumprida.
 * (Nesta versão inicial, a verificação é simbólica e simplificada.)
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    // Lógica simples: se algum território tiver mais de 15 tropas, considera missão cumprida.
    for (int i = 0; i < tamanho; i++) {
        if (mapa[i].tropas > 15) {
            return 1; // missão cumprida
        }
    }
    return 0; // missão ainda não cumprida
}

/*
 * Função: liberarMemoria
 * ----------------------
 * Libera toda a memória alocada dinamicamente durante o jogo.
 */
void liberarMemoria(Territorio* territorios, char* missaoJogador1, char* missaoJogador2) {
    free(territorios);
    free(missaoJogador1);
    free(missaoJogador2);
}

/*
 * Função: escolherTerritorio
 * --------------------------
 * Solicita ao jogador o número do território e valida a entrada.
 */
int escolherTerritorio(int quantidade, const char* tipoEscolha) {
    int indice;
    printf("Escolha o número do território %s (1 a %d): ", tipoEscolha, quantidade);
    scanf("%d", &indice);
    getchar();

    while (indice < 1 || indice > quantidade) {
        printf("Valor inválido! Escolha entre 1 e %d: ", quantidade);
        scanf("%d", &indice);
        getchar();
    }
    return indice - 1; // índice ajustado para base 0
}