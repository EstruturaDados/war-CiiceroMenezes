#include <stdio.h>
#include <string.h>

/*
 * Programa: Cadastro de Territórios - Jogo War
 * Descrição: Este programa cadastra 5 territórios, armazenando
 * nome, cor do exército e quantidade de tropas. Ao final, exibe
 * todos os dados cadastrados.
 */

// Definição da estrutura Territorio
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main() {
    struct Territorio territorios[5]; // Vetor para 5 territórios
    int i; // contador

    printf("=== CADASTRO DE TERRITÓRIOS - JOGO WAR ===\n\n");

    // Cadastro dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("Cadastro do território %d:\n", i + 1);

        printf("Nome do território: ");
        fflush(stdin); // limpa buffer (funciona na maioria dos compiladores)
        fgets(territorios[i].nome, 30, stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // remove '\n'

        printf("Cor do exército: ");
        fgets(territorios[i].cor, 10, stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // consome o '\n' que sobra após o número

        printf("-------------------------------------\n");
    }

    // Exibição dos territórios cadastrados
    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n\n");
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("-------------------------------------\n");
    }

    printf("\nCadastro concluído com sucesso!\n");
    return 0;
}