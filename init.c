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
#include "generalchecks.h"



// Fonction qui vérifie les représentation des pièces
void check_piece_representation() {

  char representations[8] = {
    QUEEN_REPRESENTATION,
    KNIGHT_REPRESENTATION,
    BISHOP_REPRESENTATION,
    ROOK_REPRESENTATION,
    KNIGHT_BISHOP_REPRESENTATION,
    PRINCE_REPRESENTATION,
    KING_REPRESENTATION,
    PAWN_REPRESENTATION
  };

  for (int i = 0; i < 7; i++) {
    for (int j = i + 1; j < 8; j++) {
      if (representations[i] == representations[j]) {
        printf("verif_representation_piece : Erreur : même representation");
        exit(1);
      } } }
 
  return;
}


// Fonction pour l'initialisation du plateau de jeux
void init_chess_board(Player player[2], Piece chess_board[11][11]) {
 
  // Placement des pièces blanches
  chess_board[9][5] = player[0].board.King;
  chess_board[10][0] = player[0].board.Pawn;
  chess_board[9][1] = player[0].board.Pawn;
  chess_board[8][2] = player[0].board.Pawn;
  chess_board[7][3] = player[0].board.Pawn;
  chess_board[10][10] = player[0].board.Pawn;
  chess_board[9][9] = player[0].board.Pawn;
  chess_board[8][8] = player[0].board.Pawn;
  chess_board[7][7] = player[0].board.Pawn;

  // Placement des pièces noires
  chess_board[1][5] = player[1].board.King;
  chess_board[0][0] = player[1].board.Pawn;
  chess_board[1][1] = player[1].board.Pawn;
  chess_board[2][2] = player[1].board.Pawn;
  chess_board[3][3] = player[1].board.Pawn;
  chess_board[0][10] = player[1].board.Pawn;
  chess_board[1][9] = player[1].board.Pawn;
  chess_board[2][8] = player[1].board.Pawn;
  chess_board[3][7] = player[1].board.Pawn;

}

// Fonction pour l'initialisation des pièces
void init_piece(PlayerPiece* white_board, PlayerPiece* black_board) {

  // Vérifie que les pointeurs ne sont pas nuls
  if (!white_board || !black_board) {
    printf("init_piece : Erreur : pointeur null\n");
    exit(1);
  }
 
  // Initialisation des pièces Blanches
  *white_board = (PlayerPiece) {
    {QUEEN_REPRESENTATION, 'B', 0},
    {KNIGHT_REPRESENTATION, 'B', 0},
    {BISHOP_REPRESENTATION, 'B', 0},
    {ROOK_REPRESENTATION, 'B', 0},
    {KNIGHT_BISHOP_REPRESENTATION, 'B', 0},
    {PRINCE_REPRESENTATION, 'B', 0},
    {KING_REPRESENTATION, 'B', 0},
    {PAWN_REPRESENTATION, 'B', 0},
    {0}
  };
 

  // Initialisation des pièces Noires
  *black_board = (PlayerPiece) {
    {QUEEN_REPRESENTATION, 'N', 0},
    {KNIGHT_REPRESENTATION, 'N', 0},
    {BISHOP_REPRESENTATION, 'N', 0},
    {ROOK_REPRESENTATION, 'N', 0},
    {KNIGHT_BISHOP_REPRESENTATION, 'N', 0},
    {PRINCE_REPRESENTATION, 'N', 0},
    {KING_REPRESENTATION, 'N', 0},
    {PAWN_REPRESENTATION, 'N', 0},
    {0}
  };
}