#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define MAX_SAUDE 100
#define MAX_FOME 100
#define MAX_ENERGIA 100
#define MAX_FELICIDADE 100
#define MAX_NOME 50

typedef struct {
    char nome[MAX_NOME];
    int saude;
    int fome;
    int energia;
    int felicidade;
} Bichinho;

void alimentar(Bichinho *b) {
    b->fome = MAX_FOME;
    screenSetColor(LIGHTGREEN, BLACK);
    printf("Você alimentou %s! Fome: %d\n", b->nome, b->fome);
}

void brincar(Bichinho *b) {
    b->felicidade = MAX_FELICIDADE;
    screenSetColor(LIGHTMAGENTA, BLACK);
    printf("Você brincou com %s! Felicidade: %d\n", b->nome, b->felicidade);
}

void dormir(Bichinho *b) {
    b->energia = MAX_ENERGIA;
    screenSetColor(LIGHTCYAN, BLACK);
    printf("%s dormiu! Energia: %d\n", b->nome, b->energia);
}

void atualizarEstado(Bichinho *b) {
    if (b->fome <= 0 || b->energia <= 0 || b->felicidade <= 0) {
        b->saude--;
        screenSetColor(LIGHTRED, BLACK);
        printf("Um dos atributos de %s está no zero! Saúde diminuindo: %d\n", b->nome, b->saude);
    } else {
        b->fome -= 2;
        b->energia -= 1;
        b->felicidade -= 1;
    }
}

void exibirMenu(Bichinho *b) {
    screenClear();
    screenSetColor(YELLOW, BLACK);
    printf("%s está aguardando uma ação:\n", b->nome);
    printf("1 - Alimentar\n");
    printf("2 - Brincar\n");
    printf("3 - Dormir\n");
    printf("q - Sair\n");
}


int main() {
    Bichinho meuBichinho = { "", MAX_SAUDE, MAX_FOME, MAX_ENERGIA, MAX_FELICIDADE };
    int opcao = 0;

    screenInit(1);
    keyboardInit();
    timerInit(500);  

    screenSetColor(CYAN, BLACK);
    printf("Dê um nome ao seu bichinho: ");
    fgets(meuBichinho.nome, MAX_NOME, stdin);
    meuBichinho.nome[strcspn(meuBichinho.nome, "\n")] = '\0';

    while (meuBichinho.saude > 0) {
        exibirMenu(&meuBichinho);

        if (keyhit()) {
            opcao = readch();
            if (opcao == '1') alimentar(&meuBichinho);
            else if (opcao == '2') brincar(&meuBichinho);
            else if (opcao == '3') dormir(&meuBichinho);
            else if (opcao == 'q') break;
        }

        if (timerTimeOver()) {
            atualizarEstado(&meuBichinho);

            if (meuBichinho.saude <= 0) {
                screenSetColor(RED, BLACK);
                printf("\n%s morreu! :(\n", meuBichinho.nome);
                break;
            }

            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    return 0;
}

