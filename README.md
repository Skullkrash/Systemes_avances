# Systèmes avancés

## À propos

Ce repo a été réalisé dans le cadre de notre module de systèmes avancés, durant la seconde année de nos études au CNAM (Conservatoire National des Arts et Métiers) d'Eckbolsheim.
Vous trouverez ici quatre dossiers de TP ainsi qu'un projet, qui nous ont été demandé de réaliser.

## Accès rapide aux différents dossiers :

- [Ressources mises à disposition par le professeur](./Ressources/)
- [TP1](./TP1/)
- [TP2](./TP2/)
- [TP3](./TP3/)
- [TP4](./TP4/)
- [Projet](./minishell/)

## Comment compiler les fichiers de TP

Pour compiler les fichiers de TP, qui utilisent un fichier CMakeLists.txt et non directement un MakeFile pour la compilation, il faut rentrer une série de commandes :
- Une première commande permettant de générer les fichiers cmake de compilation dans un dossier donné
    - cmake -S [dossier_du_CMakeLists] -B [dossier_cible_de_compilation]
- Une deuxième commande pour démarrer la compilation des différents fichiers d'exercices
    - make

Les fichiers compilés correspondants aux différents exercices seront ainsi disponibles dans le dossier bin/ respectif du TP.

## Membres du groupe

- [Quentin BRENNER](https://github.com/OneLiberty)
- [Jeannot NAEGELY](https://github.com/Skullkrash)