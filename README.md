# Tracé et remplissage de polygone

Ce programme permet à un utilisateur de tracer, remplir un polygone et le manipuler. Ce projet a été réalisé dans le cadre de l'UE "Fondements et Algorithmes de l'Imagerie Numérique".

## Installation des dépendances

     sudo apt-get install freeglut3-dev

## Lancement du programme

Après avoir compilé le programme avec la commande `make`, lancez-le avec `./plot [largeur] [hauteur]` (par exemple `./plot 400 400`).

## Utilisation du programme

Le programme fonctionne selon trois modes d'utilisation :

### Mode insertion

Ce mode s'active avec la touche `i`.

Pour ajouter un point au polygone, il suffit de cliquer dans la fenêtre. Il est ensuite possible, et ce, dans n'importe quel mode, d'ouvrir/fermer ce polygone avec la touche `c`, et de le remplir/vider avec la touche `f`.

### Mode vertex

Ce mode s'active avec la touche `v`.

Lorsqu'il est activé, un seul point est sélectionné. Pour avancer d'un point, utilisez la touche `page précédente` (qui va "vers le haut"), pour reculer, utilisez la touche `page suivante` ("vers le bas"). Que le polygone soit fermé ou non, il est possible de le parcourir entièrement, la sélection reprendra automatiquement à l'autre bout du polygone.

Il est également possible de sélectionner un point en cliquant à proximité.

Les touches directionnelles permettent de déplacer le point sélectionné dans le plan.

La touche `suppr` permet de supprimer l'ensemble des points du polygone.

### Mode edge

Ce mode s'active avec la touche `e`.

Lorsqu'il est activé, une seule arête est sélectionnée. Tout comme dans le mode vertex, la touche `page précédente` permet d'avancer d'une arête, la touche `page précédente`, de reculer, et il est possible de parcourir entièrement le polygone qu'il soit fermé ou non.

Il est également possible de sélectionner une arête en cliquant à proximité.

Un clic sur le bouton du milieu (molette) de la souris permet de découper une arête en deux, en y insérant un nouveau point.

## Exemple d'utilisation
