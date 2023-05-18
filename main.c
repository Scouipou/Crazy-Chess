#include <stdio.h>
#include <stdlib.h>

// Je le met ici mais tableau se construit comme ça : tableau[y][x]
#define cout_Reine 10
#define cout_Cavalier 3
#define cout_Fou 3
#define cout_Tour 5
#define cout_Cavalier_Fou 7
#define cout_Prince 6
// Les représentation DOIVENT être différentes (sinon bug)(faut vérifier ça aussi du coup)
#define representation_Reine 'R'
#define representation_Cavalier 'C'
#define representation_Fou 'F'
#define representation_Tour 'T'
#define representation_Cavalier_Fou 'G'
#define representation_Prince 'I'
#define representation_Roi 'K'
#define representation_Pion 'P'

#define nb_piece_disponible 4
#define nb_points_depart 40

// Structure Piece
typedef struct {
  char type;   // Sert à identifier la pièce (cavalier, tour, etc)
  char color;  // Sert à différencier les deux couleurs (blanc et noir)
  int nb_move; // Compte le nombre de déplacement de la pièce
} Piece;

// Structure Board sert à regrouper les différente pièce du joueur (Ce n'est pas les pièces qui sont sur le terrain)
typedef struct {
  Piece Reine;
  Piece Cavalier;
  Piece Fou;
  Piece Tour;
  Piece Cavalier_Fou;
  Piece Prince;
  Piece Roi;
  Piece Pion;
  Piece Vide; // Sert à vider la case occupé par une piece avant son déplacement
} Board;

// Structure Player
typedef struct {
  int points;     // Sert à acheter des pièces dans la phase_achat
  int position;   // Sert à identifier de quel côté le joueur se situe (1 en bas - Blanc / 0 en haut - Noir)
  char color[10]; // Par défaut c'est Blanc et Noir
  Board board;    // Regrouper les différentes pièces du joueurs
  int fin_achat;  // Savoir si le joueur a fini sa phase d'achat (0 pas fini \ 1 fini)
} Player;

// Structure Game (sauvegarde)
typedef struct {
  Piece tableau[11][11];
  Player player[2];
} Game;


// Fonction affichage du tableau
void affichage(Piece tableau[11][11]) {

  printf("\n");
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {

      // Vérifier si la case est occupée par une pièce et l'afficher en conséquence
      if (tableau[y][x].type == 0) {
        printf(". ");
      } else if (tableau[y][x].color == 'B') {
        printf("\x1b[1m%c\x1b[0m ", tableau[y][x].type);
      } else {
        printf("\x1b[2m%c\x1b[0m ", tableau[y][x].type);
      }
    }
    printf("%d\n", 11 - y);
  }
  printf("A B C D E F G H I J K\n");
}

// Fonction pour l'initialisation du plateau de jeux
void initialisation_plateau(Player player[2], Piece tableau[11][11]) {
 
  // Placement des pièces blanches
  tableau[9][5] = player[0].board.Roi;
  tableau[10][0] = player[0].board.Pion;
  tableau[9][1] = player[0].board.Pion;
  tableau[8][2] = player[0].board.Pion;
  tableau[7][3] = player[0].board.Pion;
  tableau[10][10] = player[0].board.Pion;
  tableau[9][9] = player[0].board.Pion;
  tableau[8][8] = player[0].board.Pion;
  tableau[7][7] = player[0].board.Pion;

  //temporaire :

  // test echec et mat : (faut juste bouger le roi blanc)
  //tableau[6][4] = player[0].board.Reine;
  //tableau[10][5] = player[0].board.Reine;
  //tableau[6][6] = player[0].board.Reine;

  // test nul : (faut mettre en commentaire tout les pions noirs)
  //tableau[9][6] = player[0].board.Reine;
  //tableau[0][3] = player[0].board.Reine;
  //tableau[2][3] = player[0].board.Reine;

  // test promotion pion : (Quand le pion n'a pas avancé il peut avancer de deux cases)
  //tableau[8][3] = player[1].board.Pion;
  //tableau[2][7] = player[0].board.Pion;

  // test rock :
  //tableau[1][2] = player[1].board.Tour;
  //tableau[9][8] = player[0].board.Tour;

  // Placement des pièces noires
  tableau[1][5] = player[1].board.Roi;
  tableau[0][0] = player[1].board.Pion;
  tableau[1][1] = player[1].board.Pion;
  tableau[2][2] = player[1].board.Pion;
  tableau[3][3] = player[1].board.Pion;
  tableau[0][10] = player[1].board.Pion;
  tableau[1][9] = player[1].board.Pion;
  tableau[2][8] = player[1].board.Pion;
  tableau[3][7] = player[1].board.Pion;

}

// Fonction de conversion de coordonnées informatiques en coordonnées d'échecs
void conversion_coord_echecs(int x, int y, char* coord) {
 
  // Vérification que les coordonnées sont valides
  if ((x < 0 || x > 10) || (y < 0 || y > 10)) {
    printf("conversion_coord_echecs : Erreur : coordonnées invalides\n");
    return;
  }
  // Conversion de la colonne (x) en lettre de A à K
  char colonne = x + 'A';
  // Conversion de la ligne (y) en chiffre de 1 à 11, puis inversion pour correspondre à la notation d'échecs
  int ligne = 11 - y;
  // Stockage de la notation d'échecs dans le tableau de caractères "coord"
  coord[0] = colonne;
  coord[1] = ligne; // Conversion du chiffre en caractère
}

