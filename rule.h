#ifndef RULE
#define RULE

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"
#include "Game.h"
#include "generalchecks.h"
int Stalemate(Player active_player, Player passive_player, Piece chess_board[11][11]);
int end_game(Player active_player, Player passive_player, Piece chess_board[11][11]);
int checkmate_verification(Player active_player, Player passive_player, Piece chess_board[11][11]);


#endif