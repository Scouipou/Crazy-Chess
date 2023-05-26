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


// Retourne 1 si nul par pat
int Stalemate(Player active_player, Player passive_player, Piece chess_board[11][11]) {

   // Vérifie s'il y a échec
  if (check_verification(passive_player, chess_board)){
    // Si echec alors il n'y a pas de nul par pat  
    return 0;
  }
   
  // Vérifie si le joueur ne peut pas déplacer de pièces
  else if (check_player_moves(active_player, passive_player, chess_board)) {
    return 1;
  }
 
  // Aucune condition de match nul par pat n'est satisfaite
  return 0;
}

// Retourne 1 si échec et mat ou nul
int end_game(Player active_player, Player passive_player, Piece chess_board[11][11]) {

  // Vérifie s'il y a échec et mat
  if (checkmate_verification(active_player, passive_player, chess_board)) {
    printf("\nBravo, le joueur %s a gagné la partie par échec et mat\n", passive_player.color);
    DeleteSave();
    return 1;
  }
   
  // Vérifie s'il y a match nul
  else if (Stalemate(active_player, passive_player, chess_board)) {
    printf("\nLa partie se termine par un match nul\n");
    DeleteSave();
    return 1;
  }

  // Aucune condition de fin de partie n'est satisfaite
  return 0;
}
// Retourne 1 si le roi adverse ne peut plus bouger de case
int checkmate_verification(Player active_player, Player passive_player, Piece chess_board[11][11]) {

  // Si il n'y a pas echec alors il n'y a pas echec et mat
  if (check_verification(passive_player, chess_board) != 1){
    return 0;
  }
 
  // Récupération des coordonnées du roi adverse
  int x_King, y_King;
  int coord[2];
  Opponent_king_position(passive_player, chess_board, coord);
  x_King = coord[0];
  y_King = coord[1];
 
  // Vérification de chaque déplacement possible du roi adverse
  for (int y2 = 0; y2 < 11; y2++) {
    for (int x2 = 0; x2 < 11; x2++) {
     
      // Si le roi peut se déplacer dessus sans être en échec
      if (check_piece_move(active_player, passive_player, chess_board, x_King, y_King, x2, y2)) {
        // retourne 0 si le roi peut se déplacer sur une case
        return 0;
        } } }
 
  // Si le roi advesre ne peut se déplacer nul part sur le plateau return 1 :
  return 1;
}