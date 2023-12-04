# Top G Un drôle de robot autonome

Ce projet vise à créer un programmer un robot capable de suivre des trajets en calculant le plus cours chemin, d'identifier des coins 
et de trouver d'autres chemins lorsqu'il rencontre un poteau sur son parcours.

## Contenu du Projet

- `app/` : Contient le fichier source main du projet à exécuter.
- `lib/` : Contient tous les fichiers qui constituent la librairie du projet.

## Configuration Matérielle

Le robot utilise un suiveur de ligne Maker Line de Cytron et un capteur de distance. 
Avant de démarrer le projet il faut s'assurer que les branchements sont corrects sur les ports du robot.(Lire la documentation de ligne Maker Line de Cytron au besoin).


## Comment Démarrer

1. **Branchement :** (Guide pour réaliser les branchments)

   - Le suiveur de ligne Maker Line de Cytron doit être connecté aux ports spécifiés
     - Vert A2
     - Bleu A4
     - Violet A5
     - Gris A6
     - Blanc A7
   - Le capteur de distance de Sharp doit être connecté au port **A0**.
   - Les moteurs des roues doivent être connectés aux ports PWM pour le contrôle de la vitesse.
     - Roue de gauche D4 et D6
     - Roue de gauche D5 et D7
   - Le piezo doit être connecté au port B3 et B5
   - Les boutons sur le breadboard doivent être connectés aux ports :
     - Bouton de sélection : orange sur B2 et gris sur B0
     - Bouton de validation : rouge de D3 et bleu sur D1
   - La LED doit être branchée sur les ports :
     - Jaune sur D6
     - Orange sur D7

2. **Compilation :** Pour compiler le projet, lancez la commande `make` à partir du makefile situé dans votre dossier `app`.
3. **Chargement :** Chargez le programme sur le microcontrôleur en lançant `make install` dans votre terminale.
**Note** Par mesure de précautions, au moment de la compilation, ne branchez rien sur les ports B.

## Modes d'Opération

Le robot fonctionne en deux modes principaux :
- **Identifier le coin :** Permet au robot de déterminer son coin initial et son orientation. Appuyez une fois sur le bouton "Interrupt" de la carte mère pour entrer dans ce mode.
- **Effectuer un trajet :** Permet au robot de choisir et de suivre un trajet en utilisant toujours le plus court chemin. Appuyez sur le bouton de sélection de la carte mère pour entrer dans ce mode. Une fois votre ligne choisie et validée à partir du bouton Validate, il vous est donné la possibilité de choisir la colonne souhaitée. Une fois vos choix confirmés, le robot peut enfin entamer son trajet.


## Guide de démarrage

- **Mode "Identifier le coin" :**
  1. Mettez sous tension le robot.
  2. La DEL clignotera en vert à 4 Hz.
  3. Appuyez une fois sur le bouton "Interrupt" de la carte mère pour lancer le mode "Identifier le coin".

- **Mode "Effectuer un trajet" :**
  1. Mettez sous tension le robot.
  2. La DEL sera éteinte
  3. Appuyez sur le bouton de sélection de la carte mère pour lancer le mode "Effectuer un trajet".



## Auteurs
Jodel Alcindor
Noah Blanchard
Matei Zama
Saïd N. Saka
Samira Folefack
