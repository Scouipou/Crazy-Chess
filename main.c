#include <stdio.h>
#include <stdlib.h>

#define QUEEN_COST 10
#define KNIGHT_COST 3
#define BISHOP_COST 3
#define ROOK_COST 5
#define KNIGHT_BISHOP_COST 7
#define PRINCE_COST 6

#define QUEEN_REPRESENTATION 'R'
#define KNIGHT_REPRESENTATION 'C'
#define BISHOP_REPRESENTATION 'F'
#define ROOK_REPRESENTATION 'T'
#define KNIGHT_BISHOP_REPRESENTATION 'G'
#define PRINCE_REPRESENTATION 'I'
#define KING_REPRESENTATION 'K'
#define PAWN_REPRESENTATION 'P'

#define available_piece_count 4
#define starting_point_count 40

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


// Fonction pour supprimer la sauvegarde
void DeleteSave() {

  remove("sauvegarde.txt");
 
}

// Fonction pour sauvegarder la sauvegarde
void SaveGame(Game game) {

  // Suppression de la sauvegarde avant de faire la nouvelle sauvegarde
  DeleteSave();
 
  // Ouverture du fichier en mode écriture
  FILE *file = fopen("sauvegarde.txt", "wb");

  if (file == NULL) {
    printf("sauvegarderGame : Erreur : fichier null.\n");
    exit(1);
    return;
  }

  // Écriture de la structure Game dans le fichier
  fwrite(&game, sizeof(Game), 1, file);

  // Fermeture du fichier
  fclose(file);
}

// Fonction pour charger la sauvegarde
int LoadSave(Game* game) {

  if (!game) {
    printf("chargerSauvegarde : Erreur : pointeur null.\n");
    exit(1);
  }
 
  // Ouverture du fichier en mode lecture
  FILE *file = fopen("sauvegarde.txt", "rb");

  if (file == NULL) {
    return 0; // Retourner 0 pour indiquer qu'il n'y a pas de partie sauvegardé
  }

  // Lecture de la structure Game à partir du fichier
  fread(game, sizeof(Game), 1, file);

  // Fermeture du fichier
  fclose(file);

  return 1; // Retourner 1 pour indiquer un chargement réussi
}

// Fonction affichage du tableau
void display_board(Piece chess_board[11][11]) {

  printf("\n");
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {

      // Vérifier si la case est occupée par une pièce et l'afficher en conséquence
      if (chess_board[y][x].type == 0) {
        printf(". ");
      } else if (chess_board[y][x].color == 'B') {
        printf("\x1b[1m%c\x1b[0m ", chess_board[y][x].type);
      } else {
        printf("\x1b[2m%c\x1b[0m ", chess_board[y][x].type);
      }
    }
    printf("%d\n", 11 - y);
  }
  printf("A B C D E F G H I J K\n");
}

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