// Fonction retourne le prix de la piece disponible la moins cher
int piece_moins_cher(int piece_dispo[6]) {
 
  // Initialisation de la valeur minimale avec un coût trop cher
  int min = nb_points_depart + 1;

  // Tableau des coûts des différentes pièces
  int cout_piece[6] = {cout_Reine, cout_Cavalier, cout_Fou, cout_Tour, cout_Cavalier_Fou, cout_Prince};

  // Parcours des pièces disponibles pour trouver la moins chère
  for (int i = 0; i < 6; i++) {
    if (piece_dispo[i] > 0) {
      if (min > cout_piece[i]) {
        min = cout_piece[i];
      }
    }
  }
  return min;
}

// Fonction pour demander les coordonnées de l'utilisateur et les vérifier
int demande_coordonnees(int coord[2]) {
 
  int x, y;
  char letter;
  printf("\nVeuillez entrer les coordonnées (ex. 8 4 ou H4) : ");

  // Boucle infinie pour demander les coordonnées jusqu'à ce qu'elles soient valides
  while (1) {
    // Si les entrées ne sont composé que de chiffres
    if (scanf("%d %d", &x, &y) == 2 && (x > 0 && x < 12) && (y > 0 && y < 12)) {
      coord[0] = x - 1;
      coord[1] = 11 - y; // Change les coordonnées d'echec pour des coordonnées informatique
      return 1;
     
      // Si la première entrée est un caractère de A à K, suivi d'un entier entre 1 et 11
    } else if (scanf(" %c%d", &letter, &y) == 2 && ((letter >= 'A' && letter <= 'K') || (letter >= 'a' && letter <= 'k')) && (y > 0 && y < 12)) {

      // Si la lettre est minuscule, la convertir en majuscule
      if (letter >= 'a' && letter <= 'k') {
        letter = letter - 'a' + 'A';
      }
      coord[0] = letter - 'A';
      coord[1] = 11 - y; // Change les coordonnées d'echec pour des coordonnées informatique
      return 1;
   
    while (getchar() != '\n');
    printf("Coordonnées invalides\n");
    printf("\nEntrez les coordonnées (ex. 1 7 ou A7) : ");
    }
  }
}

// Fonction qui va effectuer le rock si
void rock(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("rock : Erreur : coordonnées données non conforme ");
    exit(1);
  }  

  // Place le Roi
  tableau[y2][x2] = tableau[y1][x1];
  tableau[y1][x1] = player.board.Vide;

  // Verifie la direction de déplacement du roi
  int direction;
  if (x2 == x1 + 2) {
    direction = 1; // Direction vers la droite
  } else  {
  direction = -1;  // Direction vers la gauche
  }
 
  // Place la Tour
  tableau[y1][x1 + direction*1] = tableau[y1][x1 + direction*3];
  tableau[y1][x1 + direction*3] = player.board.Vide;
}

