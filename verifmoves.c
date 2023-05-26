#include <stdio.h>
#include <stdlib.h>
#include "displays.h"
#include "Save.h"
#include "struct.h"
#include "const.h"
#include "init.h"
#include "verifmoves.h"
#include "generalchecks.h"
#include "rule.h"
#include "Game.h"

// Fonction qui vérifie si un déplacement de pion est valide
int check_pawn_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_Pion : Erreur : coordonnées données non conformes. ");
    exit(1);
  }  

  int direction;
  if (player.position == 0) {
    direction = -1;
  } else {
    direction = 1;
  }
 
  // On vérifie si le déplacement correspond à un déplacement normal d'un pion
  if (x1 == x2 && y1 == y2 + direction && check_piece_coords(player, chess_board, x2, y2) == 0){
    return 1;
  }
   
  // On vérifie si le déplacement correspond à un double déplacement du pion lors de son premier mouvement
  else if ((chess_board[y1][x1].nb_move == 0) && (x1 == x2 && y1 == y2 + 2 * direction) && (check_piece_coords(player, chess_board, x2, y2) == 0 ) && (check_piece_coords(player, chess_board, x2, y2 + direction) == 0 )) {
    return 1;
  }

  // On vérifie si le déplacement correspond à une prise diagonale d'une pièce adverse
  else if (y1 == y2 + direction && (x1 == x2 - 1 || x1 == x2 + 1) && check_piece_coords(player, chess_board, x2, y2) == -1) {
    return 1;
  }
 
  return 0; // Si aucun cas n'est vérifié, le déplacement est invalide
}

// Fonction qui vérifie si un déplacement en L est valide
int check_knight_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_cavalier : Erreur : coordonnées données non conformes. ");
    exit(1);
  }
 
  // On vérifie si le déplacement correspond à un déplacement en L
  if ((abs(x2 - x1) == 2 && abs(y2 - y1) == 1) || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2)) {
    if (check_piece_coords(player, chess_board, x2, y2) != 1) {
     return 1;
    } }
  return 0; // Si aucun cas n'est vérifié, le déplacement est invalide
}

// Fonction qui vérifie si un déplacement diagonale est valide
int check_diagonal_move(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2, int allowed_distance) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_diagonale : Erreur : coordonnées données non conformes. ");
    exit(1);
  }
 
  int distance = 0;
  int step_x, step_y;
 
  // Déterminer la direction de déplacement selon l'axe x et y
  if (x1 < x2) {
    step_x = 1; // La pièce se déplace vers la droite
  } else {
    step_x = -1; // La pièce se déplace vers la gauche
  }
  if (y1 < y2) {
    step_y = 1; // La pièce se déplace vers le haut
  } else {
    step_y = -1; // La pièce se déplace vers le bas
  }

  // Vérifier si le déplacement est en diagonale
  if (abs(x2 - x1) == abs(y2 - y1)) {
    // Parcourir le chemin de la diagonale
    for (int i = 1; i <= abs(x2 - x1); i++) {
      distance += 1;
      int x = x1 + step_x * i;
      int y = y1 + step_y * i;

      // Vérifier si la case d'arrivée est la même que les coordonnées souhaité
      if ((x == x2 && y == y2) && (distance <= allowed_distance) && (check_piece_coords(player, chess_board, x, y) != 1)){
        return 1;
      }
      // Vérifier si la case est occupée par une pièce
      if (check_piece_coords(player, chess_board, x, y) != 0) {
        return 0; // Déplacement invalide, chemin bloqué par une pièce
      }

      // Vérifier si la distance autorisée a été dépassée
      if (distance > allowed_distance) {
        return 0; // Déplacement invalide, distance maximale atteinte
      }
    }
    // Si toutes les cases du chemin sont vides et que la distance autorisée n'est pas dépassée
    return 1; // Déplacement valide
  } else {
    return 0; // Déplacement non valide, pas en diagonale
  }
}

