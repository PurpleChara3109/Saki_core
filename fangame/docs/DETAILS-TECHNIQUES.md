
# Pokémon Anima — Détails techniques et code

> Copie du 9 novembre 2026 (révision 340). Document vivant : https://claude.ai/artifact/Mjwsd8xssPv3975vBDLgW2 — en cas de différence, le document vivant fait foi. Les encarts (schémas) ne sont pas reproduits.

2026-10-13 · Tarah Mondesir

Ce document regroupe les règles chiffrées et le code du jeu. L'univers, l'histoire et les personnages sont dans le document [Pokémon Anima — Univers et histoire](https://claude.ai/code/artifact/6476a490-867c-4f6a-bdc0-a992857b07fe).

## Paramètres de départ

Ces valeurs sont des points de départ, regroupés ici pour n'être modifiés qu'à un seul endroit. Les paliers de coût sont dans le tableau du combat, et les paliers du Lien dans celui de la capture.

| Paramètre            | Valeur de départ                                                                                                                                   |
| --------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------- |
| Réserve maximale     | 20 + 3 × niveau (proposition)                                                                                                                      |
| Flux                  | 25 % de la Réserve maximale                                                                                                                        |
| Régénération       | 5 % de la Réserve maximale par tour, 10 % près d'une Veine, complet au repos                                                                      |
| Instabilité          | Sous 25 % de Réserve : 2 % de risque d'échec par point manquant, 50 % au maximum, réduit par la Cohésion                                        |
| Poussée              | +30 % de puissance, coût ×1,5, Flux ignoré, 1 point de Jauge, 1 Marque                                                                           |
| Résidu               | 10 points de Résidu retirent 5 % de Réserve maximale, jusqu'au repos                                                                              |
| Jauge                 | 3 points au rang 3, +1 par rang supplémentaire                                                                                                     |
| Seuil de Marques      | 6 + rang                                                                                                                                            |
| Repos                 | −2 Marques dans un Relais, −1 ailleurs                                                                                                            |
| Voie du Don           | +2 Marques par Poussée, seuil −2, Instabilité dès 35 % de Réserve, plus l'avantage et la faille propres à la variante (voir Variantes du Don) |
| Voie de la Discipline | Jauge +1, Poussée +20 % au lieu de +30 %, repos ×2                                                                                                |
| Voie de la Techno     | Jauge +2, Poussée ×1,25, 1 Marque toutes les deux Poussées                                                                                       |

**Accréditation (Techno).** Quatre niveaux, un module de bracelet chacun : 1, Charges (Jauge de 5 Charges) ; 2, Stabilisateur (réduit l'Instabilité pendant un tour) ; 3, Analyseur (affiche le Résidu et l'Usure adverses) ; 4, Surcharge (×1,5 au lieu de ×1,25 pour une Marque lourde). Hors combat : zones restreintes et achat de Cellules. Le Consortium peut suspendre l'Accréditation, donc couper les Charges.

**Rituels (Discipline).** Quatre rituels, un aspect nouveau chacun : Souffle (Jauge +1, Régénération doublée au premier tour), Calme (seuil d'Instabilité à 15 % au lieu de 25 %), Accord (Cohésion +0,1, le Lien progresse plus vite), Retour (les Marques deviennent de l'Anima rendue à la Veine). Un rituel s'accomplit hors combat ou en consommant un tour, et une interruption l'annule.

## Système de combat

Une attaque est de l'Anima mise en forme puis émise : elle coûte de la Réserve, jamais des PP. Le combat reste au tour par tour, avec PV, vitesse et types comme dans la série, pour que le joueur s'y retrouve.

**Ce qui change par rapport à la série**

- Les **PP disparaissent**. Chaque capacité a un coût en Anima, exprimé en pourcentage de la Réserve maximale.
- Le **Fl****ux** limite ce qu'on peut engager en un tour : un quart de la Réserve maximale. Un coup plus cher ne passe qu'en Poussée.
- La Régénératio**n** rend 5 % de la Réserve maximale à la fin de chaque tour, 10 % près d'une Veine, et tout au repos. Près d'une Veine dense, les Pokémon gagnent aussi 10 % de puissance.
- Les coups sont rangés en quatre **paliers de coût**, présentés ci-dessous.| Tier     | Coût | Rôle                                                                                                    |
  | -------- | ----- | -------------------------------------------------------------------------------------------------------- |
  | Breath   | 5 %   | Faible puissance, jamais d'échec. Gratuit quand la Réserve est vide, pour ne jamais bloquer le joueur. |
  | Standard | 10 %  | Le coup de base, soutenable sur tout un combat.                                                          |
  | Heavy    | 20 %  | Gros dégâts, mais cinq lancers vident la Réserve.                                                     |
  | Ultimate | 40 %  | Dépasse le Flux, donc ne se lance qu'en Poussée. Laisse beaucoup de Résidu.                           |

Ces valeurs sont des points de départ, à équilibrer en test.

**Les trois formes d'une attaque**

- **Impact** : l'Anima est portée par le corps, au contact. Équivalent des attaques physiques.
- **Jet** : l'Anima est projetée à distance. Équivalent des attaques spéciales.
- **Link** : l'Anima modifie l'état de la cible, du lanceur ou du terrain. Équivalent des attaques de statut.
  **Instabilit****é** : sous 25 % de Réserve, chaque capacité risque d'échouer, à raison de 2 % par point de pourcentage manquant, 50 % au maximum. La Cohésion réduit ce risque. L'échec fait perdre le coût sans produire d'effet. À 0 %, seul le Breath reste possible, et il devient gratuit.

**Poussé****e** : une fois par tour, le dresseur peut dépenser de sa propre Anima pour renforcer un coup de 30 %. Le coût du coup est alors multiplié par 1,5 et la limite de Flux est ignorée. Le dresseur y gagne une Marque (voir la section Marques et Jauges).

**R****ésidu** : chaque coup Heavy ou Ultimate laisse du Résidu, qui reste entre les combats. Chaque palier de 10 points de Résidu retire 5 % de Réserve maximale, jusqu'au repos dans un centre ou près d'une Veine.

Le cycle complet d'une attaque est dessiné ci-dessous.

[encart : {'id': '.42066', 'h': 'dcee15c8', 'caption': "cycle d'une attaque · 2 tests, 1 branche d'échec", 'ref': 'node/296fd3a5-d4d8'}]

Le coût est payé au Shaping. Un coup trop cher pour le Flux ne passe qu'en Poussée, et un coup lancé sous 25 % de Réserve peut échouer en perdant son coût.

## Marques et Jauges

La Jauge est le carburant du dresseur et la Marque est le prix qu'il paie : l'une se vide quand il aide son Pokémon, l'autre se remplit à chaque fois.

**Quatre valeurs, deux paires**

| Valeur   | Qui la porte | Ce qu'elle mesure                                       | Comment elle évolue                                               |
| -------- | ------------ | ------------------------------------------------------- | ------------------------------------------------------------------ |
| Réserve | Pokémon     | L'Anima disponible pour lancer des coups.               | Baisse à chaque coup, remonte par la Régénération et au repos. |
| Résidu  | Pokémon     | L'usure laissée par les gros coups.                    | Monte avec les coups Heavy et Ultimate, redescend au repos.        |
| Jauge    | Dresseur     | L'Anima personnelle qu'il peut injecter dans un combat. | Baisse à chaque Poussée, remonte au repos.                       |
| Marques  | Dresseur     | L'usure accumulée dans son corps.                      | Monte à chaque Poussée, redescend au repos.                      |

Les deux paires se répondent : ce qui se vide (Réserve, Jauge) et ce qui s'accumule (Résidu, Marques).

**La ****J****auge** : elle est débloquée au rang 3, Conducteur, avec 3 points, puis gagne 1 point par rang supplémentaire, avant les bonus de la voie. Chaque Poussée dépense 1 point. La Jauge ne remonte pas pendant un combat : elle se recharge entièrement au repos. Sans point de Jauge, le dresseur ne peut pas utiliser la Poussée.

**Les Mar**ques : une Marque est un trait lumineux qui apparaît sur la peau. Chaque Poussée en ajoute une. La Techno n'en ajoute qu'une toutes les deux Poussées, et le Don en ajoute deux. Rémy, à l'Anima neutre, n'en ajoute qu'une toutes les trois Poussées. Le seuil supportable est de 6 plus le rang, réduit de 2 pour le Don.

**Limite du protagoniste.** Le joueur ne peut pas dépasser le seuil de Marques. À l'atteinte, l'état est Saturé et la Poussée est bloquée. S'il force, le combat est interrompu par une courte scène d'Effacement et il revient à la dernière sauvegarde.

**Usure du Pokémon.** Le Résidu s'efface au repos. L'Usure est la part qu'il n'efface plus (celle des convertisseurs en Relais) et réduit lentement sa Réserve maximale. L'Analyseur du bracelet affiche les deux.

**Humain.** Sa Jauge joue le rôle de la Réserve, une Poussée par tour celui du Flux, et le seuil de Marques celui de la Cohésion. Ce qu'épuise Conduit côté dresseur reste à trancher.

| État   | Condition                     | Effet                                                                                                    |
| ------- | ----------------------------- | -------------------------------------------------------------------------------------------------------- |
| Discret | Moins de la moitié du seuil. | Aucune trace visible, aucun effet.                                                                       |
| Visible | Plus de la moitié du seuil.  | Les traits se voient sur la peau. Les personnages réagissent, surtout ceux de l'Ordre et du Consortium. |
| Saturé | Seuil atteint.                | La Poussée est bloquée jusqu'au repos.                                                                 |

**Faire baisser les Mar****que****s** : une nuit de repos dans un Relais efface 2 Marques, une nuit ailleurs en efface 1. La Discipline double ces valeurs. Ces nombres sont des points de départ, à équilibrer en test.

**Les ****Effacés** : ce sont d'anciens porteurs dont les Marques ont dépassé le seuil sans qu'ils s'arrêtent. Leur Anima perd sa plasticité : elle redevient brute et ne garde plus de pli. Le joueur n'en devient jamais un, mais il en croise, et ils montrent ce que le seuil protège. Au combat, ils n'ont pas d'injection de dresseur, leurs attaques font des dégâts neutres, leur Instabilité est permanente et leur Pokémon agit selon trois états d'IA (Sauvage, Hésitant, Reconnaissant).

## Variantes du Don

Le Don du protagoniste prend l'une de quatre variantes, déduite du style de combat pendant le combat de la démo (une dizaine de tours). Le jeu tient quatre scores cachés. La variante au score le plus haut l'emporte. En cas d'égalité, la dernière action du joueur départage. Le Don n'est proposé qu'après le combat, avec la seule variante déduite (les quatre si les scores sont exactement égaux). Si aucun seuil n'est atteint, il n'est pas proposé. Un refus, comme l'absence d'offre, ouvre les voies Techno et Discipline. Les seuils sont des valeurs de départ, à ajuster en test de jeu.

| Variante   | Avantage                                                                                                                                                       | Faille                                                                                                          | Seuil de déduction                                                        |
| ---------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| Résonance | Réserve adverse visible en combat, intention du prochain coup annoncée. Écho : si l'action du joueur répond à l'intention, il récupère 5 % de Réserve. | Après un tour d'usage, la Cohésion baisse, et d'un cran de plus chaque tour où le joueur ignore l'intention. | 2 tours ou plus passés à observer ou attendre avant la première attaque |
| Brasier    | Plus de Marques portées, plus de puissance pour les Poussées.                                                                                                | Atteint le seuil de Marques plus vite et se retrouve bloqué plus souvent.                                      | 3 Poussées ou plus, surtout sous 50 % de Réserve                         |
| Conduit    | Passe de l'Anima à son Pokémon sans dépenser de Jauge.                                                                                                      | Sa propre Réserve baisse au même rythme.                                                                      | Au moins la moitié des tours en actions de soutien                        |
| Ancrage    | Pas d'Instabilité à très basse Réserve.                                                                                                                    | Aucune Poussée possible.                                                                                       | 3 tours ou plus sous 25 % de Réserve, sans Poussée, en défendant        |

**Le Glissement de Noé (Don propre, hors des quatre variantes).** Au début de son tour, Noé peut changer la signature (le type) d'un de ses Pokémon en plein combat. Le nouveau type remplace l'ancien pour les efficacités, dans l'attaque comme en défense, tant que le Pokémon reste en jeu ou jusqu'à un nouveau Glissement. Chaque Glissement coûte 1 Marque (valeur de base, à ajuster en test) et une Poussée ne se cumule pas avec lui dans le même tour. Un seul Glissement par tour ; le Pokémon qui vient de glisser n'a pas encore d'efficacité prévisible pour l'adversaire, ce qui l'empêche de jouer sur un seul type. La fenêtre pour le joueur : après chaque Glissement, le Pokémon est en Instabilité (taux de base, à régler) pendant un tour, et quand Noé atteint son seuil de Marques il ne peut plus glisser. Ce combat enseigne la loi d'Interférence et la table des efficacités.

**Seuils de déduction du Don (valeurs de base, à ajuster en test).** Combat final d'environ 10 tours. Résonance : 2 tours ou plus à observer ou attendre avant la première attaque. Brasier : 3 Poussées ou plus, dont 2 sous 50 % de Réserve. Conduit : 5 tours de soutien ou plus sur 10, dont 2 transferts. Ancrage : 3 tours ou plus sous 25 % de Réserve sans Poussée, en défense. Égalité : la dernière action décide ; quatre scores exactement égaux : les quatre variantes sont proposées. Seuil de Marques en démo : 6 + rang, soit 9 au rang 3 (la Poussée s'ouvre au rang 3), moins 2 avec le Don (7).

