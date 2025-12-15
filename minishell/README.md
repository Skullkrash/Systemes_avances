# Projet - Minishell

## À propos

Ce projet, toujours réalisé dans le cadre de notre module de systèmes avancés, est une version plus légère d'un shell, appelé minishell. Il réponds à la plupart des exigences techniques demandées dans le sujet, celles-ci étant expliquées ci-dessous (lien vers le sujet à l'appui).


## Compilation du projet

Le projet se compile avec le fichier Makefile situé dans le même dossier, à l'aide de la commande "make". Le fichier exécutable "minishell" se trouvera alors dans le sous-dossier output/, prêt à l'emploi.


## Documentation

- [Documentation générée par doxygen **A REMPLIR QUAND ON AURA LES DONNEES**]()
- [PDF généré par doxygen **A REMPLIR QUAND ON AURA LES DONNEES**]()
- [Sujet du projet](./doc/00-project_minishell.pdf)


## Exigences techniques auxquelles nous avons répondu

### Capacité d'exécuter des commandes simples

Le minishell réalisé permet l'exécution de commandes simples.

Exemples :
- ls -a
- ps


### Capacité d'exécuter un sous-ensemble de plusieurs commandes

La logique de différents opérateurs a été recréé, permettant l'accès aux :
- Opérateurs de contrôle
    - Le ET logique: &&
    - Le OU logique: ||
- Opérateurs de redirection de flux simples :
    - Le pipe: |
    - Les chevrons: < et >
    - Les chevrons doubles: << et >>
- Exécutions en arrière-plan
    - Symbole &


### L'exécution built-in de certaines commandes

La logique des commandes suivantes a été réécrite :
- **cd**
- **pwd**
- **exit**
- **echo**


### Le maintien d'un historique

Un fichier renseignant toutes les commandes rentrées dans le minishell est disponible sous ~/minishell_logs/command_history à partir du moment où une commande est rentrée dans le terminal.
La commande **history** a également été programmée, et permet la visualisation du fichier susmentionné dans le terminal.


### Un mode de lancement "batch"

Un mode de lancement spécial "batch" a été réalisé, qui permet l'exécution de commandes par argument (qu'elles soient simples ou imbriquées avec des opérateurs).

Pour utiliser le mode batch, la syntaxe est la suivante :
- ./[path_to_minishell_executable] [OPTION] \<command\>

Les différentes options possibles sont :
- -c, --command <command>: Execute the specified command and exit.
- --help: Display help message and exit.


### La prise en charge d'alias temporaires

Une gestion des alias temporaires, donc disponibles durant une session du minishell, a été prévue (gestion et utilisation des alias dans les commandes lancées).

Les différentes actions possibles sur les alias sont les suivantes :
-  La définition d'alias
    - alias [nom_alias]='[command]'
- L'affichage de la commande correspondante à un alias
    - alias [nom_alias]
- L'affichage de toutes les définitions d'alias
    - alias
- La suppression d'alias
    - unalias [nom_alias]
- La suppression de tous les alias
    - unalias -a


## Page de manuel linux

**A REMPLIR QUAND CE SERA FAIT**