// Fonction qui vérifie si un déplacement horizontale ou verticale est valide
int check_linear_moves(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2, int allowed_distance) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_ligne : Erreur : coordonnées données non conformes. ");
    exit(1);
  }
 
  int distance = 0;
  int step_x, step_y;
  // Déterminer la direction de déplacement selon l'axe x et y
  if (x1 < x2) {
    step_x = 1; // La pièce se déplace vers la droite
  } else {
    step_x = -1; // La pièce se déplace vers la gauche
  }
  if (y1 < y2) {
    step_y = 1; // La pièce se déplace vers le bas
  } else {
    step_y = -1; // La pièce se déplace vers le haut
  }

  // Vérifier que toutes les cases entre x1 et x2 sont vides
  if (y1 == y2) { // Déplacement vertical
    for (int i = x1; i != x2; i += step_y) {
      distance += 1;
      if ((check_piece_coords(player, chess_board, i + step_x, y1) != 1) && ((i + step_x == x2) && (y1 == y2)) && (distance <= allowed_distance)) {
        return 1;
      } else if ((check_piece_coords(player, chess_board, i + step_x, y1) != 0) || (distance > allowed_distance)) {
        return 0;
      }
    }
  } else if (x1 == x2) { // Déplacement horizontal
    for (int i = y1; i != y2; i += step_y) {
      distance += 1;
      if ((check_piece_coords(player, chess_board, x1, i + step_y) != 1) && ((x1 == x2) && (i + step_y == y2)) && (distance <= allowed_distance)) {
        return 1;
      } else if ((check_piece_coords(player, chess_board, x1, i + step_y) != 0) || (distance > allowed_distance)) {
        return 0; // Déplacement invalide
      }
    }
  } else {
    return 0;
  }
  return 1;
}

// Fonction qui gère le ou les fonction de déplacement de la pièce, retourne 1 si le déplacement est valide
int moves_management(Player player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("gestion_deplacement : Erreur : coordonnées données non conformes. ");
    exit(1);
  }

  // Vérifie le type de la pièce à déplacer et appelle la fonction de déplacement appropriée
  if (chess_board[y1][x1].type == player.board.Queen.type) {
    if (check_linear_moves(player, chess_board, x1, y1, x2, y2, 10) || (check_diagonal_move(player, chess_board, x1, y1, x2, y2, 10))) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Knight.type) {
    if (check_knight_moves(player, chess_board, x1, y1, x2, y2)) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Bishop.type) {
    if (check_diagonal_move(player, chess_board, x1, y1, x2, y2, 10)) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Rook.type) {
    if (check_linear_moves(player, chess_board, x1, y1, x2, y2, 10)) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Knight_Bishop.type) {
    if (check_knight_moves(player, chess_board, x1, y1, x2, y2) || (check_diagonal_move(player, chess_board, x1, y1, x2, y2, 10))) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Prince.type) {
    if (check_linear_moves(player, chess_board, x1, y1, x2, y2, 2) || (check_diagonal_move(player, chess_board, x1, y1, x2, y2, 2))) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.King.type) {
    if (check_linear_moves(player, chess_board, x1, y1, x2, y2, 1) || (check_diagonal_move(player, chess_board, x1, y1, x2, y2, 1))) {
      return 1;
    }
  } else if (chess_board[y1][x1].type == player.board.Pawn.type) {
    if (check_pawn_moves(player, chess_board, x1, y1, x2, y2)) {
      return 1;
    }
  }
  // Si le déplacement n'est pas possible, retourne 0
  return 0;
}

// Retourne 1 si le deplacement est possible sinon 0
int check_piece_move(Player active_player, Player passive_player, Piece chess_board[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_piece : Erreur : coordonnées données non conformes. ");
    exit(1);
  }

  // Vérifie si les coordonnées de départ et d'arrivée contiennent une pièce alliée alors le déplacment est invalide
  if (check_piece_coords(active_player, chess_board, x1, y1) == check_piece_coords(active_player, chess_board, x2, y2)) {
    return 0;
  }
 
  // Vérifie si le déplacement de la pièce de la première coordonnée à la seconde est valide
  if (moves_management(active_player, chess_board, x1, y1, x2, y2) != 1) {
    return 0;
  }
 
  // Sauvegarde des pièces pour les remettre après le test de déplacement
  Piece temporary_piece1 = chess_board[y1][x1];
  Piece temporary_piece2 = chess_board[y2][x2];
 
  // Test du déplacement
  chess_board[y1][x1] = active_player.board.Empty; // On vide la case de départ
  chess_board[y2][x2] = temporary_piece1; // On ajoute la pièce d'arrivé
 
  // Retourne 0 si le déplacement est impossible
  if (check_verification(passive_player, chess_board)) {
    // Remise en place des pièces
    chess_board[y2][x2] = temporary_piece2;
    chess_board[y1][x1] = temporary_piece1;
    return 0;
  }
  // Remise en place des pièces
  chess_board[y2][x2] = temporary_piece2;
  chess_board[y1][x1] = temporary_piece1;

  // Retourne 1 si le déplacement est possible
  return 1;
}