// Fonction qui va promouvoir le pion si il est arrivé à la dernière rangé
void promotion_pion(Player player, Piece tableau[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if (x < 0 || x > 10 || y < 0 || y > 10) {
    printf("Erreur : coordonnées données non conformes.");
    exit(1);
  }  

  // Vérification qu'un pion se trouve bien aux coordonnées données
  if (tableau[y][x].type != player.board.Pion.type) {
    return;
  }

  // Vérification que le pion est arrivé à la dernière rangée
  if ((player.position == 0 && y != 10) || (player.position == 1 && y != 0)) {
    return;
  }

  // Affichage des choix de promotion
  affichage(tableau);
  char coord[2];
  conversion_coord_echecs(x,y,coord);
  printf("\nLe pion %s en %c%d va être promu.\n",player.color,coord[0],coord[1]);
  printf("Vous avez 5 choix de promotion possible:\n");
  printf(" 1 - Promotion en Reine\n");
  printf(" 2 - Promotion en Cavalier\n");
  printf(" 3 - Promotion en Fou\n");
  printf(" 4 - Promotion en Tour\n");
  printf(" 5 - Promotion en Cavalier-Fou\n");

  // Demande à l'utilisateur de choisir une promotion
  int choix = 0;
  printf("\nVotre choix (1-5): ");
  while (scanf("%d", &choix) != 1 || (choix < 1 || choix > 5)) {
    printf("\nChoix invalide.");
    while (getchar() != '\n');
    printf("\nVotre choix: ");
  }
 
  // Sauvegarde du nombre de mouvements de la pièce temporaire
  int nb_move = tableau[y][x].nb_move;
 
  // Promotion du pion en fonction du choix de l'utilisateur
  switch (choix) {
    case 1:
      printf("La promotion se fera en Reine");
      tableau[y][x] = player.board.Reine;
      break;
    case 2:
      printf("La promotion se fera en Cavalier");
      tableau[y][x] = player.board.Cavalier;
      break;
    case 3:
      printf("La promotion se fera en Fou");
      tableau[y][x] = player.board.Fou;
      break;
    case 4:
      printf("La promotion se fera en Tour");
      tableau[y][x] = player.board.Tour;
      break;
    case 5:
      printf("La promotion se fera en Cavalier-Fou");
      tableau[y][x] = player.board.Cavalier_Fou;
      break;
  }

  // Transfert du nombre de mouvements à la nouvelle pièce promue
  tableau[y][x].nb_move = nb_move;
}

// Fonction qui renvoie si c'est une pièce alliée, adverse ou qu'il n'y en a pas
int verif_coordonnee_piece(Player player, Piece tableau[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if ((x < 0 && x > 10) || (y < 0 && y > 10)) {
    printf("verif_coordonnee_piece : Erreur : coordonnées données non conforme ");
    exit(1);
  }
 
  if (tableau[y][x].type == 0) {
    return 0;
  } // Pas de pièce
  else if (tableau[y][x].color == player.board.Roi.color) {
    return 1;
  } // Pièce allié
  else {
    return -1;
  } // Pièce ennemie
}

// verif_zone_depart va vérifier si les coordonnées sont situées dans la zone de départ autorisée au joueur
int verif_zone_depart(Player player, Piece tableau[11][11], int x, int y) {

  // Vérification que les coordonnées sont valides
  if ((x >= 0 && x <= 10) || (y >= 0 && y <= 10)) {
  } else {
    printf("verif_zone_depart : Erreur : coordonnées données non conforme ");
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
void position_roi_adverse(Player player, Piece tableau[11][11], int coord[2]) {

  // Parcours le tableau de jeu en recherchant le roi adverse
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {
     
      // Si la pièce est un roi et qu'il n'appartient pas au joueur
      if ((tableau[y][x].type == player.board.Roi.type) && (verif_coordonnee_piece(player, tableau, x, y) == -1)) {
        coord[0] = x;
        coord[1] = y;
        return ;
      } } }
 
  // Si aucun roi adverse n'a été trouvé, affiche une erreur et arrête le programme
  printf("position_roi_adverse : Erreur : Pas de roi");
  exit(1);
}

// Fonction qui vérifie si un déplacement de pion est valide
int verif_deplacement_Pion(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_Pion : Erreur : coordonnées données non conforme ");
    exit(1);
  }  

  int direction;
  if (player.position == 0) {
    direction = -1;
  } else {
    direction = 1;
  }
 
  // On vérifie si le déplacement correspond à un déplacement normal d'un pion
  if (x1 == x2 && y1 == y2 + direction && verif_coordonnee_piece(player, tableau, x2, y2) == 0){
    return 1;
  // On vérifie si le déplacement correspond à un double déplacement du pion lors de son premier mouvement
  } else if ((tableau[y1][x1].nb_move == 0) && (x1 == x2 && y1 == y2 + 2 * direction) && (verif_coordonnee_piece(player, tableau, x2, y2) == 0 ) && (verif_coordonnee_piece(player, tableau, x2, y2 + direction) == 0 )) {
    return 1;

  // On vérifie si le déplacement correspond à une prise diagonale d'une pièce adverse
  } else if (y1 == y2 + direction && (x1 == x2 - 1 || x1 == x2 + 1) && verif_coordonnee_piece(player, tableau, x2, y2) == -1) {
    return 1;
  }
  return 0; // Si aucun cas n'est vérifié, le déplacement est invalide
}

// Fonction qui vérifie si un déplacement en L est valide
int verif_deplacement_cavalier(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_cavalier : Erreur : coordonnées données non conforme ");
    exit(1);
  }
 
  // On vérifie si le déplacement correspond à un déplacement en L
  if ((abs(x2 - x1) == 2 && abs(y2 - y1) == 1) || (abs(x2 - x1) == 1 && abs(y2 - y1) == 2)) {
    if (verif_coordonnee_piece(player, tableau, x2, y2) != 1) {
     return 1;
    } }
  return 0; // Si aucun cas n'est vérifié, le déplacement est invalide
}

// Fonction qui vérifie si un déplacement diagonale est valide
int verif_deplacement_diagonale(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2, int distance_autorise) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_diagonale : Erreur : coordonnées données non conforme ");
    exit(1);
  }
 
  int distance = 0;
  int pas_x, pas_y;
 
  // Déterminer la direction de déplacement selon l'axe x et y
  if (x1 < x2) {
    pas_x = 1; // La pièce se déplace vers la droite
  } else {
    pas_x = -1; // La pièce se déplace vers la gauche
  }
  if (y1 < y2) {
    pas_y = 1; // La pièce se déplace vers le haut
  } else {
    pas_y = -1; // La pièce se déplace vers le bas
  }

  // Vérifier si le déplacement est en diagonale
  if (abs(x2 - x1) == abs(y2 - y1)) {
    // Parcourir le chemin de la diagonale
    for (int i = 1; i <= abs(x2 - x1); i++) {
      distance += 1;
      int x = x1 + pas_x * i;
      int y = y1 + pas_y * i;

      // Vérifier si la case d'arrivée est la même que les coordonnées souhaité
      if ((x == x2 && y == y2) && (distance <= distance_autorise) && (verif_coordonnee_piece(player, tableau, x, y) != 1)){
        return 1;
      }
      // Vérifier si la case est occupée par une pièce
      if (verif_coordonnee_piece(player, tableau, x, y) != 0) {
        return 0; // Déplacement invalide, chemin bloqué par une pièce
      }

      // Vérifier si la distance autorisée a été dépassée
      if (distance > distance_autorise) {
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
int verif_deplacement_croix(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2, int distance_autorise) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_croix : Erreur : coordonnées données non conforme ");
    exit(1);
  }
 
  int distance = 0;
  int pas_x, pas_y;
  // Déterminer la direction de déplacement selon l'axe x et y
  if (x1 < x2) {
    pas_x = 1; // La pièce se déplace vers la droite
  } else {
    pas_x = -1; // La pièce se déplace vers la gauche
  }
  if (y1 < y2) {
    pas_y = 1; // La pièce se déplace vers le bas
  } else {
    pas_y = -1; // La pièce se déplace vers le haut
  }

  // Vérifier que toutes les cases entre x1 et x2 sont vides
  if (y1 == y2) { // Déplacement vertical
    for (int i = x1; i != x2; i += pas_x) {
      distance += 1;
      if ((verif_coordonnee_piece(player, tableau, i + pas_x, y1) != 1) && ((i + pas_x == x2) && (y1 == y2)) && (distance <= distance_autorise)) {
        return 1;
      } else if ((verif_coordonnee_piece(player, tableau, i + pas_x, y1) != 0) || (distance > distance_autorise)) {
        return 0;
      }
    }
  } else if (x1 == x2) { // Déplacement horizontal
    for (int i = y1; i != y2; i += pas_y) {
      distance += 1;
      if ((verif_coordonnee_piece(player, tableau, x1, i + pas_y) != 1) && ((x1 == x2) && (i + pas_y == y2)) && (distance <= distance_autorise)) {
        return 1;
      } else if ((verif_coordonnee_piece(player, tableau, x1, i + pas_y) != 0) || (distance > distance_autorise)) {
        return 0; // Déplacement invalide
      }
    }
  } else {
    return 0;
  }
  return 1;
}

// Fonction qui gère le ou les fonction de déplacement de la pièce, retourne 1 si le déplacement est valide
int gestion_deplacement(Player player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {
 
  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("gestion_deplacement : Erreur : coordonnées données non conforme ");
    exit(1);
  }

  // Vérifie le type de la pièce à déplacer et appelle la fonction de déplacement appropriée
  if (tableau[y1][x1].type == player.board.Reine.type) {
    if (verif_deplacement_croix(player, tableau, x1, y1, x2, y2, 10) || (verif_deplacement_diagonale(player, tableau, x1, y1, x2, y2, 10))) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Cavalier.type) {
    if (verif_deplacement_cavalier(player, tableau, x1, y1, x2, y2)) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Fou.type) {
    if (verif_deplacement_diagonale(player, tableau, x1, y1, x2, y2, 10)) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Tour.type) {
    if (verif_deplacement_croix(player, tableau, x1, y1, x2, y2, 10)) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Cavalier_Fou.type) {
    if (verif_deplacement_cavalier(player, tableau, x1, y1, x2, y2) || (verif_deplacement_diagonale(player, tableau, x1, y1, x2, y2, 10))) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Prince.type) {
    if (verif_deplacement_croix(player, tableau, x1, y1, x2, y2, 2) || (verif_deplacement_diagonale(player, tableau, x1, y1, x2, y2, 2))) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Roi.type) {
    if (verif_deplacement_croix(player, tableau, x1, y1, x2, y2, 1) || (verif_deplacement_diagonale(player, tableau, x1, y1, x2, y2, 1))) {
      return 1;
    }
  } else if (tableau[y1][x1].type == player.board.Pion.type) {
    if (verif_deplacement_Pion(player, tableau, x1, y1, x2, y2)) {
      return 1;
    }
  }
  // Si le déplacement n'est pas possible, retourne 0
  return 0;
}

// retourne 1 si le player fait echec sur le roi adverse
int verif_echec(Player player, Piece tableau[11][11]) {

  int x_roi;
  int y_roi;
  int coord[2];
  // Retourne la position du roi adverse
  position_roi_adverse(player, tableau, coord);
  x_roi = coord[0];
  y_roi = coord[1];
 
  // Parcours les cases du tableau et regarde chaque pieces allié et regarde si ils peuvent aller sur la case du roi adverse.
  for (int y = 0; y < 11; y++) {
    for (int x = 0; x < 11; x++) {
      // Si il y a un allié qui peut se déplacer sur la case du roi adverse
      if (verif_coordonnee_piece(player, tableau, x, y) == 1){
        if (gestion_deplacement(player, tableau, x, y, x_roi, y_roi)) {
          return 1;
        } } } }
  return 0;
}

// Retourne 1 si le placement est possible sinon 0
int verif_placement_piece(Player player, Piece tableau[11][11], int x, int y, Piece piece) {

  // Vérification que les coordonnées sont valides
  if ((x < 0 && x > 10) || (y < 0 && y > 10)) {
    printf("verif_placement_piece : Erreur : coordonnées données non conforme ");
    exit(1);
  }  
 
  // Sauvegarde la pièce qui se trouve sur la case (x,y) avant le test
  Piece piece_temporaire = tableau[y][x];

  // Place la pièce sur la case (x,y) du tableau
  tableau[y][x] = piece;

  // Vérifie si le placement met le roi adverse en échec
  if (verif_echec(player, tableau)) {
    tableau[y][x] = piece_temporaire;
    // Le roi adverse est en échec, placement impossible
    return 0;
  }
  tableau[y][x] = piece_temporaire;
  // Le roi adverse n'est pas en échec, placement autorisé
  return 1;
}

// Retourne 1 si le deplacement est possible sinon 0
int verif_deplacement_piece(Player active_player, Player passive_player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_deplacement_piece : Erreur : coordonnées données non conforme ");
    exit(1);
  }

  // Vérifie si les coordonnées de départ et d'arrivée contiennent une pièce alliée alors le déplacment est invalide
  if (verif_coordonnee_piece(active_player, tableau, x1, y1) == verif_coordonnee_piece(active_player, tableau, x2, y2)) {
    return 0;
  }
 
  // Vérifie si le déplacement de la pièce de la première coordonnée à la seconde est valide
  if (gestion_deplacement(active_player, tableau, x1, y1, x2, y2) != 1) {
    return 0;
  }
 
  // Sauvegarde des pièces pour les remettre après le test de déplacement
  Piece piece_temporaire1 = tableau[y1][x1];
  Piece piece_temporaire2 = tableau[y2][x2];
 
  // Test du déplacement
  tableau[y1][x1] = active_player.board.Vide; // On vide la case de départ
  tableau[y2][x2] = piece_temporaire1; // On ajoute la pièce d'arrivé
 
  // Retourne 0 si le déplacement est impossible
  if (verif_echec(passive_player, tableau)) {
    // Remise en place des pièces
    tableau[y2][x2] = piece_temporaire2;
    tableau[y1][x1] = piece_temporaire1;
    return 0;
  }
  // Remise en place des pièces
  tableau[y2][x2] = piece_temporaire2;
  tableau[y1][x1] = piece_temporaire1;

  // Retourne 1 si le déplacement est possible
  return 1;
}

