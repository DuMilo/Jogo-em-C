#include <stdio.h>
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
    char nome[MAX_NOME];
    int saude;
    int fome;
    int energia;
    int felicidade;
    Estagio estagio; 
    int interacoes;   
} Bichinho;

void atualizarPosicaoBichinho() {
    int newX = x + incX;
    if (newX >= (MAXX - 1) || newX <= MINX) incX = -incX;
    int newY = y + incY;
    if (newY >= MAXY - 1 || newY <= MINY) incY = -incY;
   
    x = newX;
    y = newY;
    screenGotoxy(x, y);
    printf("O");
}

void exibirEstado(Bichinho *b) {
    screenGotoxy(2, MAXY - 4);
    printf("Saúde: %d ", b->saude);
    screenGotoxy(2, MAXY - 3);
    printf("Fome: %d ", b->fome);
    screenGotoxy(2, MAXY - 2);
    printf("Energia: %d ", b->energia);
    screenGotoxy(2, MAXY - 1);
    printf("Felicidade: %d ", b->felicidade);
}

void exibirEstagio(Bichinho *b) {
    char *estagioStr = "Filhote";
    switch (b->estagio) {
        case FILHOTE: estagioStr = "Filhote"; break;
        case CRIANCA: estagioStr = "Criança"; break;
        case ADOLESCENTE: estagioStr = "Adolescente"; break;
        case ADULTO: estagioStr = "Adulto"; break;
    }
    screenGotoxy(2, MAXY - 6);
    printf("Estágio: %s", estagioStr);
}

void alimentar(Bichinho *b) {
    b->fome = MAX_FOME;
    b->energia -= 5;  
    b->interacoes++;   
    screenSetColor(LIGHTGREEN, BLACK);
    screenGotoxy(2, 1);
    printf("Você alimentou %s! Fome: %d", b->nome, b->fome);
}

void brincar(Bichinho *b) {
    b->felicidade = MAX_FELICIDADE;
    b->energia -= 10; 
    b->interacoes++;  
    screenSetColor(LIGHTMAGENTA, BLACK);
    screenGotoxy(2, 1);
    printf("Você brincou com %s! Felicidade: %d", b->nome, b->felicidade);
}

void dormir(Bichinho *b) {
    b->energia = MAX_ENERGIA;
    b->interacoes++;  
    screenSetColor(LIGHTCYAN, BLACK);
    screenGotoxy(2, 1);
    printf("%s dormiu! Energia: %d", b->nome, b->energia);
}

void atualizarEstado(Bichinho *b) {
    if (b->fome <= 0 || b->energia <= 0 || b->felicidade <= 0) {
        b->saude--;
        screenSetColor(LIGHTRED, BLACK);
        screenGotoxy(2, MAXY - 5);
        printf("Um dos atributos de %s está no zero! Saúde diminuindo: %d", b->nome, b->saude);
    } else {
        b->fome -= 2;
        b->energia -= 1;
        b->felicidade -= 1;
    }

    if (b->interacoes >= 15 && b->estagio == ADOLESCENTE) {
        b->estagio = ADULTO;
        screenSetColor(GREEN, BLACK);
        screenGotoxy(2, MAXY - 7);
        printf("Parabéns! %s agora é um Adulto!", b->nome);
    } else if (b->interacoes >= 10 && b->estagio == CRIANCA) {
        b->estagio = ADOLESCENTE;
        screenSetColor(GREEN, BLACK);
        screenGotoxy(2, MAXY - 7);
        printf("%s evoluiu para Adolescente!", b->nome);
    } else if (b->interacoes >= 5 && b->estagio == FILHOTE) {
        b->estagio = CRIANCA;
        screenSetColor(GREEN, BLACK);
        screenGotoxy(2, MAXY - 7);
        printf("%s evoluiu para Criança!", b->nome);
    }
}

void exibirMenu(Bichinho *b) {
    screenGotoxy(2, 5);
    screenSetColor(YELLOW, BLACK);
    printf("%s está aguardando uma ação:\n", b->nome);
    printf("1 - Alimentar\n");
    printf("2 - Brincar\n");
    printf("3 - Dormir\n");
    printf("q - Sair\n");
}

void pedirNome(Bichinho *b) {
    screenSetColor(CYAN, BLACK);
    screenGotoxy(2, 2);
    printf("Dê um nome ao seu bichinho: ");
    fgets(b->nome, MAX_NOME, stdin);
    b->nome[strcspn(b->nome, "\n")] = '\0';
}

int main() {
    Bichinho meuBichinho = { "", MAX_SAUDE, MAX_FOME, MAX_ENERGIA, MAX_FELICIDADE, FILHOTE, 0 };
    int opcao = 0;

    screenInit(1);
    keyboardInit();
    timerInit(500);

    pedirNome(&meuBichinho);

    while (meuBichinho.saude > 0) {
        exibirMenu(&meuBichinho);
        exibirEstado(&meuBichinho);
        exibirEstagio(&meuBichinho);

        if (keyhit()) {
            opcao = readch();
            if (opcao == '1') alimentar(&meuBichinho);
            else if (opcao == '2') brincar(&meuBichinho);
            else if (opcao == '3') dormir(&meuBichinho);
            else if (opcao == 'q') break;
        }

        if (timerTimeOver()) {
            atualizarEstado(&meuBichinho);
            atualizarPosicaoBichinho();
            screenUpdate();
        }
    }

    if (meuBichinho.saude <= 0) {
        screenSetColor(RED, BLACK);
        screenGotoxy(2, 1);
        printf("\n%s morreu! :(\n", meuBichinho.nome);
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
