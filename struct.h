#ifndef STRUCT
#define STRUCT

#include <stdio.h>
#include <stdlib.h>

// Structure Piece
typedef struct {
  char type;   // Sert à identifier la pièce (cavalier, tour, etc)
  char color;  // Sert à différencier les deux couleurs (blanc et noir)
  int nb_move; // Compte le nombre de déplacement de la pièce
} Piece;

// Structure Board sert à regrouper les différente pièce du joueur (Ce n'est pas les pièces qui sont sur le terrain)
typedef struct {
  Piece Queen;
  Piece Knight;
  Piece Bishop;
  Piece Rook;
  Piece Knight_Bishop;
  Piece Prince;
  Piece King;
  Piece Pawn;
  Piece Empty; // Sert à vider la case occupé par une piece avant son déplacement
} PlayerPiece;

// Structure Player
typedef struct {
  int points;     // Sert à acheter des pièces dans la phase_achat
  int position;   // Sert à identifier de quel côté le joueur se situe (1 en bas - Blanc / 0 en haut - Noir)
  char color[10]; // Par défaut c'est Blanc et Noir
  PlayerPiece board;    // Regrouper les différentes pièces du joueurs
  int end_of_purchases;  // Savoir si le joueur a fini sa phase d'achat (0 pas fini \ 1 fini)
} Player;

// Structure Game (sauvegarde)
typedef struct {
  Piece chess_board[11][11];
  Player player[2];
  int playerTurn; // Sert à savoir qui est le joueur qui joue
} Game;
#endif