// Retourne 1 si le rock est possible sinon 0
int verif_rock(Player active_player, Player passive_player, Piece tableau[11][11], int x1, int y1, int x2, int y2) {

  // Vérification que les coordonnées sont valides
  if ((x1 < 0 && x1 > 10) || (y1 < 0 && y1 > 10) || (x2 < 0 && x2 > 10) || (y2 < 0 && y2 > 10)) {
    printf("verif_rock : Erreur : coordonnées données non conforme ");
    exit(1);
  }  

  // Si le roi est en échec alors le rock est impossible
  if (verif_echec(passive_player, tableau)) {
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

  if (x2 != x1 + (direction * 2) && y2 != y1) {
    return 0;
  }
  // Verif qu'il y a bien un Roi et une Tour pour rock
  if (tableau[y1][x1].type != active_player.board.Roi.type && tableau[y1][x1 + direction*3].type != active_player.board.Roi.type) {
    return 0;
  }
  // Verif qu'il y a bien un Roi et une Tour qui n'ont pas bougé
  if (tableau[y1][x1].nb_move != 0 && tableau[y1][x1 + direction*3].nb_move != 0) {
    return 0;
  }
  // Verif qu'il y a bien deux pieces vide entre le Roi et la Tour
  if (verif_coordonnee_piece(active_player, tableau, x1 + direction, y1) != 0 && (verif_coordonnee_piece(active_player, tableau, x1 + direction*2, y1) != 0)) {
    return 0;
  }
 
  // J'enlève la Tour pour verifier le déplacement
  Piece piece_temporaire = tableau[y1][x1 + direction*3];
  tableau[y1][x1 + direction*3] = active_player.board.Vide;

  for (int i = x1 + direction; i != x1 + (direction * 2); i += direction) {
    if (verif_deplacement_piece(active_player, passive_player, tableau, x1, y1, i, y2) == 0) {
      // Remise en place de la Tour
      tableau[y1][x1 + direction*3] = piece_temporaire;
      // retourne 0 si le roi est en échec
      return 0;
    }
  }
  // Remise en place de la Tour
  tableau[y1][x1 + direction*3] = piece_temporaire;
  return 1;
}

// Retourne 1 si le roi adverse ne peut plus bouger de case
int verif_echec_mat(Player active_player, Player passive_player, Piece tableau[11][11]) {

  // Si il n'y a pas echec alors il n'y a pas echec et mat
  if (verif_echec(passive_player, tableau) != 1){
    return 0;
  }
 
  // Récupération des coordonnées du roi adverse
  int x_roi, y_roi;
  int coord[2];
  position_roi_adverse(passive_player, tableau, coord);
  x_roi = coord[0];
  y_roi = coord[1];
 
  // Vérification de chaque déplacement possible du roi adverse
  for (int y2 = 0; y2 < 11; y2++) {
    for (int x2 = 0; x2 < 11; x2++) {
     
      // Si le roi peut se déplacer dessus sans être en échec
      if (verif_deplacement_piece(active_player, passive_player, tableau, x_roi, y_roi, x2, y2)) {
        // retourne 0 si le roi peut se déplacer sur une case
        return 0;
        } } }
 
  // Si le roi advesre ne peut se déplacer nul part sur le plateau return 1 :
  return 1;
}

// Retourne 1 si le joueur ne peut plus déplacer de pièces
int verif_deplacement_joueur(Player active_player, Player passive_player, Piece tableau[11][11]) {

  int x1, y1, x2, y2;

  // Parcourt toutes les coordonnées possibles sur le plateau de jeu
  for (y1 = 0; y1 < 11; y1++) {
    for (x1 = 0; x1 < 11; x1++) {

      // Vérifie si la case contient une pièce appartenant au joueur
      if (verif_coordonnee_piece(active_player, tableau, x1, y1) == 1){
       
        // Parcourt à nouveau toutes les coordonnées possibles sur le plateau de jeu
        for (y2 = 0; y2 < 11; y2++) {
          for (x2 = 0; x2 < 11; x2++) {

            // Vérifie si le déplacement de la pièce de la première coordonnée à la seconde est possible
            if (verif_deplacement_piece(active_player, passive_player, tableau, x1, y1, x2, y2) && (x1 != x2 || y1 != y2)){
              return 0;
            } } } } } }
  return 1;
}

// Retourne 1 si nul par pat
int nul_pat(Player active_player, Player passive_player, Piece tableau[11][11]) {

   // Vérifie s'il y a échec
  if (verif_echec(passive_player, tableau)){
    // Si echec alors il n'y a pas de nul par pat  
    return 0;
  }
  // Vérifie si le joueur ne peut pas déplacer de pièces
  else if (verif_deplacement_joueur(active_player, passive_player, tableau)) {
    return 1;
  }
 
  // Aucune condition de match nul par pat n'est satisfaite
  return 0;
}

// Retourne 1 si échec et mat ou nul
int end_game(Player active_player, Player passive_player, Piece tableau[11][11]) {

  // Vérifie s'il y a échec et mat
  if (verif_echec_mat(active_player, passive_player, tableau)) {
    printf("\nBravo, le joueur %s a gagné la partie par échec et mat\n", passive_player.color);
    return 1;
  }
   
  // Vérifie s'il y a match nul
  else if (nul_pat(active_player, passive_player, tableau)) {
    printf("\nLa partie se termine par un match nul\n");
    return 1;
  }

  // Aucune condition de fin de partie n'est satisfaite
  return 0;
}

// Fonction placement_piece gère la vérification et le déplacement d'une pièce
void deplacement_piece(Player active_player, Player passive_player, Piece tableau[11][11]) {

  int x1, y1, x2, y2;

  // Tableau pour stocker les coordonnées entrées par le joueur
  int coord[2];
 
  printf("\nJoueur %s, c'est à vous de jouer.", active_player.color);
 
  // Boucle pour demander les coordonnées de départ jusqu'à ce qu'elles soient valides
  while (1) {
    printf("\nQuelle pièce souhaitez-vous déplacer ?\n");
   
    while (1) {
      // Demande des coordonnées de départ
      demande_coordonnees(coord);
      x1 = coord[0];
      y1 = coord[1];

      // Vérification des coordonnées de départ
      if (verif_coordonnee_piece(active_player, tableau, x1, y1) == 1 && ((x1 != x2) || (y1 != y2))) {
        break;
      }
      printf("Coordonnées invalides\n");
    }
    // Demande des coordonnées d'arrivée
    printf("\nOù souhaitez vous déplacer votre pièce");
    demande_coordonnees(coord);
    x2 = coord[0];
    y2 = coord[1];

    // Vérification du rock
    if (verif_rock(active_player, passive_player, tableau, x1, y1, x2, y2)) {
      rock(active_player, tableau, x1, y1, x2, y2);
      break;
    }
   
    // Vérification que le déplacement est valide
    else if (verif_deplacement_piece(active_player, passive_player, tableau, x1, y1, x2, y2)){
     
      // On déplace la pièce
      tableau[y2][x2] = tableau[y1][x1];
      tableau[y1][x1] = active_player.board.Vide;
      tableau[y2][x2].nb_move += 1;
       
      // Vérification si le pion est promu
      promotion_pion(active_player, tableau, x2, y2);
      break;
      }
    printf("Déplacement impossible\n");  }
}

// Fonction placement_piece gère la vérification et le placement d'une pièce
void placement_piece(Player player, Piece tableau[11][11], int choix) {

  affichage(tableau);
  while (1) {
    // Affichage de l'échiquier
 
    // Demande de coordonnées au joueur
    int coord[2];
    demande_coordonnees(coord);
    int x = coord[0];
    int y = coord[1];
   
    // Vérification des coordonnées de la pièce
    while ((verif_coordonnee_piece(player, tableau, x, y) != 0) || (verif_zone_depart(player, tableau, x, y) != 0)) {
     
      printf("Vous ne pouvez pas placer votre pièce ici, veuillez choisir à nouveau\n");
      demande_coordonnees(coord);
      x = coord[0];
      y = coord[1];
    }
 
    // Placement de la pièce achetée sur l'échiquier si echec on reboucle
    if ((choix == 1) && (verif_placement_piece(player, tableau, x, y, player.board.Reine))) {
      tableau[y][x] = player.board.Reine;
      break;
    } else if ((choix == 2) && (verif_placement_piece(player, tableau, x, y, player.board.Cavalier))) {
      tableau[y][x] = player.board.Cavalier;
      break;
    } else if ((choix == 3) && (verif_placement_piece(player, tableau, x, y, player.board.Fou))) {
      tableau[y][x] = player.board.Fou;
      break;
    } else if ((choix == 4) && (verif_placement_piece(player, tableau, x, y, player.board.Tour))) {
      tableau[y][x] = player.board.Tour;
      break;
    } else if ((choix == 5) && (verif_placement_piece(player, tableau, x, y, player.board.Cavalier_Fou))) {
      tableau[y][x] = player.board.Cavalier_Fou;
      break;
    } else if ((choix == 6) && (verif_placement_piece(player, tableau, x, y, player.board.Prince))) {
      tableau[y][x] = player.board.Prince;
      break;
    }
    printf("Vous ne pouvez pas placer votre pièce ici, veuillez choisir à nouveau\n");
  }
  // Affichage de l'échiquier mis à jour
  affichage(tableau);
}

// Fonction boutique (choix d'une pièce) qui renvoie la pièce achetée (1 à 6)
int boutique(Player *player, int *piece_dispo) {

  // Vérifie que les pointeurs ne sont pas nuls
  if (!player || !piece_dispo) {
    printf("boutique : Erreur : pointeur null\n");
    exit(1);
  }
  // Vérifie si le joueur peut/veut jouer
  if (player->fin_achat == 1) {
    // Il sort de la boucle dans phase_achat
    return -1;
  }

  // Affiche le nombre de points du joueur et les pièces disponibles
  printf("\nJoueur %s, vous avez %d points.\n", player->color, player->points);
  printf("Voici les pièces disponibles:\n");
  printf(" 1 - Reine (%d points) - %d restant\n", cout_Reine, piece_dispo[0]);
  printf(" 2 - Cavalier (%d points) - %d restant\n", cout_Cavalier, piece_dispo[1]);
  printf(" 3 - Fou (%d points) - %d restant\n", cout_Fou, piece_dispo[2]);
  printf(" 4 - Tour (%d points) - %d restant\n", cout_Tour, piece_dispo[3]);
  printf(" 5 - Cavalier-Fou (%d points) - %d restant\n", cout_Cavalier_Fou, piece_dispo[4]);
  printf(" 6 - Prince (%d points) - %d restant\n", cout_Prince, piece_dispo[5]);
  printf(" 0 - Quitter la boutique \n");

  // Demande au joueur de faire un choix valide
  int choix = 0;
  printf("\nVotre choix (0-6): ");
  while (scanf("%d", &choix) != 1 || (choix < 0 || choix > 6)) {
    printf("\nChoix invalide.");
    while (getchar() != '\n');
    printf("\nVotre choix: ");
  }
  printf("\n");

 
  // Selon le choix du joueur, vérifie si la pièce est disponible et si le joueur a suffisamment de points pour l'acheter
  switch (choix) {
  case 1:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Reine) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Reine;
        printf("Vous avez acheté une Reine.\n");
        return choix;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("La Reine n'est plus disponible\n");
    }
    break;
  case 2:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Cavalier) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Cavalier;
        printf("Vous avez acheté un Cavalier.\n");
        return choix;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Cavalier n'est plus disponible\n");
    }
    break;
  case 3:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Fou) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Fou;
        printf("Vous avez acheté un Fou.\n");
        return choix;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Fou n'est plus disponible\n");
    }
    break;
  case 4:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Tour) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Tour;
        printf("Vous avez acheté une Tour.\n");
        return choix;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("La Tour n'est plus disponible\n");
    }
    break;
  case 5:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Cavalier_Fou) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Cavalier_Fou;
        printf("Vous avez acheté un Cavalier-Fou.\n");
        return choix;
      } else {
        printf("Vous n'avez pas assez de points.\n");
      }
    } else {
      printf("Le Cavalier-Fou n'est plus disponible\n");
    }
    break;
  case 6:
    if (piece_dispo[choix - 1]) {
      if (player->points >= cout_Prince) {
        piece_dispo[choix - 1] -= 1;
        player->points -= cout_Prince;
        printf("Vous avez acheté un Prince.\n");
        return choix;
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
      player->fin_achat = 1;
      printf("\nJoueur %s vous avez décidé de quitter la boutique. Vous ne pourrez donc plus placer de pièce.\n",player->color);
      return -1;
    } else{
      printf("\nVous revenez sur votre décision \n");
    }
  }
  return 0;
}