// Retourne 1 si le joueur ne peut plus déplacer de pièces
int check_player_moves(Player active_player, Player passive_player, Piece chess_board[11][11]) {

  int x1, y1, x2, y2;

  // Parcourt toutes les coordonnées possibles sur le plateau de jeu
  for (y1 = 0; y1 < 11; y1++) {
    for (x1 = 0; x1 < 11; x1++) {

      // Vérifie si la case contient une pièce appartenant au joueur
      if (check_piece_coords(active_player, chess_board, x1, y1) == 1){
       
        // Parcourt à nouveau toutes les coordonnées possibles sur le plateau de jeu
        for (y2 = 0; y2 < 11; y2++) {
          for (x2 = 0; x2 < 11; x2++) {

            // Vérifie si le déplacement de la pièce de la première coordonnée à la seconde est possible
            if (check_piece_move(active_player, passive_player, chess_board, x1, y1, x2, y2) && (x1 != x2 || y1 != y2)){
             
              return 0;
            } } } } } }
  return 1;
}


// Fonction placement_piece gère la vérification et le déplacement d'une pièce
void Piece_moves(Player active_player, Player passive_player, Piece chess_board[11][11]) {

  int x1, y1, x2, y2;

  // Tableau pour stocker les coordonnées entrées par le joueur
  int coord[2];
 
  printf("\nJoueur %s, c'est à vous de jouer.", active_player.color);
 
  // Boucle pour demander les coordonnées de départ jusqu'à ce qu'elles soient valides
  while (1) {
    printf("\nQuelle pièce souhaitez-vous déplacer ?");
   
    while (1) {
      // Demande des coordonnées de départ
      ask_for_coords(coord);
      x1 = coord[0];
      y1 = coord[1];

      // Vérification des coordonnées de départ
      if (check_piece_coords(active_player, chess_board, x1, y1) == 1 && ((x1 != x2) || (y1 != y2))) {
        break;
      }
      printf("Coordonnées invalides\n");
    }
   
    // Demande des coordonnées d'arrivée
    printf("\nOù souhaitez vous déplacer votre pièce");
    ask_for_coords(coord);
    x2 = coord[0];
    y2 = coord[1];

    // Vérification du rock
    if (verif_Castle(active_player, passive_player, chess_board, x1, y1, x2, y2)) {
      Castle(active_player, chess_board, x1, y1, x2, y2);
      break;
    }
   
    // Vérification que le déplacement est valide
    else if (check_piece_move(active_player, passive_player, chess_board, x1, y1, x2, y2)){
     
      // On déplace la pièce
      chess_board[y2][x2] = chess_board[y1][x1];
      chess_board[y1][x1] = active_player.board.Empty;
      chess_board[y2][x2].nb_move += 1;
       
      // Vérification de la promotion du pion
      pawn_promotion(active_player, chess_board, x2, y2);
      break;
      }
    printf("\nDéplacement impossible\n");  }
}

// Fonction qui donne la position du roi adverse
void Opponent_king_position(Player player, Piece chess_board[11][11], int coord[2]) {

  // Parcours le tableau de jeu en recherchant le roi adverse
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {
     
      // Si la pièce est un roi et qu'il n'appartient pas au joueur
      if ((chess_board[y][x].type == player.board.King.type) && (check_piece_coords(player, chess_board, x, y) == -1)) {
        coord[0] = x;
        coord[1] = y;
        return ;
      } } }
 
  // Si aucun roi adverse n'a été trouvé, affiche une erreur et arrête le programme
  printf("position_roi_adverse : Erreur : Pas de roi");
  exit(1);
}