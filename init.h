#ifndef INIT
#define INIT

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "Game.h"
#include "verifmoves.h"
#include "generalchecks.h"

void init_chess_board(Player player[2], Piece chess_board[11][11]);
void init_piece(PlayerPiece* white_board, PlayerPiece* black_board);
void check_piece_representation();

#endif