// Fonction pour la phase d'achat du jeu
void phase_achat(Player *player, Piece tableau[11][11]) {

  // Vérifie que le pointeur n'est pas nul
  if (!player) {
    printf("phase_achat : Erreur : pointeur null.\n");
    exit(1);
  }
 
  printf("\nVoici l'échiquier :");
  affichage(tableau);
  printf("\nLa boutique est disponible au début de la partie.");
  printf("\nVous avez chacun %d points mais %d exemplaires sont disponible pour chaque pièce.\n",nb_points_depart, nb_piece_disponible);
 
  // Tableau pour stocker le nombre de pièces disponibles pour chaque type de pièce
  int piece_dispo[6];

  // Initialisation du tableau avec le nombre de pièces disponibles
  for (int i = 0; i < 6; i++) {
    piece_dispo[i] = nb_piece_disponible;
  }

  // Le joueur blanc commence toujours
  int active_player = 0;

  // Boucle qui permet d'alterner entre les joueurs jusqu'à ce qu'aucun des joueurs ne puisse plus acheter de pièces
  while ((player[0].fin_achat == 0) || (player[1].fin_achat == 0)) {

    // Verifie si le joueur a assez de points pour acheter une pièce
    if ((player[active_player].fin_achat == 0) && (player[active_player].points < piece_moins_cher(piece_dispo))){
      player[active_player].fin_achat = 1;
      printf("\nJoueur %s vous n'avez plus assez de points pour acheter des pièces. Vous quittez donc la boutique !\n",player[active_player].color);
    }
   
    // Demande au joueur actuel de choisir une pièce à acheter
    int choix_piece = boutique(&player[active_player], piece_dispo);

    // Boucle pour demander au joueur tant qu'il n'a pas choisi une pièce valide
    while (choix_piece == 0) {
      choix_piece = boutique(&player[active_player], piece_dispo);
    }

    // Si le joueur a choisi une pièce valide, on la place
    if (choix_piece != -1) {
      placement_piece(player[active_player], tableau, choix_piece);
    }

    // On passe au joueur suivant
    active_player = (active_player + 1) % 2;
  }
 
  affichage(tableau);
  printf("\nLa boutique n'est plus disponible, la partie peut commencer.\n");
}

