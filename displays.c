#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"
#include "rule.h"
#include "Game.h"
// Fonction affichage du tableau
void display_board(Piece chess_board[11][11]) {

  printf("\n");
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {

      // Vérifier si la case est occupée par une pièce et l'afficher en conséquence
      if (chess_board[y][x].type == 0) {
        printf(". ");
      } else if (chess_board[y][x].color == 'B') {
        printf("\x1b[1m%c\x1b[0m ", chess_board[y][x].type);
      } else {
        printf("\x1b[2m%c\x1b[0m ", chess_board[y][x].type);
      }
    }
    printf("%d\n", 11 - y);
  }
  printf("A B C D E F G H I J K\n");
}