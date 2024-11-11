#include <stdio.h>
#include <stdlib.h> 
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

int x = 34, y = 12;
int incX = 1, incY = 1;

typedef enum {
    FILHOTE,
    CRIANCA,
    ADOLESCENTE,
    ADULTO
} Estagio;

typedef struct {
    char *nome; 
    int saude;
    int fome;
    int energia;
    int felicidade;
    Estagio estagio;
    int interacoes;
} Bichinho;

void atualizarPosicaoBichinho() {
    screenGotoxy(x, y);
    printf(" ");

    int newX = x + incX;
    if (newX >= (MAXX - 2) || newX <= MINX + 5) incX = -incX;
    int newY = y + incY;
    if (newY >= MAXY - 10 || newY <= MINY + 3) incY = -incY;

    x = newX;
    y = newY;
    screenGotoxy(x, y);
    printf("O");
}

void exibirEstado(Bichinho *b) {
    screenGotoxy(2, MAXY - 8);
    screenSetColor(GREEN, BLACK);
    printf("Saúde: %d ", b->saude);
    screenGotoxy(2, MAXY - 7);
    screenSetColor(GREEN, BLACK);
    printf("Fome: %d ", b->fome);
    screenGotoxy(2, MAXY - 5);
    screenSetColor(GREEN, BLACK);
    printf("Felicidade: %d ", b->felicidade);
    screenGotoxy(2, MAXY - 6);
    screenSetColor(GREEN, BLACK);
    printf("Energia: %d ", b->energia);
}

void exibirEstagio(Bichinho *b) {
    const char *estagioStr[] = { "Filhote", "Criança", "Adolescente", "Adulto" };
    screenGotoxy(2, MAXY - 9);
    screenSetColor(CYAN, BLACK);
    printf("Estágio:                     ");
    screenGotoxy(2, MAXY - 9);
    printf("Estágio: %s", estagioStr[b->estagio]);
}

void exibirMensagem(const char *mensagem) {
    screenGotoxy(2, MAXY - 22);;
    printf("                                        ");
    screenGotoxy(2, MAXY - 22);
    printf("%s", mensagem);
}

void alimentar(Bichinho *b) {
    b->fome = MAX_FOME;
    b->energia -= 5;
    b->interacoes++;
    screenSetColor(GREEN, BLACK);
    exibirMensagem("Você alimentou o bichinho!");
}

void brincar(Bichinho *b) {
    b->felicidade = MAX_FELICIDADE;
    b->energia -= 10;
    b->interacoes++;
    screenSetColor(GREEN, BLACK);
    exibirMensagem("Você brincou com o bichinho!");
}

void dormir(Bichinho *b) {
    b->energia = MAX_ENERGIA;
    b->interacoes++;
    exibirMensagem("O bichinho dormiu e recuperou energia!");
}

void atualizarEstado(Bichinho *b) {
    if (b->fome <= 0 || b->energia <= 0 || b->felicidade <= 0) {
        b->saude--; 
        screenSetColor(RED, BLACK);
        exibirMensagem("Cuidado! Saúde está diminuindo.");
    } else {
        b->fome -= 1;
        b->energia -= 1;
        b->felicidade -= 2;
    }

    if (b->interacoes >= 20 && b->estagio == ADOLESCENTE) {
        b->estagio = ADULTO;
        screenSetColor(CYAN, BLACK);
        exibirMensagem("Parabéns! O bichinho virou Adulto!");
    } else if (b->interacoes >= 15 && b->estagio == CRIANCA) {
        b->estagio = ADOLESCENTE;
        screenSetColor(CYAN, BLACK);
        exibirMensagem("O bichinho agora é um Adolescente!");
    } else if (b->interacoes >= 10 && b->estagio == FILHOTE) {
        b->estagio = CRIANCA;
        screenSetColor(CYAN, BLACK);
        exibirMensagem("O bichinho evoluiu para Criança!");
    }
}

void exibirMenu() {
    screenSetColor(CYAN, BLACK);
    screenSetColor(YELLOW, BLACK);
    screenGotoxy(2, MAXY - 20);
    printf("Escolha uma ação:\n");
    screenGotoxy(2, MAXY - 19);
    printf("1 - Alimentar\n");
    screenGotoxy(2, MAXY - 18);
    printf("2 - Dormir\n");
    screenGotoxy(2, MAXY - 17);
    printf("3 - Brincar\n");
    screenGotoxy(2, MAXY - 16);
    printf("q - Sair\n");
}

void pedirNome(Bichinho *b) {
    screenSetColor(CYAN, BLACK);
    screenGotoxy(2, MAXY - 21);
    printf("O nome do seu bichinho é: ");

    int pos = 0;
    char ch;

    b->nome = (char *)malloc(MAX_NOME * sizeof(char));
    if (b->nome == NULL) {
        exibirMensagem("Erro ao alocar memória para o nome!");
        exit(1);
    }

    while (1) {
        ch = getchar();

        if (ch == '\n' && pos > 0) {
            b->nome[pos] = '\0';
            break;
        } else if (ch == '\b' || ch == 127) {
            if (pos > 0) {
                pos--;
                b->nome[pos] = '\0';
                screenGotoxy(26 + pos, 2);
                printf(" ");
                screenGotoxy(26 + pos, 2);
            }
        } else if (pos < MAX_NOME - 1) {
            b->nome[pos++] = ch;
            b->nome[pos] = '\0';
            printf("%c", ch);
        }
    }

    while (getchar() != '\n');
}

void liberarMemoria(Bichinho *b) {
    free(b->nome);
}

int main() {
    Bichinho meuBichinho = { NULL, MAX_SAUDE, MAX_FOME, MAX_ENERGIA, MAX_FELICIDADE, FILHOTE, 0 };
    char opcao;

    screenInit(1);
    keyboardInit();
    timerInit(500);
    pedirNome(&meuBichinho);
    exibirMensagem("Comece a cuidar do seu bichinho!");

    while (meuBichinho.saude > 0) {
        exibirMenu();
        exibirEstado(&meuBichinho);
        exibirEstagio(&meuBichinho);

        if (keyhit()) {
            opcao = readch();
            if (opcao == '1') alimentar(&meuBichinho);
            else if (opcao == '2') dormir(&meuBichinho);
            else if (opcao == '3') brincar(&meuBichinho);
            else if (opcao == 'q') break;
        }

        if (timerTimeOver()) {
            atualizarEstado(&meuBichinho);
            atualizarPosicaoBichinho();
            screenUpdate();
        }
    }

    exibirMensagem("O bichinho morreu. :( Obrigado por jogar!");
    sleep(2);

    liberarMemoria(&meuBichinho);

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