**Coût du Conduit.** Chaque transfert d'Anima (soin, appui à la Poussée, résistance à l'Instabilité) n'utilise pas de Jauge, mais coûte 1 Marque tous les 2 transferts. Au seuil de Marques, plus de transfert et le Pokémon perd le bonus du Conduit ce tour-là.

**Voie obligatoire.** Fin du Relais 2 : le Don est proposé si un seuil est atteint ; refus ou aucun seuil, choix entre Techno et Discipline. Pas de sortie sans voie.

**Pokémon de départ (démo).** Trois espèces originales, une par voie, avec un Talent inédit. Don : Plante (finale Plante/Aria, nouveau type des Pokémon sensibles à l'Anima), Talent Pli libre : son Flux varie chaque tour (25 % ±10 %), affiché un tour à l'avance. Discipline : Eau (finale Eau/Psy), Talent Aspect : le premier rituel de chaque combat ne consomme pas de tour. Techno : Feu (finale Feu/Électrik), Talent Circuit : chaque Charge consommée sur lui rend 5 % de Réserve.

**Type Aria (efficacités).** Faiblesse : Poison. Résistances : Ténèbres, Spectre. Super efficace contre : Combat, Ténèbres. Immunité : attaques de statut. Type normal dans la région de Ryven. Plante est déjà faible au Poison, donc Plante/Aria subit x4 : c'est voulu, compensé par l'immunité aux statuts.

**Espèces (base).** Environ 100 espèces dans le jeu final, 15 dans la démo, mélange de Pokémon existants (type Aria ajouté si besoin) et d'espèces originales. Un tiers sont de type Aria (environ 33 au total, 5 dans la démo).

**Espèces Aria de la démo (5).** Starter du Don (Plante/Aria) ; Ralts et son évolution (Psy/Fée/Aria) près d'un Relais ; Natu (Psy/Vol/Aria) sur une Veine ; Linox (espèce originale Spectre/Aria, Talent Anima brute : dégâts neutres) dans la zone des Effacés ; le Psystigri de Maëlys, surnommé Silvane (Psy/Aria), Talent Écoute (voit la catégorie de la prochaine action adverse, sans la Réserve) et coup signature Dissonance (dégâts moyens, vole les boosts de stats et les immunités de l'adversaire : l'adversaire perd une immunité de type, le Psystigri la gagne). Chaque Aria réagit visiblement à l'Anima (halo près d'une Veine, réaction avant un combat).

## Confiance des rivaux

Chaque rival a un score caché. Les choix des actes 1 et 2 valent ±1, ceux de l'acte 3 valent ±2 : le score va de −12 à +12. À l'acte 4, Allié à +5 ou plus, Neutre de −4 à +4, Opposé à −5 ou moins. Valeurs de départ : Maëlys 0, Noé 0, Rémy −2. Chaque rival a un choix de trahison, à l'acte 3, qui plafonne son état à Neutre. Le ton des dialogues change quand le score passe ±3. Les neuf choix de chaque rival sont dans le document Univers et histoire.

## Capture, Lien et évolution

Capture, Lien et évolution sont trois façons de réécrire l'Anima d'un Pokémon : le jeu les traite comme une seule mécanique vue sous trois angles.

**La Cell****ule**** (équivalent de la Poké Ball)** : elle ramène le corps du Pokémon à l'état d'énergie pure et le stocke. À l'ouverture, l'Anima reprend sa forme, d'où le flash blanc. L'idée s'inspire d'une théorie de fans connue. Une Cellule se fabrique avec de l'énergie distillée, ce qui la rend coûteuse et lie le joueur à l'économie du monde.

**La capture, ou l'Accord** : la chance de réussite dépend de trois facteurs.

- Les PV restants de la cible : plus ils sont bas, plus la signature est instable.
- La **résonance** : plus la signature du Pokémon est proche de celle du dresseur, plus l'Accord est facile.
- Le **rang** du dresseur.
  À la capture, l'Anima du Pokémon s'accorde à celle du dresseur. C'est ce qui explique pourquoi un Pokémon sauvage devient docile.

**Le ****Lien** : une valeur de 0 à 100 par Pokémon. Elle monte avec les combats gagnés ensemble, les soins et les Poussées réussies, et baisse quand le dresseur abuse de la Poussée ou laisse son Pokémon tomber KO.

| Lien      | État          | Effet                                                              |
| --------- | -------------- | ------------------------------------------------------------------ |
| 0 à 29   | Réticent      | 10 % de chances d'ignorer un ordre.                                |
| 30 à 59  | Fiable         | Obéit toujours.                                                   |
| 60 à 89  | Uni            | La Poussée coûte ×1,25 au lieu de ×1,5.                        |
| 90 à 100 | Accord parfait | Cohésion +20 % et un peu de Résidu effacé après chaque combat. |

Le multiplicateur ×1,25 de la Techno ne se cumule pas avec celui du palier Uni : le plus faible s'applique.

**L'évolution, ou mutation d'Anima** : un Pokémon évolue quand sa Réserve maximale atteint un seuil et qu'une condition est remplie (Lien, lieu près d'une Veine dense, ou cristal d'Anima condensée). L'évolution consomme toute la Réserve en un flash : le Pokémon repasse à 0 % juste après, ce qui crée un moment de vulnérabilité à mettre en scène. Elle augmente la Réserve maximale et peut changer la signature.

## Plafond de niveau (level cap)

Un Pokémon ne peut pas dépasser un niveau maximum, qui dépend de la progression du dresseur et de sa voie. L'idée : un Pokémon ne va pas plus loin que ce que son dresseur sait canaliser. Cela empêche de grinder pour écraser les duels, qui sont calibrés sur des équipes précises.

**Formule.** Plafond = plafond de base de l'étape + bonus de la voie.

- **Plafond de base** : une table par étape de l'histoire (Relais), fixée à 25 avant le Relais 2, dans une ressource `.tres`.
- **Discipline** : chaque Rituel appris relève le plafond (quatre Rituels).
- **Techno** : chaque niveau d'Accréditation relève le plafond (quatre niveaux).
- **Don** : sur la voie du Don, le joueur rencontre des Effacés exclusifs à cette voie (à définir ; Thaïs, Hector et Armand en font partie), et chacun relève son plafond de niveau.
- **Rang** : chaque nouveau rang relève aussi le plafond, et battre le rival après le choix de la voie le relève aussi.
  **Champions.** Les champions (les Chefs de Relais) ont leur Pokémon signature au plafond +2, et le reste de leur équipe reste au plafond.

**Trois modes, un seul système.** Récit : pas de plafond. Standard : plafond souple (au-dessus, l'expérience gagnée est très réduite, mais le Pokémon n'est pas bloqué). Défi : plafond strict.

**Implémentation.** Les valeurs sont des données, pas du code : une ressource `LevelCapData` (`.tres`) contient la table de base et les bonus par Rituel, par niveau d'Accréditation et par rang. Une seule fonction `get_level_cap()` calcule le plafond à partir de l'état du dresseur (voie, Rituels appris, Accréditation, rang, étape) et du mode. La fonction de gain d'expérience la consulte, et l'interface affiche le plafond pour ne pas surprendre le joueur. Les valeurs chiffrées se règlent après les tests ; pour la démo, on peut poser la structure sans activer le plafond.

## Implémentation dans Godot

Tout le jeu repose sur des données : capacités et créatures sont des ressources `.tres` éditées dans Godot, et le code ne contient aucune règle écrite en dur par créature. Le code se rédige dans VS Code, les ressources et les scènes dans Godot. Les exemples visent Godot 4.3.

**Vocabulaire du code** : les textes affichés et les commentaires sont en anglais. Deux choses restent en français parce qu'elles sont enregistrées dans tes fichiers : les noms de champs `forme` et `palier` de `MoveData` (dans tes `.tres`), et les noms de nœuds de la scène de combat (`%NomEnnemi`, `%Surcharge`...). Les renommer casserait tes données ou ta scène.

**Qui fait quoi**

Chaque bloc de code de cette page est un fichier distinct. Voici à quoi sert chacun et comment l'utiliser.

| Fichier                      | À quoi il sert                                                              | Comment l'utiliser                                                                  |
| ---------------------------- | ---------------------------------------------------------------------------- | ----------------------------------------------------------------------------------- |
| `scripts/MoveData.gd`      | Décrit une capacité : nom, signature, palier de coût, puissance.          | Ressource : on crée des fichiers `.tres` dans `data/moves/`, un par capacité. |
| `scripts/CreatureData.gd`  | Décrit une créature : statistiques, signatures, capacités.                | Ressource : un fichier `.tres` par créature dans `data/creatures/`.            |
| `scripts/AnimaPool.gd`     | Gère la Réserve d'un combattant : coût, Régénération, risque d'échec. | Classe sans nœud, utilisée par le code avec `AnimaPool.new(50)`.                |
| `scripts/BattleManager.gd` | Squelette de la machine à états d'un tour, vérifié par le test 3.        | À attacher à un nœud `Node`. La scène de combat ne l'utilise pas encore.      |
| `scripts/Combatant.gd`     | Un combattant : créature, PV, Réserve, lancer d'un coup, dégâts.         | Classe sans nœud, créée par `BattleScene.gd`.                                  |
| `scripts/BattleScene.gd`   | Pilote la scène de combat : boutons, tour de l'ennemi, affichage.           | À attacher à la racine de `scenes/battle/combat.tscn`.                          |
| `scripts/Player.gd`        | Déplace le personnage case par case.                                        | À attacher au nœud `Node2D` du personnage, qui contient un `RayCast2D`.       |
| `scripts/AnimaTest.gd`     | Test 2 : vérifie la Réserve sans interface.                                | À attacher à un nœud `Node` dans une scène de test, puis lancer avec F6.      |
| `scripts/BattleTest.gd`    | Test 3 : vérifie le déroulé d'un tour.                                    | À attacher à un nœud `Node` dans une scène de test.                           |
| `scripts/DataTest.gd`      | Test 4 : vérifie le chargement des données.                                | À attacher à un nœud `Node` dans une scène de test.                           |

**Créer un script dans Godot, pas à pas**

1. Dans le panneau FileSystem, fais un clic droit sur le dossier `scripts`, puis *Create New > Script*.
2. Donne-lui le nom du fichier du tableau, par exemple `AnimaPool.gd`, et valide.
3. Le script s'ouvre dans VS Code. Remplace tout son contenu par le code correspondant de cette page, ligne `extends` comprise, puis enregistre.
4. Pour un script à attacher (`Player.gd`, `BattleManager.gd`, `AnimaTest.gd`), sélectionne le nœud dans la scène, clique sur l'icône *Attach Script* en haut du panneau Scene, puis choisis le fichier existant.
   L'étiquette « auto (bash) » en haut des blocs de code n'est que l'affichage de ce document, qui ne connaît pas GDScript : elle n'a aucun effet sur le code.

**Structure du projet**

- `data/moves/` et `data/creatures/` : les fichiers `.tres`.
- `scenes/overworld/` et `scenes/battle/` : la carte et le combat.
- `scripts/` : les classes de règles, dont `AnimaPool.gd` et `BattleManager.gd`.
- Un autoload `GameState` : rang du joueur, Jauge, Marques, équipe.
  **Les classes de données : capacité et créature**

**Fichier : ****`scripts/MoveData.gd`**. Une capacité : nom, signature, forme, palier de coût et puissance.

```gdscript
class_name MoveData
extends Resource
 
const COST_PERCENT := [5, 10, 20, 40]
 
@export var move_name: String
@export var signature: StringName
@export_enum("Impact", "Jet", "Link") var forme: int
@export_enum("Breath", "Standard", "Heavy", "Ultimate") var palier: int = 1
@export var power: int = 40
@export var accuracy: int = 100
 
func cost(reserve_max: int) -> int:
    return ceili(reserve_max * COST_PERCENT[palier] / 100.0)
```

**Fichier : ****`scripts/CreatureData.gd`**. Une créature : statistiques, signatures, Cohésion et liste de capacités.

```gdscript
class_name CreatureData
extends Resource
 
@export var species_name: String
@export var signatures: Array[StringName] = []
@export var base_hp: int = 40
@export var base_attack: int = 40
@export var base_defense: int = 40
@export var base_speed: int = 40
@export_range(0.0, 1.0) var cohesion: float = 0.2
@export var reserve_base: int = 20
@export var moves: Array[MoveData] = []
 
func reserve_max(level: int) -> int:
    return reserve_base + 3 * level
```

`reserve_max` applique la formule 20 + 3 × niveau du tableau des paramètres : la valeur de base change d'une créature à l'autre, le reste est commun.

**La R****é****serve d'Anima**

**Fichier : ****`scripts/AnimaPool.gd`**. La Réserve d'un combattant. Il n'a pas besoin d'être attaché à un nœud : le code le crée avec `AnimaPool.new(50)`.

```gdscript
class_name AnimaPool
extends RefCounted
 
var current: int
var maximum: int
 
func _init(max_value: int) -> void:
    maximum = max_value
    current = max_value
 
func flow() -> int:
    return int(maximum / 4.0)
 
func percent() -> float:
    return 100.0 * current / maximum
 
func failure_chance(cohesion: float) -> float:
    var missing := 25.0 - percent()
    if missing <= 0.0:
        return 0.0
    return clampf(missing * 2.0, 0.0, 50.0) * (1.0 - cohesion)
 
func spend(amount: int) -> void:
    current = maxi(current - amount, 0)
 
func regen(percent_of_max: float) -> void:
    current = mini(current + int(maximum * percent_of_max / 100.0), maximum)
```

**Le combat, en machine à états**

**Fichier : ****`scripts/BattleManager.gd`**, à attacher à un nœud `Node` de la scène de combat.

```gdscript
class_name BattleManager
extends Node
 
enum State { INTENT, SHAPING, EMISSION, RESOLUTION, END_OF_TURN }
 
signal state_changed(new_state: State)
 
var state: State = State.INTENT
var player_pool: AnimaPool
var enemy_pool: AnimaPool
var player_cohesion := 0.2
var chosen_move: MoveData
var surge := false
 
func choose_move(move: MoveData, use_surge: bool = false) -> void:
    if state != State.INTENT:
        return
    chosen_move = move
    surge = use_surge
    _set_state(State.SHAPING)
    _shape_move()
 
func _shape_move() -> void:
    var is_breath := chosen_move.palier == 0
    var cost := chosen_move.cost(player_pool.maximum)
    if surge:
        cost = ceili(cost * 1.5)
    elif cost > player_pool.flow():
        _set_state(State.INTENT)  # move refused: the trainer gives another order
        return
    if is_breath and cost > player_pool.current:
        cost = 0  # Breath becomes free when the reserve is empty
    elif cost > player_pool.current:
        _set_state(State.INTENT)
        return
    var failed := not is_breath and randf() < player_pool.failure_chance(player_cohesion)
    player_pool.spend(cost)
    _set_state(State.END_OF_TURN if failed else State.EMISSION)
 
func end_turn() -> void:
    player_pool.regen(5.0)
    enemy_pool.regen(5.0)
    _set_state(State.INTENT)
 
func _set_state(new_state: State) -> void:
    state = new_state
    state_changed.emit(new_state)
```

Ce squelette applique les règles du diagramme : Flux, Poussée, Instabilité, coût payé au Shaping. Les états Emission et Resolution restent à brancher sur le calcul des dégâts et sur les animations, et `end_turn()` sera appelée à la fin de la Resolution. Le point de Jauge et la Marque de la Poussée se gèrent dans `GameState`, à l'étape 5 de la construction.

**Le déplacement sur grille**

**Fichier : ****`scripts/Player.gd`**, à attacher au nœud du personnage.

```gdscript
extends Node2D
 
const TILE_SIZE := 16
const MOVE_TIME := 0.15
 
var moving := false
 
@onready var ray: RayCast2D = $RayCast2D
 
func _process(_delta: float) -> void:
    if moving:
        return
    var dir := Vector2.ZERO
    if Input.is_action_pressed("ui_right"):
        dir = Vector2.RIGHT
    elif Input.is_action_pressed("ui_left"):
        dir = Vector2.LEFT
    elif Input.is_action_pressed("ui_down"):
        dir = Vector2.DOWN
    elif Input.is_action_pressed("ui_up"):
        dir = Vector2.UP
    if dir != Vector2.ZERO:
        _try_move(dir)
 
func _try_move(dir: Vector2) -> void:
    ray.target_position = dir * TILE_SIZE
    ray.force_raycast_update()
    if ray.is_colliding():
        return
    moving = true
    var tween := create_tween()
    tween.tween_property(self, "position", position + dir * TILE_SIZE, MOVE_TIME)
    await tween.finished
    moving = false
```

Le script se place sur le personnage, avec un nœud enfant `RayCast2D`. La carte doit avoir une couche de physique sur ses tuiles de mur, et le rayon doit détecter cette couche.

**Un test sans interface**

**Fichier : ****`scripts/AnimaTest.gd`**, à attacher à un nœud `Node` d'une scène de test.

```gdscript
extends Node
 
func _ready() -> void:
    var pool := AnimaPool.new(50)
    var move := MoveData.new()
    move.palier = 2
    print("Cost of a Heavy move: ", move.cost(pool.maximum))
    pool.spend(move.cost(pool.maximum))
    print("Reserve: ", pool.current, " (", pool.percent(), " %)")
    pool.regen(5.0)
    print("After regen: ", pool.current)
    pool.current = 5
    print("Failure risk at 10 %: ", pool.failure_chance(0.2))
```

Crée une scène avec un seul nœud `Node`, attache le script, puis lance cette scène avec **F6** : les résultats s'affichent dans la console de Godot, en bas de l'éditeur.

Avec une Réserve de 50, le coup Heavy coûte 10, il reste 40 puis 42 après la Régénération, et le risque d'échec à 10 % de Réserve est de 24 % avec une Cohésion de 0,2. Si les chiffres affichés diffèrent, c'est que la règle et le code se sont éloignés.

**Ordre de construction**

1. Déplacement sur grille de 16 × 16 pixels, avec une carte en `TileMapLayer` et des collisions.
2. Les ressources `MoveData` et `CreatureData`, avec trois créatures et cinq capacités de test.
3. `AnimaPool` testée par des `print()` : coûts, Régénération, Instabilité.
4. Combat 1 contre 1 avec `BattleManager` : Intent, Shaping, Emission, Resolution, End of turn avec Régénération.
5. Poussée et Jauge du dresseur, enseignées au Relais 2.
6. Rencontres dans l'herbe, puis capture par Accord.
7. Premier Relais jouable, celui de Tibère, qui enseigne la Réserve.
   Le prototype des étapes 1 à 4 suffit pour savoir si le combat par Réserve est amusant avant d'écrire la moindre ligne d'histoire.

## Créer les données de test

Capacités et créatures sont des fichiers `.tres`, créés à partir des classes `MoveData` et `CreatureData`. Cette section crée trois capacités et deux créatures pour le prototype de combat. Les noms sont des marques de réservation, à remplacer par les tiens.

**Avant de commencer** : `scripts/MoveData.gd` et `scripts/CreatureData.gd` doivent exister et être enregistrés. Un fichier `.tres` ne contient que des valeurs : sans le script de sa classe, il est vide. C'est ce qui produit un `move_data.tres` inutile, créé comme ressource générique au lieu d'un vrai `MoveData`.

**Créer une capacité**

1. Dans le panneau FileSystem, fais un clic droit sur `data/moves/`, puis *Create New > Resource*.
2. Tape `MoveData` dans la recherche de la fenêtre, sélectionne-le et clique sur *Create*. S'il n'apparaît pas, enregistre le script puis fais *Project > Reload Current Project*.
3. Enregistre le fichier sous le nom du tableau, par exemple `souffle.tres`. Il s'ouvre dans l'Inspecteur.
4. Remplis les champs avec les valeurs du tableau, puis enregistre avec Ctrl+S.| Fichier          | move_name | signature  | forme  | palier   | power | accuracy |
   | ---------------- | --------- | ---------- | ------ | -------- | ----- | -------- |
   | `souffle.tres` | Souffle   | normal     | Jet    | Breath   | 20    | 100      |
   | `choc.tres`    | Choc      | roche      | Impact | Standard | 40    | 100      |
   | `rayon.tres`   | Rayon     | electrique | Jet    | Heavy    | 70    | 90       |

Les champs `forme` et `palier` gardent leur nom français, mais leurs listes déroulantes affichent les libellés anglais (Breath, Standard, Heavy, Ultimate).

**Créer une créature**

1. Fais un clic droit sur `data/creatures/`, puis *Create New > Resource*, choisis `CreatureData` et enregistre le fichier sous le nom voulu, par exemple `caillou.tres`.
2. Remplis les champs avec les valeurs ci-dessous.
3. Pour `signatures`, clique sur le champ, règle *Size* sur 1 et saisis la signature dans l'élément 0.
4. Pour `moves`, règle *Size* sur 2, puis glisse les deux fichiers `.tres` de capacités depuis FileSystem dans les emplacements.

- **`caillou.tres`** : `species_name` Caillou, `signatures` roche, `base_hp` 45, `base_attack` 50, `base_defense` 55, `base_speed` 30, `cohesion` 0,2, `reserve_base` 20, capacités `souffle` et `choc`.
- **`etincelle.tres`** : `species_name` Etincelle, `signatures` electrique, `base_hp` 40, `base_attack` 45, `base_defense` 35, `base_speed` 60, `cohesion` 0,15, `reserve_base` 20, capacités `souffle` et `rayon`.
  **Les pièges à éviter**
- **Un ****`.tres`**** créé sans choisir la classe** est une ressource vide. Supprime-le et recommence en choisissant `MoveData` ou `CreatureData`.
- **`MoveData`**** absent de la liste** : le script n'est pas enregistré, ou le projet n'a pas été rechargé.
- **Les champs n'apparaissent pas dans l'Inspecteur** : le script de la classe contient une erreur, soulignée en rouge dans VS Code. Corrige-la d'abord.
- **Les anciens ****`move_data.tres`**** et ****`creature_data.tres`** peuvent être supprimés.
  Le test 4 de la section suivante vérifie que ces données se chargent bien.

## Scène de combat minimale

Cette scène rend le combat visible avec des barres et des boutons, sans le moindre sprite. Elle ne sert qu'à une chose : juger si gérer sa Réserve rend les combats intéressants. C'est l'étape 4 de la construction, en version jouable.

**Ce que la scène fait, et ne fait pas**

- **Elle fait** : un combat 1 contre 1 au tour par tour, Caillou contre Etincelle au niveau 10, avec les coûts, le Flux, la Régénération, l'Instabilité, la Poussée, la Jauge de 3 points et les Marques.
- **Elle ne fait pas encore** : les types et leurs efficacités, la précision, la vitesse (le joueur agit toujours en premier), le Résidu, la capture et les statuts.
- **`BattleManager.gd`** reste intact, car le test 3 le vérifie. La scène utilise `Combatant`, qui applique la même règle aux deux camps. On les fusionnera plus tard.
  **Avant de commencer**

Les cinq fichiers de la section « Créer les données de test » doivent exister. Fais deux ajustements : donne à Caillou une troisième capacité, `rayon.tres`, pour qu'il ait un coup Heavy (la signature n'est pas encore utilisée), et vérifie que la première capacité de chaque créature est `souffle.tres`, le coup Breath, car l'ennemi s'en sert quand il ne peut rien payer d'autre.

**Fichier : ****`scripts/Combatant.gd`**. Un combattant : créature, PV, Réserve, lancer d'un coup et dégâts.

```gdscript
class_name Combatant
extends RefCounted
 
var data: CreatureData
var level: int
var hp: int
var hp_max: int
var pool: AnimaPool
 
func _init(creature: CreatureData, lvl: int) -> void:
    data = creature
    level = lvl
    hp_max = data.base_hp + level
    hp = hp_max
    pool = AnimaPool.new(data.reserve_max(level))
 
func is_down() -> bool:
    return hp <= 0
 
# Returns a dictionary: "result" is "ok", "failed" or "refused".
func cast(move: MoveData, surge: bool) -> Dictionary:
    var is_breath := move.palier == 0
    var cost := move.cost(pool.maximum)
    if surge:
        cost = ceili(cost * 1.5)
    elif cost > pool.flow():
        return {"result": "refused", "cost": 0}
    if is_breath and cost > pool.current:
        cost = 0
    elif cost > pool.current:
        return {"result": "refused", "cost": 0}
    var failed := not is_breath and randf() < pool.failure_chance(data.cohesion)
    pool.spend(cost)
    return {"result": "failed" if failed else "ok", "cost": cost}
 
func damage_against(target: Combatant, move: MoveData, boosted: bool) -> int:
    var power := float(move.power)
    if boosted:
        power *= 1.3
    var base := ((2.0 * level / 5.0 + 2.0) * power * data.base_attack / target.data.base_defense) / 50.0 + 2.0
    return maxi(int(base), 1)
```

**Fichier : ****`scripts/BattleScene.gd`**, à attacher à la racine de la scène de combat. Il crée les boutons de capacités tout seul, à partir des capacités de Caillou.

```gdscript
extends Control
 
const LEVEL := 10
const START_GAUGE := 3
 
@onready var enemy_name: Label = %NomEnnemi
@onready var enemy_hp_bar: ProgressBar = %PVEnnemi
@onready var enemy_reserve_bar: ProgressBar = %ReserveEnnemi
@onready var player_name: Label = %NomJoueur
@onready var player_hp_bar: ProgressBar = %PVJoueur
@onready var player_reserve_bar: ProgressBar = %ReserveJoueur
@onready var player_state: Label = %EtatJoueur
@onready var journal: Label = %Journal
@onready var actions: HBoxContainer = %Actions
@onready var surge_box: CheckButton = %Surcharge
 
var player: Combatant
var enemy: Combatant
var gauge := START_GAUGE
var marks := 0
var busy := false
var lines: Array[String] = []
 
func _ready() -> void:
    var caillou: CreatureData = load("res://data/creatures/caillou.tres")
    var etincelle: CreatureData = load("res://data/creatures/etincelle.tres")
    player = Combatant.new(caillou, LEVEL)
    enemy = Combatant.new(etincelle, LEVEL)
    player_hp_bar.modulate = Color(0.5, 1.0, 0.5)
    enemy_hp_bar.modulate = Color(0.5, 1.0, 0.5)
    player_reserve_bar.modulate = Color(0.4, 0.7, 1.0)
    enemy_reserve_bar.modulate = Color(0.4, 0.7, 1.0)
    for move in player.data.moves:
        var button := Button.new()
        button.text = "%s (cost %d)" % [move.move_name, move.cost(player.pool.maximum)]
        button.pressed.connect(_on_move_pressed.bind(move))
        actions.add_child(button)
    _log("The fight begins: %s vs %s." % [player.data.species_name, enemy.data.species_name])
    _refresh()
 
func _on_move_pressed(move: MoveData) -> void:
    if busy:
        return
    if surge_box.button_pressed and gauge <= 0:
        _log("Gauge empty: no Surge.")
    var boosted := surge_box.button_pressed and gauge > 0
    if not _play_turn(player, enemy, move, boosted):
        return
    if boosted:
        gauge -= 1
        marks += 1
    surge_box.button_pressed = false
    busy = true
    _refresh()
    await get_tree().create_timer(0.6).timeout
    if not enemy.is_down():
        _play_turn(enemy, player, _enemy_choose(), false)
        _refresh()
        await get_tree().create_timer(0.6).timeout
    _end_round()
    busy = false
 
func _play_turn(attacker: Combatant, target: Combatant, move: MoveData, boosted: bool) -> bool:
    var who := attacker.data.species_name
    var result := attacker.cast(move, boosted)
    match result["result"]:
        "refused":
            _log("%s can't cast %s: cost too high." % [who, move.move_name])
            return false
        "failed":
            _log("%s fails %s: unstable reserve, %d lost." % [who, move.move_name, result["cost"]])
        _:
            var damage := attacker.damage_against(target, move, boosted)
            target.hp = maxi(target.hp - damage, 0)
            var suffix := " with Surge" if boosted else ""
            _log("%s casts %s%s: %d damage." % [who, move.move_name, suffix, damage])
    return true
 
func _enemy_choose() -> MoveData:
    var options: Array[MoveData] = []
    for m in enemy.data.moves:
        if m.cost(enemy.pool.maximum) <= mini(enemy.pool.flow(), enemy.pool.current):
            options.append(m)
    if options.is_empty():
        return enemy.data.moves[0]
    return options.pick_random() as MoveData
 
func _end_round() -> void:
    player.pool.regen(5.0)
    enemy.pool.regen(5.0)
    _refresh()
    if enemy.is_down():
        _finish("Victory!")
    elif player.is_down():
        _finish("Defeat...")
 
func _finish(message: String) -> void:
    _log(message)
    for child in actions.get_children():
        (child as Button).disabled = true
    surge_box.disabled = true
 
func _refresh() -> void:
    _show(enemy, enemy_name, enemy_hp_bar, enemy_reserve_bar)
    _show(player, player_name, player_hp_bar, player_reserve_bar)
    player_state.text = "Gauge: %d   Marks: %d" % [gauge, marks]
 
func _show(c: Combatant, label: Label, hp_bar: ProgressBar, reserve_bar: ProgressBar) -> void:
    label.text = "%s  Lv %d   HP %d/%d   Reserve %d/%d" % [
        c.data.species_name, c.level, c.hp, c.hp_max, c.pool.current, c.pool.maximum]
    hp_bar.max_value = c.hp_max
    hp_bar.value = c.hp
    reserve_bar.max_value = c.pool.maximum
    reserve_bar.value = c.pool.current
 
func _log(text: String) -> void:
    lines.append(text)
    if lines.size() > 5:
        lines.pop_front()
    journal.text = "\n".join(PackedStringArray(lines))
```

**Construire la scène**

1. Dans Godot, fais *Scene > New Scene*, puis clique sur *User Interface*. Un nœud `Control` devient la racine. Renomme-le `Combat`, puis règle *Layout > Anchors Preset* sur *Full Rect* dans l'Inspecteur.
2. Ajoute-lui un enfant `MarginContainer` nommé `Marge`, aussi en *Full Rect*. Dans *Theme Overrides > Constants*, mets les quatre marges (gauche, haut, droite, bas) à 24.
3. Sous `Marge`, ajoute un `VBoxContainer` nommé `Colonnes`, avec une *Separation* de 16 dans *Theme Overrides > Constants*.
4. Sous `Colonnes`, crée les nœuds de l'arbre ci-dessous, dans cet ordre.
5. Pour chacun des dix nœuds marqués d'une étoile, fais un clic droit puis *Access as Unique Name*. Un symbole `%` apparaît à côté de leur nom.
6. Sur `Journal`, règle *Custom Minimum Size > Y* sur 120. Sur `Surcharge`, règle *Text* sur `Poussé``e`.
7. Sélectionne la racine `Combat`, attache-lui `scripts/BattleScene.gd`, puis enregistre la scène sous `scenes/battle/combat.tscn`.
8. Lance-la avec F6.

```
Combat (Control)
└── Marge (MarginContainer)
    └── Colonnes (VBoxContainer)
        ├── Ennemi (VBoxContainer)
        │   ├── NomEnnemi (Label) *
        │   ├── PVEnnemi (ProgressBar) *
        │   └── ReserveEnnemi (ProgressBar) *
        ├── Journal (Label) *
        ├── Joueur (VBoxContainer)
        │   ├── NomJoueur (Label) *
        │   ├── PVJoueur (ProgressBar) *
        │   ├── ReserveJoueur (ProgressBar) *
        │   └── EtatJoueur (Label) *
        ├── Actions (HBoxContainer) *
        └── Surcharge (CheckButton) *
```

**Résultat attendu** : la fenêtre affiche Etincelle en haut avec ses deux barres, le journal au milieu, Caillou en dessous, et trois boutons de capacités. Un clic sur un bouton lance le coup, l'ennemi répond après une courte pause, et le journal raconte chaque action. Coche *Poussé**e* avant un coup pour le renforcer, au prix d'un point de Jauge et d'une Marque. Le combat se termine par « Victory! » ou « Defeat... » et les boutons se désactivent.

**Si la scène ne marche pas**

- **Erreur sur ****`%NomEnnemi`**** ou un autre nom en ****`%`** : le nœud n'a pas été marqué *Access as Unique Name*, ou son nom diffère d'une lettre.
- **`Combatant`**** non reconnu** : le script n'est pas enregistré, ou Godot ne l'a pas encore pris en compte. Fais *Project > Reload Current Project*.
- **Aucun bouton** : `caillou.tres` n'a pas de capacités. Remplis son champ `moves`.
- **Erreur sur ****`null`**** au chargement** : un chemin de `.tres` ne correspond pas à un vrai nom de fichier.
  **Ce qu'il faut observer pour juger le système**
- Hésites-tu entre le coup sûr (Choc) et le coup puissant (Rayon), ou le Rayon écrase-t-il tout ?
- La Réserve te limite-t-elle vraiment, ou finis-tu toujours le combat avec presque tout ?
- Le Breath est-il un vrai choix de secours, ou ne l'utilises-tu jamais ?
- La Poussée est-elle tentante, et sens-tu son prix avec 3 points de Jauge ?
- Combien de tours dure un combat ? Un combat de 6 à 10 tours est un bon point de départ.
  Si la Réserve ne te limite jamais, augmente les coûts des paliers ou baisse la Régénération. Si elle t'étouffe, fais l'inverse. Ces valeurs sont dans le tableau des paramètres de départ, en haut de ce document. Les dégâts de `Combatant.gd` sont aussi des valeurs provisoires.

## Tester pas à pas

Chaque brique du code se vérifie par un petit test avant de passer à la suivante. Cette section s'allongera au fil du projet : chaque nouvelle étape de la construction y ajoute un test.

**Avant chaque test**

- Enregistre les scripts dans VS Code (Ctrl+S) et la scène dans Godot (Ctrl+S).
- Lance la scène ouverte avec **F6**. F5 lance la scène principale, qui n'existe pas encore.
- Lis les résultats dans l'onglet *Output* en bas de l'éditeur Godot. Les erreurs y apparaissent en rouge, avec le nom du fichier et la ligne.| Test                | Étape de construction | Scène                                     | Résultat attendu                                                                                          |
  | ------------------- | ---------------------- | ------------------------------------------ | ---------------------------------------------------------------------------------------------------------- |
  | 1. Déplacement     | Étape 1               | `scenes/overworld/test_deplacement.tscn` | Le carré avance d'une case par flèche et s'arrête devant le mur.                                        |
  | 2. Réserve         | Étape 3               | `scenes/tests/test_anima.tscn`           | Quatre lignes dans la console : 10, 40, 42, 24.                                                            |
  | 3. Combat           | Étape 4               | `scenes/tests/test_combat.tscn`          | Les états 1, 2, 0 et une Réserve de 47.                                                                  |
  | 4. Données         | Étape 2               | `scenes/tests/test_donnees.tscn`         | Deux lignes dans la console : Choc costs 5, Caillou max reserve 50.                                        |
  | 5. Scène de combat | Étape 4               | `scenes/battle/combat.tscn`              | Un combat jouable jusqu'à la victoire ou la défaite, à juger avec les critères de la section dédiée. |

**Test 1 : le déplacement sur grille**

1. Dans Godot, fais *Scene > New Scene*, puis clique sur *2D Scene*. Un nœud `Node2D` devient la racine. Renomme-le `TestDeplacement` (touche F2).
2. Ajoute-lui un enfant `Node2D` (clic droit sur la racine, *Add Child Node*) et nomme-le `Joueur`.
3. Sous `Joueur`, ajoute un `Sprite2D`. Dans l'Inspecteur, glisse `icon.svg` depuis le panneau FileSystem dans le champ *Texture*, puis règle *Transform > Scale* sur `0.125` pour obtenir un carré de 16 × 16 pixels.
4. Sous `Joueur`, ajoute un `RayCast2D`. Son nom doit rester exactement `RayCast2D`.
5. Sous `Joueur`, ajoute un `Camera2D` et règle son *Zoom* sur `4`, pour voir le carré en grand.
6. Sélectionne `Joueur`, clique sur l'icône *Attach Script*, puis sur l'icône de dossier à côté de *Path*, choisis `scripts/Player.gd` et valide.
7. Ajoute un mur : sous la racine, crée un `StaticBody2D` nommé `Mur`, avec un enfant `CollisionShape2D`. Dans l'Inspecteur du `CollisionShape2D`, choisis *Shape > New RectangleShape2D* et règle sa taille sur 16 × 16. Place `Mur` en position `(32, 0)`.
8. Enregistre la scène sous `scenes/overworld/test_deplacement.tscn`, puis lance-la avec F6.
   Résultat attendu : le carré apparaît au centre de la fenêtre. Chaque pression sur une flèche le déplace d'une case en un instant, et il refuse d'avancer vers la droite après un pas, car le mur est devant lui. Pour voir le mur, active *Debug > Visible Collision Shapes* dans l'éditeur avant de lancer.

**Si le déplacement ne marche pas**

- **Rien ne bouge** : vérifie qu'une petite icône de script apparaît à côté de `Joueur`, et clique dans la fenêtre de jeu pour lui donner le focus.
- **Erreur rouge sur ****`RayCast2D`** : le nœud n'est pas un enfant direct de `Joueur`, ou son nom est différent.
- **Le carré traverse le mur** : le `CollisionShape2D` n'a pas de forme, ou `Mur` n'est pas en `(32, 0)`.
- **Écran vide** : le `Sprite2D` n'a pas de texture, ou le `Camera2D` n'est pas sous `Joueur`.
  **Test 2 : la R****é****serve d'Anima**

1. Vérifie que `MoveData.gd` et `AnimaPool.gd` sont bien enregistrés dans `scripts/`.
2. Crée `scripts/AnimaTest.gd` avec le code de la section « Un test sans interface ».
3. Dans Godot, fais *Scene > New Scene > Other Node*, choisis `Node` et renomme-le `TestAnima`. Attache-lui `AnimaTest.gd`.
4. Enregistre la scène sous `scenes/tests/test_anima.tscn` (crée d'abord le dossier `tests` dans FileSystem), puis lance-la avec F6.
   Résultat attendu dans l'onglet *Output* :

```
Cost of a Heavy move: 10
Reserve: 40 (80 %)
After regen: 42
Failure risk at 10 %: 24
```

Godot peut afficher `80.0` et `24.0` au lieu de `80` et `24` : c'est la même valeur. Si les nombres diffèrent, la règle du document et le code se sont éloignés, et il faut corriger l'un des deux.

**Test 3 : le combat**

1. Vérifie que `BattleManager.gd` est enregistré dans `scripts/`.
2. Crée `scripts/BattleTest.gd` avec le code ci-dessous.
3. Crée une scène avec un nœud `Node` nommé `TestCombat`, attache-lui `BattleTest.gd`, enregistre-la sous `scenes/tests/test_combat.tscn`, puis lance-la avec F6.

```gdscript
extends Node
 
func _ready() -> void:
    var battle := BattleManager.new()
    add_child(battle)
    battle.player_pool = AnimaPool.new(50)
    battle.enemy_pool = AnimaPool.new(50)
    battle.state_changed.connect(func(s): print("State: ", s))
    var move := MoveData.new()
    move.palier = 1
    battle.choose_move(move)
    battle.end_turn()
    print("Player reserve: ", battle.player_pool.current)
```

Résultat attendu :

```
State: 1
State: 2
State: 0
Player reserve: 47
```

Les états sont numérotés dans l'ordre de l'énumération : 0 est l'Intent, 1 le Shaping, 2 l'Emission. Le coup Standard coûte 5 sur 50, il reste 45, et la Régénération de 5 % ajoute 2, d'où 47.

**Variante : un coup refusé.** Remplace `move.palier = 1` par `move.palier = 3`. Le coup Ultimate coûte 20, plus que le Flux de 12, donc il est refusé. Tu dois obtenir les états 1, 0, 0 et une Réserve de 50.

**Test 4 : charger les données**

1. Vérifie que `souffle.tres`, `choc.tres`, `rayon.tres` et `caillou.tres` existent et sont remplis.
2. Crée `scripts/DataTest.gd` avec le code ci-dessous.
3. Crée une scène avec un nœud `Node` nommé `TestDonnees`, attache-lui `DataTest.gd`, enregistre-la sous `scenes/tests/test_donnees.tscn`, puis lance-la avec F6.

```gdscript
extends Node
 
func _ready() -> void:
    var choc: MoveData = load("res://data/moves/choc.tres")
    var caillou: CreatureData = load("res://data/creatures/caillou.tres")
    print(choc.move_name, " costs ", choc.cost(caillou.reserve_max(10)))
    print(caillou.species_name, " max reserve ", caillou.reserve_max(10))
```

Résultat attendu :

```
Choc costs 5
Caillou max reserve 50
```

À 10 niveaux, la Réserve vaut 20 + 3 × 10 = 50, et un coup Standard coûte 10 % de 50, soit 5. Si la console affiche une erreur sur `null`, le chemin du fichier ne correspond pas à son nom réel : vérifie l'orthographe dans FileSystem.

Chaque nouveau test s'ajoute au tableau en haut de cette section, avec sa scène et son résultat attendu.

## Bonnes pratiques du code

Ces règles valent pour tout nouveau fichier et pour chaque fichier qu'on retouche. Le code existant qui s'en écarte se corrige à la prochaine modification du fichier concerné. La liste des écarts actuels est à la fin de cette section.

**Sources de vérité**

- Le document vivant fait foi, puis le tableau des paramètres de départ, puis le code. Une valeur chiffrée change d'abord dans le document, ensuite dans le code ou les `.tres`.
- Aucune règle propre à une créature ou à une capacité dans le code : ces valeurs vont dans les ressources `.tres`. Les constantes communes (paliers de coût, multiplicateur de Poussée) sont déclarées une seule fois, en `const`.
- Si un test donne un autre résultat que celui du document, c'est un écart : on corrige la règle ou le code, jamais seulement le résultat attendu.

**Langue et nommage**

- Identifiants, textes affichés, messages du journal, sorties de test et commentaires sont en anglais. Les termes du jeu reprennent l'équivalent anglais du Lexique de la bible :

| Terme du jeu          | Dans le code                                                  |
| --------------------- | ------------------------------------------------------------- |
| Réserve               | `pool` (classe `AnimaPool`), `reserve_max`                    |
| Flux                  | `flow`                                                        |
| Régénération          | `regen`                                                       |
| Instabilité, Cohésion | `instability`, `cohesion`                                     |
| Poussée               | `surge`                                                       |
| Jauge, Marque         | `gauge`, `mark`                                               |
| Résidu, Lien          | `residue`, `bond`                                             |
| Dresseur              | `trainer`                                                     |
| Paliers de coût       | `Breath`, `Standard`, `Heavy`, `Ultimate`                     |
| Formes                | `Impact`, `Jet`, `Link`                                       |
| Étapes du tour        | `INTENT`, `SHAPING`, `EMISSION`, `RESOLUTION`, `END_OF_TURN` |

- Exceptions, parce que ces noms sont enregistrés dans tes fichiers : les champs `forme` et `palier` de `MoveData`, et les noms de nœuds déjà utilisés par une scène. Renommer un champ `@export` efface sa valeur dans tous les `.tres` existants. Les libellés d'un `@export_enum` peuvent en revanche changer sans risque, car le `.tres` n'enregistre que le numéro.
- Conventions Godot : `PascalCase` pour les `class_name`, les fichiers de script et les nœuds ; `snake_case` pour les fonctions, les variables et les fichiers `.tres` et `.tscn` ; `UPPER_SNAKE_CASE` pour les constantes et les valeurs d'enum ; préfixe `_` pour ce qui est privé.
- Le fichier d'un script porte le nom de sa classe : `class_name Combatant` va dans `Combatant.gd`.

**Style GDScript**

- Indentation par tabulations, dans tous les fichiers. Godot refuse un fichier qui mélange tabulations et espaces. Dans VS Code, ajoute `"[gdscript]": { "editor.insertSpaces": false }` aux réglages. Les blocs de code de ce document sont indentés avec des espaces : après un collage, fais *Convert Indentation to Tabs* (palette de commandes, Ctrl+Maj+P).
- Typage statique partout : types des paramètres, type de retour (`-> void`), `:=` quand le type se déduit, type explicite sinon (`var options: Array[MoveData] = []`).
- Ordre dans un script : `class_name`, `extends`, `signal`, `enum`, `const`, `@export`, variables, `@onready`, puis `_init` et `_ready`, les fonctions publiques et enfin les fonctions privées.
- Les nœuds s'atteignent par leur nom unique (`%Nom`), jamais par un chemin relatif comme `get_node("../..")`.
- Un résultat à plusieurs cas renvoie une valeur d'`enum`, pas une chaîne : `CastResult.REFUSED` plutôt que `"refused"`.

**Architecture**

- Trois sortes de classes. Les `Resource` portent les données éditées dans Godot (`MoveData`, `CreatureData`). Les `RefCounted` portent les règles et se testent sans scène (`AnimaPool`, `Trainer`, `Combatant`). Les `Node` ne servent qu'à la scène : affichage, entrées, timers. Une règle de jeu ne vit jamais dans un script de scène.
- Une règle n'a qu'une implémentation. Le lancer d'un coup (Flux, Poussée, Breath gratuit, Instabilité) existe aujourd'hui dans `BattleManager._shape_move()` et dans `Combatant.cast()` : `Combatant.cast()` devient la version de référence, et `BattleManager` l'appelle.
- Les règles ne connaissent pas l'interface : elles émettent des signaux (`state_changed`) que la scène écoute.
- Ce qui persiste d'un combat à l'autre (rang, voie, Jauge, Marques, équipe) vit dans l'autoload `GameState`. Ce qui ne dure qu'un combat vit dans les objets du combat.
- Une IA lit l'état du combat et renvoie une action (capacité et Poussée ou non), sans toucher à l'affichage.

**Données**

- Les capacités vont dans `data/moves/`, les créatures dans `data/creatures/`, et les données de test dans un sous-dossier `tests/` de chacun.
- Un fichier `.tres` porte le nom de sa capacité ou de sa créature en `snake_case` : `bloom.tres`.
- La première capacité de chaque créature est un coup Breath, car l'IA s'en sert quand elle ne peut rien payer d'autre.
- Les chemins `res://` sont déclarés en `const` en tête de script, pas dispersés dans le code.

**Tests**

- Chaque brique de règle a sa paire : `scripts/XxxTest.gd` et `scenes/tests/xxx_test.tscn`, lancée avec F6.
- On vérifie avec `assert(condition, "message")` plutôt qu'en lisant des `print`, et une dernière ligne affiche `Xxx: all tests passed`. Les `assert` ne s'exécutent que dans l'éditeur et les builds de debug, ce qui suffit pour ces tests.
- Les valeurs attendues se calculent à partir du tableau des paramètres, avec le calcul en commentaire (par exemple `# 20 + 3 × 10 = 50`).
- Pour une règle aléatoire, on teste la probabilité (`failure_chance`) plutôt qu'un lancer, ou on fixe la graine avec `seed(1)`.
- Chaque nouveau test s'ajoute au tableau de la section « Tester pas à pas ».

**Git**

- Un commit par étape de la feuille de route, une fois son test passé. Messages en anglais, à l'impératif.
- Un `.tres` ou un `.tscn` part dans le même commit que les scripts dont il dépend. Les fichiers `.import` se versionnent, pas le dossier `.godot/`.
- Les fichiers `*:Zone.Identifier`, que Windows crée lors d'une copie, ne se versionnent pas : ajoute la ligne `*:Zone.Identifier` au `.gitignore`.

**Écarts actuels à corriger**

| Écart                                  | Où                                                                                                                                                                                                    | Correction                                                                                                                                                                    |
| -------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Identifiants en français               | `AnimaPool.debit()` et `reflux()` ; états `INTENTION`, `MISE_EN_FORME`, `FIN_DE_TOUR` et variable `overcharge` dans `BattleManager` ; libellés `Souffle`, `Puissant`, `Ultime`, `Lien` de `MoveData` ; `overcharge_box` dans `BattleScene` | `flow()`, `regen()`, `INTENT`, `SHAPING`, `END_OF_TURN`, `surge`, `Breath`, `Heavy`, `Ultimate`, `Link`, `surge_box`                                                          |
| Textes et commentaires en français     | `BattleScene.gd`, `AnimaTest.gd`, `BattleManager.gd`, `Trainer.gd`, `TrainerTest.gd`, et les blocs Trainer et TrainerTest de ce document                                                              | Traduire en anglais                                                                                                                                                           |
| Espaces au lieu de tabulations         | `AnimaPool`, `AnimaTest`, `BattleManager`, `BattleTest`, `CreatureData`, `MoveData`, `Player`, `Trainer`, `TrainerTest`                                                                               | *Convert Indentation to Tabs*                                                                                                                                                 |
| Nom de fichier différent de la classe  | `Combattant.gd`                                                                                                                                                                                       | Renommer en `Combatant.gd` depuis le panneau FileSystem de Godot, qui met à jour les références                                                                                |
| Lancer d'un coup écrit deux fois       | `BattleManager._shape_move()` et `Combatant.cast()`                                                                                                                                                   | Fusionner sur `Combatant.cast()`                                                                                                                                              |
| Résultat de lancer en chaîne           | `Combatant.cast()`                                                                                                                                                                                    | Remplacer par un `enum CastResult`                                                                                                                                            |
| Dossiers de test au nom différent      | `data/moves/test` et `data/creatures/tests`                                                                                                                                                           | `tests/` pour les deux                                                                                                                                                        |
| Noms et places des scènes              | `test_combat`, `test_data`, `test_deplacement_joueur` ; `trainer_test.tscn` manquante ; `battle_scene.tscn` dans `scenes/tests/`                                                                      | `combat_test`, `data_test`, `player_movement_test` ; créer `trainer_test.tscn` ; déplacer la scène de combat dans `scenes/battle/`. Mettre ensuite à jour le tableau des tests |

## Feuille de route du code (démo)

**État de départ (arborescence du projet).** `autoload/` est vide ; `data/creatures/tests` contient pikachu et rika, `data/moves/test` contient bloom, hyperbeam et tomberoche ; `scripts/` contient `AnimaPool.gd`, `Combattant.gd`, `BattleManager.gd`, `BattleScene.gd`, `CreatureData.gd`, `MoveData.gd`, `Player.gd` et les scripts de test ; `scenes/battle` est vide, les scènes de test sont dans `scenes/tests`. Le prototype de combat par Réserve fonctionne ; le reste des règles reste à coder.

**Ordre proposé pour la suite** (une étape à la fois, chacune testable avant la suivante) :

1. **Autoload ****`GameState.gd`** (dans `autoload/`, à déclarer dans Projet > Paramètres > Autoload) : rang, voie, Marques, seuil de Marques, Jauge. C'est là que vivent les valeurs qui persistent d'un combat à l'autre.
2. **Instabilité dans ****`AnimaPool.gd`** : déjà codée (`failure_chance` dans `AnimaPool.gd`, multipliée par (1 − Cohésion)). Il reste à ajouter un test dédié dans `AnimaTest.gd`.
3. **Poussée dans ****`BattleManager.gd`**** et ****`Combattant.gd`** : déjà codée sous le nom `surge` (`Combattant.cast` : coût ×1,5, ignore le Flux ; `damage_aga``in``s``t` : +30 %). Mais la Jauge et les Marques sont des variables locales de `BattleScene.gd`, et `BattleManager.gd` n'est pas utilisé par la scène. À faire : sortir la Jauge et les Marques vers un objet `Trainer` (voir ci-dessous), avec un coût en Marques variable (1 par Poussée, 1 toutes les 2 pour un Techno, 1 toutes les 3 pour Rémy).
4. **États de Marques** : Discret, Visible, Saturé (seuil 6 + rang), blocage de la Poussée à Saturé, et repos qui efface des Marques.
5. **Ressource ****`Dresseur`**** (****`scripts/TrainerData.gd`****)** : voie, Jauge, Marques, paramètres de Poussée, accréditation. Les rivaux sont des instances de cette ressource.
6. **Charges et Accréditation** (`scripts/Bracelet.gd`) : Jauge de 5 Charges, module Charges (niveau 1), puis Stabilisateur, Analyseur et Surcharge plus tard.
7. **IA de Rémy** (`scripts/ai/RemyAI.gd`) : Poussée tous les 3 tours, bascule sous 25 % de Réserve. L'IA lit l'état du combat et renvoie une action, sans toucher à l'affichage.
8. **Scène ****`scenes/battle/duel_remy.tscn`** : le duel de Rémy comme premier combat scripté de la démo.
9. **Ensuite** : Glissement de Noé (changement de type en combat, 1 Marque), rituels de Maëlys (Souffle, Calme, Accord), Talents des starters (Pli libre, Aspect, Circuit), type Aria et son immunité aux statuts.
   **Tests à garder pour chaque étape** : une scène de test dans `scenes/tests` avec un script `*Test.gd`, comme pour `AnimaTest.gd`. Les règles chiffrées (Instabilité, coût de la Poussée, seuil de Marques) se vérifient avec des valeurs connues avant d'être branchées sur le combat.

**Ce que tu dois faire.** Dis-moi quand une étape est terminée et colle les sorties de test dans la conversation. Pour l'étape en cours, je te donne le code complet des fichiers à créer ou à modifier.

### Étape 1 : l'objet Trainer

**Fichier : ****`scripts/Trainer.gd`****.** Il regroupe ce qui appartient au dresseur et non à une créature : rang, Jauge, Marques, seuil et états de Marques. Les paramètres `marks_per_push` et `pushes_per_mark` permettent de régler le coût en Marques d'une Poussée selon la voie (Don : 2 Marques par Poussée ; Techno : 1 Marque toutes les 2 Poussées ; Rémy : 1 Marque toutes les 3 Poussées). Le seuil de Marques vaut 6 + rang, donc 9 au rang 3, et 7 avec le Don (`threshold_bonus = -2`). L'état Visible commence à la moitié du seuil (valeur de départ, à ajuster).

```gdscript
class_name Trainer
extends RefCounted
 
enum MarkState { DISCREET, VISIBLE, SATURATED }
 
var rank: int = 3
var gauge: int = 3
var marks: int = 0
var threshold_bonus: int = 0   # -2 pour un porteur de Don
var marks_per_push: int = 1    # 2 pour un porteur de Don
var pushes_per_mark: int = 1   # 2 pour un Techno, 3 pour Rémy
var push_count: int = 0
 
func mark_threshold() -> int:
    return 6 + rank + threshold_bonus
 
func mark_state() -> MarkState:
    if marks >= mark_threshold():
        return MarkState.SATURATED
    if marks * 2 >= mark_threshold():
        return MarkState.VISIBLE
    return MarkState.DISCREET
 
func can_push() -> bool:
    return gauge > 0 and mark_state() != MarkState.SATURATED
 
# Appeler après une Poussée réussie ou ratée.
func register_push() -> void:
    gauge -= 1
    push_count += 1
    if push_count % pushes_per_mark == 0:
        marks += marks_per_push
```

### Étape 2 : le test du Trainer

**Fichier : ****`scripts/TrainerTest.gd`**, à attacher au `Node` racine d'une scène `scenes/tests/trainer_test.tscn`. Lance la scène avec F6 : la ligne `Trainer: tous les tests passent` doit apparaître dans Output.

```gdscript
extends Node
 
func _ready() -> void:
    var t := Trainer.new()
    assert(t.mark_threshold() == 9)
    t.threshold_bonus = -2
    assert(t.mark_threshold() == 7)
 
    var r := Trainer.new()          # Rémy : 1 Marque tous les 3 Poussées
    r.pushes_per_mark = 3
    r.gauge = 9
    for i in 3:
        r.register_push()
    assert(r.marks == 1)
 
    var d := Trainer.new()          # Don : 2 Marques par Poussée, seuil 7
    d.threshold_bonus = -2
    d.marks_per_push = 2
    d.gauge = 9
    for i in 4:
        d.register_push()
    assert(d.marks == 8)
    assert(d.mark_state() == Trainer.MarkState.SATURATED)
    assert(not d.can_push())
    print("Trainer: tous les tests passent")
```

### Étape 3 : brancher le Trainer dans BattleScene.gd

Une fois le test passé, dans `scripts/BattleScene.gd` :

1. Remplace `var gauge := START_GAUGE` et `var marks := 0` par `var trainer := Trainer.new()`.
2. Dans `_on_move_pressed`, remplace la condition de Poussée par `var boosted := surge_box.button_pressed and trainer.can_push()`. Si la case est cochée mais `can_push()` est faux, écris dans le journal que la Poussée est impossible (Jauge vide ou Marques saturées).
3. Après le coup, remplace `gauge -= 1` et `marks += 1` par `trainer.register_push()`.
4. Dans `_refresh`, affiche `trainer.gauge`, `trainer.marks` et `trainer.mark_threshold()` dans `player_state`.
   `BattleManager.gd` n'est pas utilisé par la scène actuelle : la logique de tour est dans `BattleScene.gd`. Elle sera sortie de la scène dans une étape ultérieure, pour que l'IA de Rémy puisse jouer sans affichage.
