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


// Fonction qui renvoie si c'est une pièce alliée, adverse ou qu'il n'y en a pas
int check_piece_coords(Player player, Piece chess_board[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if ((x < 0 && x > 10) || (y < 0 && y > 10)) {
    printf("verif_coordonnee_piece : Erreur : coordonnées données non conformes. ");
    exit(1);
  }
 
  if (chess_board[y][x].type == 0) {
    return 0;
  } // Pas de pièce
  else if (chess_board[y][x].color == player.board.King.color) {
    return 1;
  } // Pièce allié
  else {
    return -1;
  } // Pièce ennemie
}


// verif_zone_depart va vérifier si les coordonnées sont situées dans la zone de départ autorisée au joueur
int check_starting_area(Player player, Piece chess_board[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if ((x >= 0 && x <= 10) || (y >= 0 && y <= 10)) {
  } else {
    printf("verif_zone_depart : Erreur : coordonnées données non conformes. ");
    exit(1);
  }
 
  // Vérifie si la coordonnée est située dans la zone de départ autorisée du joueur
  if ((player.position == 0) && (y == 0 || (y == 1 && (x > 0 && x < 10)) || (y == 2 && (x > 1 && x < 9)) || (y == 3 && (x > 2 && x < 8)))) {
    // la coordonnée est dans la zone de départ autorisée pour le joueur noir
    return 0;
  }
  else if ((player.position == 1) && (y == 10 || (y == 9 && (x > 0 && x < 10)) || (y == 8 && (x > 1 && x < 9)) || (y == 7 && (x > 2 && x < 8)))) {
    // la coordonnée est dans la zone de départ autorisée pour le joueur blanc
    return 0;
  }
 
  // La coordonnée n'est pas située dans la zone autorisée de départ
  return 1;
}


// retourne 1 si le player fait echec sur le roi adverse
int check_verification(Player player, Piece chess_board[11][11]) {

  int x_King;
  int y_King;
  int coord[2];
  // Retourne la position du roi adverse
  Opponent_king_position(player, chess_board, coord);
  x_King = coord[0];
  y_King = coord[1];
 
  // Parcours les cases du tableau et regarde chaque pieces allié et regarde si ils peuvent aller sur la case du roi adverse.
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {
      // Si il y a un allié qui peut se déplacer sur la case du roi adverse
      if (check_piece_coords(player, chess_board, x, y) == 1){
        if (moves_management(player, chess_board, x, y, x_King, y_King)) {
          return 1;
        } } } }
  return 0;
}

// Retourne 1 si le placement est possible sinon 0
int check_piece_placement(Player player, Piece chess_board[11][11], int x, int y, Piece piece) {

  // Vérification que les coordonnées sont valides
  if ((x < 0 && x > 10) || (y < 0 && y > 10)) {
    printf("verif_placement_piece : Erreur : coordonnées données non conformes. ");
    exit(1);
  }  
 
  // Sauvegarde la pièce qui se trouve sur la case (x,y) avant le test
  Piece piece_temporaire = chess_board[y][x];

  // Place la pièce sur la case (x,y) du tableau
  chess_board[y][x] = piece;

  // Vérifie si le placement met le roi adverse en échec
  if (check_verification(player, chess_board)) {
    chess_board[y][x] = piece_temporaire;
    // Le roi adverse est en échec, placement impossible
    return 0;
  }
  chess_board[y][x] = piece_temporaire;
  // Le roi adverse n'est pas en échec, placement autorisé
  return 1;
}   