// Fonction pour la phase de jeu standard
void phase_jeux(Player *players, Piece tableau[11][11]) {
 
  // Vérifie que le pointeur n'est pas nul
  if (!players) {
    printf("Erreur : pointeur null.\n");
    exit(1);
  }

  int active_player = 0;
  int passive_player = 1;
 
  // Continue le jeu tant que le jeu n'est pas terminé
  while (end_game(players[active_player], players[passive_player], tableau) == 0) {
    // Demande au joueur actif de déplacer une pièce
    deplacement_piece(players[active_player], players[passive_player], tableau);

    // Affiche l'échiquier mis à jour
    affichage(tableau);

    // Passe au joueur suivant
    active_player = (active_player + 1) % 2;
    passive_player = (active_player + 1) % 2;
  }
}

int main() {
 
  // Initialisation des pièces Blanches
 Board board_blanc = {{representation_Reine, 'B', 0},
                       {representation_Cavalier, 'B', 0},
                       {representation_Fou, 'B', 0},
                       {representation_Tour, 'B', 0},
                       {representation_Cavalier_Fou, 'B', 0},
                       {representation_Prince, 'B', 0},
                       {representation_Roi, 'B', 0},
                       {representation_Pion, 'B', 0},
                       {0}};

  // Initialisation des pièces Noires
  Board board_noir = {{representation_Reine, 'N', 0},
                      {representation_Cavalier, 'N', 0},
                      {representation_Fou, 'N', 0},
                      {representation_Tour, 'N', 0},
                      {representation_Cavalier_Fou, 'N', 0},
                      {representation_Prince, 'N', 0},
                      {representation_Roi, 'N', 0},
                      {representation_Pion, 'N', 0},
                      {0}};

  // Initialisation du jeu
  Game game = {.tableau = {0},
               .player = {{nb_points_depart, 1, "Blanc", board_blanc, 0},
                          {nb_points_depart, 0, "Noir", board_noir, 0}}};

  // Initialisation du plateau de jeux
  initialisation_plateau(game.player, game.tableau);

  // Affichage du texte de présentation
  printf("\nDans ce jeu d'échecs modifié, l'échiquier mesure 11x11 cases. Les joueurs ont la possibilité de choisir la position initiale des pièces pendant la phase d'achat. Le but du jeu est de mettre en échec et mat le roi adverse pour gagner la partie. Les joueurs ont à leur disposition les pièces classiques ainsi que deux nouvelles pièces, le cavalier fou et le prince. Les règles de base du jeu d'échecs sont respectées.\n");

  // Lancement de la phase achat
  phase_achat(game.player, game.tableau);
 
  // Lancement de la phase jeux
  phase_jeux(game.player, game.tableau);
  return 0;
}