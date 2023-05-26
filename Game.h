#ifndef GAME
#define GAME

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"
#include "rule.h"
#include "generalchecks.h"

void Castle(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
void pawn_promotion(Player player, Piece chess_board[11][11], int x, int y);
int verif_Castle(Player active_player, Player passive_player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
void ask_for_coords(int coord[2]);
void Conversion_Coords_to_Chesscords(int x, int y, char* coord);


#endif