#ifndef GENERALCHECKS
#define GENERALCHECKS

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"
#include "Game.h"
#include "rule.h"
#include "generalchecks.h"

int check_piece_coords(Player player, Piece chess_board[11][11], int x, int y);
int check_starting_area(Player player, Piece chess_board[11][11], int x, int y);
int check_verification(Player player, Piece chess_board[11][11]);
int check_piece_placement(Player player, Piece chess_board[11][11], int x, int y, Piece piece);

#endif