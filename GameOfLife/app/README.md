Projet : Conway's Game of Life sur STM32G4

Ce projet implémente le célèbre "Jeu de la vie" de Conway sur une carte microcontrôleur STM32G4, interfacée avec un écran TFT tactile. Il offre une interface utilisateur interactive pour manipuler la grille et observer l'évolution des cellules selon les règles du jeu.
Table des matières

    Description du projet
    Fonctionnalités
    Matériel requis
    Environnement de développement
    Utilisation
    Structure du projet
    Crédits

1. Description du projet

Le Jeu de la vie de Conway est un automate cellulaire zéro-joueur, ce qui signifie que son évolution est déterminée par son état initial, sans aucune autre intervention. Ce projet permet d'interagir avec la grille initiale sur un écran tactile, puis de lancer la simulation pour observer les "générations" de cellules.
2. Fonctionnalités

    Simulation du Jeu de la Vie : Application des règles classiques pour l'évolution des cellules.
    Pause/Lecture : Mettre en pause ou reprendre la simulation.
    Modification interactive de la grille : Activer ou désactiver des cellules individuellement via l'écran tactile lorsque la simulation est en pause.
    Patterns prédéfinis : Insérer des configurations de cellules connues (ex: Box, Glider, Pulsar, Vaisseaux, etc.) via un menu dédié.
    Effacement de la grille : Réinitialiser toute la grille à des cellules mortes.
    Système de menus : Navigation intuitive entre les options principales et la sélection de patterns à l'aide des boutons.
    Gestion du rafraîchissement d'écran : Optimisation de l'affichage pour une expérience fluide.

3. Matériel requis

    Carte de développement STM32G4 (par exemple, Nucleo STM32G431KB).
    Écran TFT ILI9341 .
    Contrôleur tactile XPT2046 (souvent intégré à l'écran TFT).
    Boutons physiques (Down, Center) connectés selon la configuration définie dans button.h/button.c.

4. Environnement de développement

    IDE : STM32CubeIDE (recommandé pour les projets STM32).
    Bibliothèques : STM32Cube HAL/LL Drivers pour STM32G4.
    Outils : make pour la compilation, doxygen pour la génération de documentation.

5. Utilisation

Une fois le firmware flashé et le dispositif démarré :

    Écran d'accueil : Le projet affiche "Conway's Game Of Life". Appuyez sur le bouton Center pour commencer.
    Mode édition/Pause : La grille s'affiche.
        Tactile : Touchez n'importe quelle cellule sur l'écran pour inverser son état (vivante/morte).
        Bouton Center : Appuyez sur le bouton Center pour basculer entre le mode PLAY (la simulation tourne) et le mode PAUSE (vous pouvez éditer la grille).
    Menus :
        Lorsque la simulation est en pause (ou au démarrage), appuyez sur le bouton Center (si configuré ainsi) pour ouvrir le menu principal.
        Utilisez le bouton Down pour naviguer entre les options du menu.
        Appuyez sur le bouton Center pour valider une option.
        Options du menu principal :
            Play/Pause : Bascule l'état de la simulation.
            Clear Grid : Efface toutes les cellules de la grille.
            Save Grid : (Fonctionnalité de sauvegarde - le comportement exact dépend de votre implémentation).
            Select Pattern : Accède au menu de sélection de patterns.
        Menu de sélection de patterns :
            Utilisez le bouton Down pour choisir un pattern.
            Appuyez sur le bouton Center pour appliquer le pattern sur la grille (généralement au centre ou à une position prédéfinie).

6. Structure du projet (aperçu)

    app/ : Fichiers principaux (ex: gameoflife.h, actions.h, menu.h).
    Drivers/ : Fichiers liés aux drivers (HAL, ILI9341, XPT2046, etc.).
    config.h : Fichier de configuration globale du projet.
    Doxyfile : Fichier de configuration Doxygen.


