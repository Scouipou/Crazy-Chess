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
#include "Game.h"

// Fonction qui va effectuer le rock
void Castle(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("roque : Erreur : coordonnées données non conformes.");
    exit(1);
  }  

  // Place le Roi
  chess_board[y2][x2] = chess_board[y1][x1];
  chess_board[y1][x1] = player.board.Empty;

  // Verifie la direction de déplacement du roi
  int direction;
  if (x2 == x1 + 2) {
    direction = 1; // Direction vers la droite
  } else  {
  direction = -1;  // Direction vers la gauche
  }
 
  // Place la Tour
  chess_board[y1][x1 + direction*1] = chess_board[y1][x1 + direction*3];
  chess_board[y1][x1 + direction*3] = player.board.Empty;
}

// Fonction qui va promouvoir le pion si il est arrivé à la dernière rangé
void pawn_promotion(Player player, Piece chess_board[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if (x < 0 || x > 10 || y < 0 || y > 10) {
    printf("promotion_pion : Erreur : coordonnées données non conformes.");
    exit(1);
  }

  // Vérification qu'un pion se trouve bien aux coordonnées données
  if (chess_board[y][x].type != player.board.Pawn.type) {
    return;
  }

  // Vérification que le pion est arrivé à la dernière rangée
  if ((player.position == 0 && y != 10) || (player.position == 1 && y != 0)) {
    return;
  }

  display_board(chess_board);
  char coord[2];

  // Affichage des choix de promotion
  Conversion_Coords_to_Chesscords(x,y,coord);
  printf("\nLe pion %s en %c%d va être promu.\n",player.color,coord[0],coord[1]);
  printf("Vous avez 5 choix de promotion possible:\n");
  printf(" 1 - Promotion en Reine\n");
  printf(" 2 - Promotion en Cavalier\n");
  printf(" 3 - Promotion en Fou\n");
  printf(" 4 - Promotion en Tour\n");
  printf(" 5 - Promotion en Cavalier-Fou\n");

  // Demande à l'utilisateur de choisir une promotion
  int choice = 0;
  printf("\nVotre choix (1-5): ");
  while (scanf("%d", &choice) != 1 || (choice < 1 || choice > 5)) {
    printf("\nChoix invalide.");
    while (getchar() != '\n');
    printf("\nVotre choix: ");
  }
 
  // Sauvegarde du nombre de mouvements de la pièce temporaire
  int nb_move = chess_board[y][x].nb_move;
 
  // Promotion du pion en fonction du choix de l'utilisateur
  switch (choice) {
    case 1:
      printf("La promotion se fera en Reine");
      chess_board[y][x] = player.board.Queen;
      break;
    case 2:
      printf("La promotion se fera en Cavalier");
      chess_board[y][x] = player.board.Knight;
      break;
    case 3:
      printf("La promotion se fera en Fou");
      chess_board[y][x] = player.board.Bishop;
      break;
    case 4:
      printf("La promotion se fera en Tour");
      chess_board[y][x] = player.board.Rook;
      break;
    case 5:
      printf("La promotion se fera en Cavalier-Fou");
      chess_board[y][x] = player.board.Knight_Bishop;
      break;
  }

  // Transfert du nombre de mouvements à la nouvelle pièce promue
  chess_board[y][x].nb_move = nb_move;
}

// Retourne 1 si le rock est possible sinon 0
int verif_Castle(Player active_player, Player passive_player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if (x1 < 0 || x1 > 10 || y1 < 0 || y1 > 10 || x2 < 0 || x2 > 10 || y2 < 0 || y2 > 10) {
    printf("Erreur : Coordonnées non conformes dans la fonction verif_rock.\n");
    exit(1);
  }

  // Si le roi est en échec alors le rock est impossible
  if (check_verification(passive_player, chess_board)) {
    return 0;
  }
 
  int direction = 0;
 
  // Vérifie si le déplacement est un roque et détermine la direction du déplacement
  if (x2 == x1 + 2) {
    direction = 1;
  } else if (x2 == x1 - 2) {
    direction = -1;
  } else {
    // Pas de roque
    return 0;
  }

  if (x2 != x1 + (direction * 2) || y2 != y1) {
    return 0;
  }
 
  // Vérifie qu'il y a bien un Roi et une Tour pour le rock
  if (chess_board[y1][x1].type != active_player.board.King.type || chess_board[y1][x1 + direction*3].type != active_player.board.Rook.type) {
    return 0;
  }
 
  // Vérifie que le Roi et la Tour n'ont pas bougé
  if (chess_board[y1][x1].nb_move != 0 || chess_board[y1][x1 + direction*3].nb_move != 0) {
    return 0;
  }
 
  // Vérifie qu'il y a bien deux cases vides entre le Roi et la Tour
  if (chess_board[y1][x1 + direction].type != active_player.board.Empty.type || chess_board[y1][x1 + direction*2].type != active_player.board.Empty.type) {
    return 0;
  }

  // On enlève la Tour pour vérifier le déplacement
  Piece temporary_piece = chess_board[y1][x1 + direction*3];
  chess_board[y1][x1 + direction*3] = active_player.board.Empty;

  for (int i = x1 + direction; i != x1 + (direction * 2); i += direction) {
    if (check_piece_move(active_player, passive_player, chess_board, x1, y1, i, y2) == 0) {
      // Remise en place de la Tour
      chess_board[y1][x1 + direction*3] = temporary_piece;
      // Retourne 0 si le roi est en échec
      return 0;
    }
  }
 
  // Remise en place de la Tour
  chess_board[y1][x1 + direction*3] = temporary_piece;
  return 1;
}

// Fonction pour demander les coordonnées de l'utilisateur et les vérifier
void ask_for_coords(int coord[2]) {
 
  int x, y;
  char letter, input[2];
  printf("\nVeuillez entrer les coordonnées (ex. 8 4 ou H4) : ");

  // Boucle infinie pour demander les coordonnées jusqu'à ce qu'elles soient valides
  while (1) {
    // Si les entrées ne sont composé que de chiffres
    if (scanf("%d %d", &x, &y) == 2 && (x > 0 && x < 12) && (y > 0 && y < 12)) {
      coord[0] = x - 1;
      coord[1] = 11 - y; // Change les coordonnées d'echec pour des coordonnées informatique
      return;
     
      // Si la première entrée est un caractère de A à K, suivi d'un entier entre 1 et 11
    } else if (scanf(" %c%d", &letter, &y) == 2 && ((letter >= 'A' && letter <= 'K') || (letter >= 'a' && letter <= 'k')) && (y > 0 && y < 12)) {

      // Si la lettre est minuscule, la convertir en majuscule
      if (letter >= 'a' && letter <= 'k') {
        letter = letter - 'a' + 'A';
      }
      coord[0] = letter - 'A';
      coord[1] = 11 - y; // Change les coordonnées d'echec pour des coordonnées informatique
      return;
    }
   
    while (getchar() != '\n');
    printf("Coordonnées invalides\n");
    printf("\nEntrez les coordonnées (ex. 8 4 ou H4) : ");
  }
}

// Fonction de conversion de coordonnées informatiques en coordonnées d'échecs
void Conversion_Coords_to_Chesscords(int x, int y, char* coord) {
 
  // Vérification que les coordonnées sont valides
  if ((x < 0 || x > 10) || (y < 0 || y > 10)) {
    printf("conversion_coord_echecs : Erreur : coordonnées données non conformes.\n");
    exit(1);
  }
  // Conversion de la colonne (x) en lettre de A à K
  char column = x + 'A';
 
  // Conversion de la ligne (y) en chiffre de 1 à 11, puis inversion pour correspondre à la notation d'échecs
  int line = 11 - y;
 
  // Stockage de la notation d'échecs dans le tableau de caractères "coord"
  coord[0] = column;
  coord[1] = line; // Conversion du chiffre en caractère
}