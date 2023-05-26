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





// Fonction retourne le prix de la piece disponible la moins cher
int cheapest_piece(int available_piece[6]) {
 
  // Initialisation de la valeur minimale avec un coût trop cher
  int min = starting_point_count + 1;

  // Tableau des coûts des différentes pièces
  int piece_cost[6] = {QUEEN_COST, KNIGHT_COST, BISHOP_COST, ROOK_COST, KNIGHT_BISHOP_COST, PRINCE_COST};

  // Parcours des pièces disponibles pour trouver la moins chère
  for (int i = 0; i < 6; i++) {
    if (available_piece[i] > 0) {
      if (min > piece_cost[i]) {
        min = piece_cost[i];
      } } }
  return min;
}













// Fonction placement_piece gère la vérification et le placement d'une pièce
void Piece_placement(Player player, Piece chess_board[11][11], int choice) {

  display_board(chess_board);
  while (1) {
    // Affichage de l'échiquier
 
    // Demande de coordonnées au joueur
    int coord[2];
    ask_for_coords(coord);
    int x = coord[0];
    int y = coord[1];
   
    // Vérification des coordonnées de la pièce
    while ((check_piece_coords(player, chess_board, x, y) != 0) || (check_starting_area(player, chess_board, x, y) != 0)) {
     
      printf("Vous ne pouvez pas placer votre pièce ici, veuillez choisir à nouveau\n");
      ask_for_coords(coord);
      x = coord[0];
      y = coord[1];
    }
 
    // Placement de la pièce achetée sur l'échiquier si echec on reboucle
    if ((choice == 1) && (check_piece_placement(player, chess_board, x, y, player.board.Queen))) {
      chess_board[y][x] = player.board.Queen;
      break;
    } else if ((choice == 2) && (check_piece_placement(player, chess_board, x, y, player.board.Knight))) {
      chess_board[y][x] = player.board.Knight;
      break;
    } else if ((choice == 3) && (check_piece_placement(player, chess_board, x, y, player.board.Bishop))) {
      chess_board[y][x] = player.board.Bishop;
      break;
    } else if ((choice == 4) && (check_piece_placement(player, chess_board, x, y, player.board.Rook))) {
      chess_board[y][x] = player.board.Rook;
      break;
    } else if ((choice == 5) && (check_piece_placement(player, chess_board, x, y, player.board.Knight_Bishop))) {
      chess_board[y][x] = player.board.Knight_Bishop;
      break;
    } else if ((choice == 6) && (check_piece_placement(player, chess_board, x, y, player.board.Prince))) {
      chess_board[y][x] = player.board.Prince;
      break;
    }
    printf("Vous ne pouvez pas placer votre pièce ici, veuillez choisir à nouveau\n");
  }
  // Affichage de l'échiquier mis à jour
  display_board(chess_board);
}

