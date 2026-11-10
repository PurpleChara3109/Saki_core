# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Contexte

Fangame Pokémon (« Pokémon Anima: Reminiscence ») sous Godot 4.3 (rendu Forward Plus), écrit en GDScript. Le code s'écrit dans VS Code, les scènes et les ressources `.tres` dans l'éditeur Godot. Le dépôt Git a pour racine le dossier parent (`Saki_core/`). `Anima-vault-fr/` est un coffre de notes Obsidian, non suivi par Git.

## Méthode de travail

- Prévenir l'utilisateur et attendre son accord avant chaque action : lecture de fichiers, commande, modification.
- Les fichiers de `docs/` sont des copies de documents vivants sur claude.ai. En cas de différence, le document vivant fait foi (voir l'en-tête de chaque fichier).

## Périmètre

- Ne modifier que les fichiers que l'utilisateur a explicitement autorisés. Par défaut : `docs/DETAILS-TECHNIQUES.md` uniquement.
- `docs/LORE.md` et `docs/PERSONNAGES.md` sont en lecture seule.

## Lancer et tester

- Pas de build, pas de linter, pas de lanceur de tests en ligne de commande. Aucune scène principale n'est définie dans `project.godot`, donc F5 ne lance rien.
- Un test est une scène de `scenes/tests/` dont le nœud racine porte un script `scripts/*Test.gd`. On l'ouvre dans Godot et on la lance avec F6. Le résultat s'affiche dans l'onglet Output (`print` et `assert`).
- Les résultats attendus de chaque test sont dans la section « Tester pas à pas » de `docs/DETAILS-TECHNIQUES.md`.

## Architecture

- **Données** : `MoveData` et `CreatureData` (`Resource`) sont éditées dans Godot et enregistrées en `.tres` dans `data/moves/` et `data/creatures/`. Aucune règle propre à une créature n'est écrite dans le code.
- **Règles** : des `RefCounted` testables sans scène. `AnimaPool` gère la Réserve (coût, Flux, Régénération, Instabilité), `Combatant` le lancer d'un coup et les dégâts, `Trainer` la Jauge et les Marques.
- **Scène** : `BattleScene.gd` pilote le combat jouable (`scenes/tests/battle_scene.tscn`) avec `Combatant` et `Trainer`. `BattleManager.gd` est une machine à états du tour que seul `BattleTest.gd` utilise. Sa logique de lancer fait doublon avec `Combatant.cast()`.
- **À venir** : un autoload `GameState` pour l'état persistant entre combats (`autoload/` est vide pour l'instant). La suite est décrite dans la section « Feuille de route du code (démo) » de `docs/DETAILS-TECHNIQUES.md`.

## Informations

- Règles chiffrées, code de référence et feuille de route : `docs/DETAILS-TECHNIQUES.md`.
- Conventions de code (anglais pour les identifiants, textes et commentaires ; tabulations ; nommage ; tests ; Git) et liste des écarts actuels entre document et code : section « Bonnes pratiques du code » de `docs/DETAILS-TECHNIQUES.md`.
- Univers, personnages, Lexique (termes du jeu et leur équivalent anglais, utilisés pour les identifiants) : `docs/LORE.md` et `docs/PERSONNAGES.md`.
