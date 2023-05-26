#ifndef VERIFMOVES
#define VERIFMOVES

#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "Game.h"
#include "rule.h"
#include "generalchecks.h"
int check_pawn_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
int check_knight_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
int check_diagonal_move(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2, int allowed_distance);
int check_linear_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2, int allowed_distance);
int moves_management(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
void Piece_moves(Player active_player, Player passive_player, Piece chess_board[11][11]);
int check_player_moves(Player active_player, Player passive_player, Piece chess_board[11][11]);
int check_piece_move(Player active_player, Player passive_player, Piece chess_board[11][11], int x1, int y1, int x2, int y2);
void Opponent_king_position(Player player, Piece chess_board[11][11], int coord[2]);


#endif