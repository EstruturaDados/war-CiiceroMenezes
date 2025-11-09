#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Programa: Interatividade no WAR estruturado (nível intermediário)
 * Descrição:
 *   Este programa simula o cadastro e o combate entre territórios no estilo WAR.
 *   Agora, os territórios são alocados dinamicamente e manipulados por ponteiros.
 *   O sistema permite cadastrar territórios, exibi-los e simular ataques entre eles.
 *
 * Recursos incluídos:
 *   - Uso de struct Territorio.
 *   - Alocação dinâmica (malloc).
 *   - Funções separadas para cadastro, exibição, ataque e liberação de memória.
 *   - Simulação de ataque com números aleatórios (dados de 1 a 6).
 *   - Manipulação por ponteiros.
 */

// Definição da estrutura Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};
typedef struct Territorio Territorio; // apelido para simplificar a sintaxe

// Protótipos das funções
Territorio* cadastrarTerritorios(int quantidade);
void exibirTerritorios(Territorio* territorios, int quantidade);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* territorios);
int escolherTerritorio(int quantidade, const char* tipoEscolha);

int main() {
    srand(time(NULL)); // Garante aleatoriedade nas rolagens de dados
    int quantidade;

    printf("=== GUERRA DE TERRITÓRIOS - JOGO WAR ===\n\n");

    // Usuário escolhe quantos territórios deseja cadastrar
    printf("Quantos territórios deseja cadastrar? ");
    scanf("%d", &quantidade);
    getchar(); // consome o '\n' que sobra após o número

    // Aloca dinamicamente memória para os territórios
    Territorio* territorios = (Territorio*) malloc(quantidade * sizeof(Territorio));
    if (territorios == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Cadastro dos territórios
    printf("\n=== CADASTRO DE TERRITÓRIOS ===\n");
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritório %d:\n", i + 1);

        printf("Nome do território: ");
        fgets(territorios[i].nome, 30, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // limpa o buffer

        printf("-------------------------------------\n");
    }

    // Exibe todos os territórios cadastrados
    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n");
    exibirTerritorios(territorios, quantidade);

    // Simulação de ataque
    char continuar;
    do {
        printf("\n=== FASE DE ATAQUE ===\n");

        // Escolher territórios por índice
        int idxAtacante = escolherTerritorio(quantidade, "atacante");
        int idxDefensor = escolherTerritorio(quantidade, "defensor");

        // Verifica se o jogador está tentando atacar um território da mesma cor
        if (strcmp(territorios[idxAtacante].cor, territorios[idxDefensor].cor) == 0) {
            printf("\nVocê não pode atacar um território do seu próprio exército!\n");
        } else {
            atacar(&territorios[idxAtacante], &territorios[idxDefensor]);
        }

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        getchar();

    } while (continuar == 's' || continuar == 'S');

    // Exibe territórios atualizados após os ataques
    printf("\n=== ESTADO FINAL DOS TERRITÓRIOS ===\n");
    exibirTerritorios(territorios, quantidade);

    // Libera memória alocada
    liberarMemoria(territorios);

    printf("\nMemória liberada e programa finalizado.\n");
    return 0;
}

/* ----------------------------------------------------------
 * Função: exibirTerritorios
 * Exibe os dados de todos os territórios cadastrados.
 * ---------------------------------------------------------- */
void exibirTerritorios(Territorio* territorios, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\nTerritório %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("-------------------------------------\n");
    }
}

/* ----------------------------------------------------------
 * Função: escolherTerritorio
 * Pede ao jogador que selecione o índice de um território.
 * ---------------------------------------------------------- */
int escolherTerritorio(int quantidade, const char* tipoEscolha) {
    int indice;
    printf("Escolha o número do território %s (1 a %d): ", tipoEscolha, quantidade);
    scanf("%d", &indice);
    getchar();
    while (indice < 1 || indice > quantidade) {
        printf("Número inválido. Escolha entre 1 e %d: ", quantidade);
        scanf("%d", &indice);
        getchar();
    }
    return indice - 1; // retorna índice base 0
}

/* ----------------------------------------------------------
 * Função: atacar
 * Simula o ataque entre dois territórios.
 * Utiliza dados aleatórios (1 a 6) para determinar o vencedor.
 * ---------------------------------------------------------- */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n%s (%s) está atacando %s (%s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Gera valores aleatórios simulando rolagem de dados
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    // Determina o resultado do ataque
    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu a batalha!\n");
        strcpy(defensor->cor, atacante->cor); // muda a cor (controle do território)
        defensor->tropas = atacante->tropas / 2; // transfere metade das tropas
        atacante->tropas /= 2; // atacante perde metade ao conquistar
    } else if (dadoAtacante < dadoDefensor) {
        printf("O defensor resistiu ao ataque!\n");
        atacante->tropas--; // atacante perde 1 tropa
        if (atacante->tropas < 0) atacante->tropas = 0;
    } else {
        printf("Empate! Nenhum território muda de dono.\n");
    }
}

/* ----------------------------------------------------------
 * Função: liberarMemoria
 * Libera a memória alocada dinamicamente.
 * ---------------------------------------------------------- */
void liberarMemoria(Territorio* territorios) {
    free(territorios);
}