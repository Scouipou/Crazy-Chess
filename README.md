
# CrazY-CHESS

Dans ce jeu d'échecs modifié, l'échiquier mesure 11x11 cases. Les joueurs ont la possibilité de choisir la position initiale des pièces pendant une phase d'achat. Le but du jeu est de mettre en échec et mat le roi adverse pour gagner la partie. Les joueurs ont à leur disposition les pièces classiques ainsi que deux nouvelles pièces, le cavalier fou et le prince. Les règles de base du jeu d'échecs sont respectées.

## Comment executer le projet
- Dans le terminal : se trouver dans le répertoire crazyChess 
```bash
  cd CrazyChess
```
- Compiler le projet : taper la commande suivante dans le terminal
```bash
  make -f makefile
```
- Executer le projet : 
```bash
  .\crazychess
```

## Modules

Ce projet est un jeu d'échecs modifié implémenté en utilisant 8 modules distincts pour gérer différentes fonctionnalités. Il offre des règles personnalisées, des fonctionnalités d'affichage, des vérifications générales, des initialisations, des fonctions principales, des vérifications de mouvement et une fonctionnalité de sauvegarde.

1. **Rule** : Ce module contient les règles spécifiques du jeu d'échecs modifié, telles que les mouvements autorisés pour chaque pièce, les règles de capture, etc.

2. **Game** : Le module Game gère le déroulement du jeu, y compris la boucle principale du jeu, l'interaction avec les joueurs et les vérifications de fin de partie.

3. **Displays** : Ce module est responsable de l'affichage graphique du jeu d'échecs modifié.

4. **GeneralChecks** : Ce module fournit des vérifications générales nécessaires au jeu, comme la vérification des échecs et des mises en échec, les vérifications de validité des mouvements, etc.

5. **Initialisation** : L'initialisation gère la mise en place initiale du jeu, y compris la disposition des pièces sur le plateau, l'attribution des rôles aux joueurs, etc.

6. **Projet** : Le module principal du jeu d'échecs modifié. Il regroupe les fonctions principales et les mécanismes de jeu.

7. **VerifMoves** : Ce module contient des fonctions spécifiques pour la vérification des mouvements des différentes pièces, notamment les mouvements spéciaux du jeu d'échecs modifié.

8. **Save** : Le module de sauvegarde permet aux joueurs de sauvegarder et de charger des parties enregistrées.

## Utilisation

- Au démarrage du jeu, le plateau s'affiche à l'écran avec les pièces disposées selon les règles spécifiées.

- Chaque joueur dispose de 40 points. Ces points permettent aux joueurs d'acheter, à leur tour, des pièces dans une boutique affichée, puis de les placer à leur guise dans leur zone de départ. Le roi ne peut pas être mis en échec avant le début de la partie.

- Les pièces disponibles sont les pièces classiques du jeu d'échecs, ainsi que deux nouvelles pièces : le Cavalier Fou (un déplacement combinant les mouvements du Cavalier et du Fou) et le Prince (se déplaçant comme le roi, mais sur une distance de 2 cases).

- Une fois que les deux joueurs sont prêts, la partie peut commencer.

- Les règles du jeu sont identiques à celles du jeu d'échecs classique, en tenant compte des pièces supplémentaires.

- La partie se termine dans les cas suivants : échec et mat (lorsque le roi est mis en échec et ne peut plus échapper) ou pat (lorsque le joueur ne peut plus déplacer aucune de ses pièces, mais n'est pas en position d'échec).