// Fonction boutique (choix d'une pièce) qui renvoie la pièce achetée (1 à 6)
int shop(Player *player, int *available_piece) {

  // Vérifie que les pointeurs ne sont pas nuls
  if (!player || !available_piece) {
    printf("boutique : Erreur : pointeur null\n");
    exit(1);
  }
 
  // Vérifie si le joueur peut jouer
  if (player->end_of_purchases == 1) {
    // Il sort de la boucle dans phase_achat
    return -1;
  }

  // Affiche le nombre de points du joueur et les pièces disponibles
  printf("\nJoueur %s, vous avez %d points.\n", player->color, player->points);
  printf("Voici les pièces disponibles:\n");
  printf(" 1 - Reine (%d points) - %d restant\n", QUEEN_COST, available_piece[0]);
  printf(" 2 - Cavalier (%d points) - %d restant\n", KNIGHT_COST, available_piece[1]);
  printf(" 3 - Fou (%d points) - %d restant\n", BISHOP_COST, available_piece[2]);
  printf(" 4 - Tour (%d points) - %d restant\n", ROOK_COST, available_piece[3]);
  printf(" 5 - Cavalier-Fou (%d points) - %d restant\n", KNIGHT_BISHOP_COST, available_piece[4]);
  printf(" 6 - Prince (%d points) - %d restant\n", PRINCE_COST, available_piece[5]);
  printf(" 0 - Quitter la boutique \n");

  // Demande au joueur de faire un choix valide
  int choice = 0;
  printf("\nVotre choix (0-6): ");
  while (scanf("%d", &choice) != 1 || (choice < 0 || choice > 6)) {
    printf("\nChoix invalide.");
    while (getchar() != '\n');
    printf("\nVotre choix: ");
  }
  printf("\n");

 
  // Selon le choix du joueur, vérifie si la pièce est disponible et si le joueur a suffisamment de points pour l'acheter
  switch (choice) {
  case 1:
    if (available_piece[choice - 1]) {
      if (player->points >= QUEEN_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= QUEEN_COST;
        printf("Vous avez acheté une Reine.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("La Reine n'est plus disponible\n");
    }
    break;
  case 2:
    if (available_piece[choice - 1]) {
      if (player->points >= KNIGHT_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= KNIGHT_COST;
        printf("Vous avez acheté un Cavalier.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Cavalier n'est plus disponible\n");
    }
    break;
  case 3:
    if (available_piece[choice - 1]) {
      if (player->points >= BISHOP_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= BISHOP_COST;
        printf("Vous avez acheté un Fou.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Fou n'est plus disponible\n");
    }
    break;
  case 4:
    if (available_piece[choice - 1]) {
      if (player->points >= ROOK_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= ROOK_COST;
        printf("Vous avez acheté une Tour.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("La Tour n'est plus disponible\n");
    }
    break;
  case 5:
    if (available_piece[choice - 1]) {
      if (player->points >= KNIGHT_BISHOP_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= KNIGHT_BISHOP_COST;
        printf("Vous avez acheté un Cavalier-Fou.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Cavalier-Fou n'est plus disponible\n");
    }
    break;
  case 6:
    if (available_piece[choice - 1]) {
      if (player->points >= PRINCE_COST) {
        available_piece[choice - 1] -= 1;
        player->points -= PRINCE_COST;
        printf("Vous avez acheté un Prince.\n");
        return choice;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Prince n'est plus disponible\n");
    }
    break;
  case 0:
   
    // Confirmation du joueur pour quitter la boutique
    printf("Êtes-vous sûre ? Vous ne pourrez plus acheter de pièce dans la boutique.\n");
    printf("\nVeuillez confirmer votre choix : \n");
    printf(" 1 - Quitter la boutique\n");
    printf(" 0 - Revenir à la boutique\n");

   
    int confirmation = 0;
    printf("\nVotre choix (0-1): ");
    while (scanf("%d", &confirmation) != 1 || (confirmation < 0 || confirmation > 1)) {
      printf("\nChoix invalide.");
      while (getchar() != '\n');
      printf("\nVotre choix: ");
    }

   
    if (confirmation == 1){
      // Met à jour le statut du joueur pour indiquer qu'il a quitté la boutique
      player->end_of_purchases = 1;
      printf("\nJoueur %s vous avez décidé de quitter la boutique. Vous ne pourrez donc plus acheter de pièce.\n",player->color);
      return -1;
    } else{
      printf("\nVous revenez sur votre décision \n");
    }
  }
  return 0;
}

// Fonction pour la phase d'achat du jeu
void buying_phase(Player *player, Piece chess_board[11][11]) {

  // Vérifie que le pointeur n'est pas nul
  if (!player) {
    printf("phase_achat : Erreur : pointeur null.\n");
    exit(1);
  }
 
  printf("\nVoici l'échiquier :");
  display_board(chess_board);
  printf("\nLa boutique est disponible au début de la partie.");
  printf("\nVous avez chacun %d points mais %d exemplaires sont disponible pour chaque pièce.\n",starting_point_count, available_piece_count);
 
  // Tableau pour stocker le nombre de pièces disponibles pour chaque type de pièce
  int available_piece[6];

  // Initialisation du tableau avec le nombre de pièces disponibles
  for (int i = 0; i < 6; i++) {
    available_piece[i] = available_piece_count;
  }

  // Le joueur blanc commence toujours
  int active_player = 0;

  // Boucle qui permet d'alterner entre les joueurs jusqu'à ce qu'aucun des joueurs ne puisse plus acheter de pièces
  while ((player[0].end_of_purchases == 0) || (player[1].end_of_purchases == 0)) {

    // Verifie si le joueur a assez de points pour acheter une pièce
    if ((player[active_player].end_of_purchases == 0) && (player[active_player].points < cheapest_piece(available_piece))){
      player[active_player].end_of_purchases = 1;
      printf("\nJoueur %s vous n'avez plus assez de points pour acheter des pièces. Vous quittez donc la boutique !\n",player[active_player].color);
    }

    // Demande au joueur actuel de choisir une pièce à acheter
    int choix_piece = shop(&player[active_player], available_piece);

    // Boucle pour demander au joueur tant qu'il n'a pas choisi une pièce valide
    while (choix_piece == 0) {
      choix_piece = shop(&player[active_player], available_piece);
    }

    // Si le joueur a choisi une pièce valide, on la place
    if (choix_piece != -1) {
      Piece_placement(player[active_player], chess_board, choix_piece);
    }

    // On passe au joueur suivant
    active_player = (active_player + 1) % 2;
  }
 
  display_board(chess_board);
  printf("\nLa boutique n'est plus disponible, la partie peut commencer.\n");
}

// Fonction pour la phase de déplacement des joueurs
void Game_phase(Game *game) {

  if (!game) {
    printf("phase_jeux : Erreur : pointeur null.\n");
    exit(1);
  }

  SaveGame(*game);
 
  int active_player = game->playerTurn;
  int passive_player = (game->playerTurn + 1) % 2;

  display_board(game->chess_board);
 
  // Continue le jeu tant que le jeu n'est pas terminé
  while (end_game(game->player[active_player], game->player[passive_player], game->chess_board) == 0) {
    // Demande au joueur actif de déplacer une pièce
    Piece_moves(game->player[active_player], game->player[passive_player], game->chess_board);

    // Affiche l'échiquier mis à jour
    display_board(game->chess_board);

    // Passe au joueur suivant
    game->playerTurn = (game->playerTurn + 1) % 2;
    active_player = game->playerTurn;
    passive_player = (game->playerTurn + 1) % 2;

    SaveGame(*game);
   
  }
}

// Fonction qui coordonne la phase de jeux
void play_new_game() {
 
  // Affichage du texte de présentation
  printf("\nDans ce jeu d'échecs modifié, l'échiquier mesure 11x11 cases. Les joueurs ont la possibilité de choisir la position initiale des pièces pendant la phase d'achat. Le but du jeu est de mettre en échec et mat le roi adverse pour gagner la partie. Les joueurs ont à leur disposition les pièces classiques ainsi que deux nouvelles pièces, le cavalier fou et le prince. Les règles de base du jeu d'échecs sont respectées.\n");
 
  // Initialisation des pièces
  PlayerPiece white_board, black_board;
  init_piece(&white_board, &black_board);

  // Initialisation du jeu
  Game game = {
    .chess_board = {0},
    .playerTurn = 0,
    .player = {
      {starting_point_count, 1, "Blanc", white_board, 0},
      {starting_point_count, 0, "Noir", black_board, 0}
    }
  };

  // Initialisation du plateau de jeux
  init_chess_board(game.player, game.chess_board);

  // Verifie les représentations des différentes pièces
  check_piece_representation();

  // Lancement de la phase achat
  buying_phase(game.player, game.chess_board);

  // Lancement de la phase jeux
  Game_phase(&game);
}

int main() {
 
  Game gameSauvegarde;
  int choice;

  // Vérifier si une sauvegarde existe
  int sauvegardeExiste = LoadSave(&gameSauvegarde);

  if (sauvegardeExiste) {
    printf("\nUne sauvegarde existe.\n");
    printf(" 1 - Continuer la partie sauvegardé.\n");
    printf(" 2 - Commencer une nouvelle partie.\n");
   
    printf("\nVotre choix (1-2) : ");
    while (scanf("%d", &choice) != 1 || (choice < 1 || choice > 2)) {
      printf("\nChoix invalide.");
      while (getchar() != '\n');
      printf("\nVotre choix (1-2) : ");
    }

    if (choice == 1) {
      printf("Reprise en cours de la partie...\n");
      Game_phase(&gameSauvegarde);
   
    } else if (choice == 2) {
      printf("Démarrage d'une nouvelle partie...\n");
      DeleteSave();
      play_new_game();
    }
   
  } else {
    printf("\nDémarrage d'une nouvelle partie...\n");
    play_new_game();
  }

  return 0;
}