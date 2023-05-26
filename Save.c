#include <stdio.h>
#include <stdlib.h>
#include "Save.h"

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
