# Démo Firestore — librairie en ligne (Python)

Projet de démonstration pour l'oral « NoSQL & Modern Data Stack » (sujet : Cloud Firestore).
Il illustre : modélisation (collections, documents, sous-collections), CRUD, requêtes,
agrégations, transactions/batch et temps réel, avec le client Python officiel.

## Modélisation

```
produits/{produit_id}                        catalogue (collection racine)
clients/{client_id}                          profil + adresse (map imbriquée)
clients/{client_id}/commandes/{commande_id}  sous-collection
    lignes: [{produit_id, titre, prix_unitaire, quantite}]   <- données dénormalisées
```

Choix assumés : une commande est stockée **dans** le client (lecture « mes commandes » en une requête),
et ses lignes sont **imbriquées** dans le document avec une copie du titre et du prix
(pas de JOIN dans Firestore, et le prix payé ne doit pas changer si le catalogue change).

## Prérequis

- Python 3.9+
- `pip install -r requirements.txt`

## Option A — Émulateur local (recommandée pour la démo, gratuit, sans compte)

Prérequis : Node.js, un JDK Java récent (requis par l'émulateur) et la Firebase CLI.

```bash
npm install -g firebase-tools
firebase emulators:start --only firestore      # lit firebase.json (port 8080, UI sur :4000)
```

Dans un second terminal :

```bash
# Linux / macOS
export FIRESTORE_EMULATOR_HOST=localhost:8080
# Windows PowerShell
$env:FIRESTORE_EMULATOR_HOST = "localhost:8080"
```

L'interface web de l'émulateur (http://localhost:4000/firestore) permet de voir les
documents pendant la démo.

## Option B — Projet Firestore réel

1. Créer un projet sur https://console.firebase.google.com et une base Firestore (mode Native, édition Standard).
2. Créer une clé de compte de service (JSON) et l'indiquer :
   ```bash
   export GOOGLE_APPLICATION_CREDENTIALS=/chemin/vers/cle.json
   export GCLOUD_PROJECT=votre-projet-id
   ```
3. Ne pas définir `FIRESTORE_EMULATOR_HOST`.
4. Attention : le quota gratuit est de 50 000 lectures / 20 000 écritures / 20 000 suppressions par jour,
   et certaines requêtes demandent un **index composite** (voir `firestore.indexes.json`).

## Ordre de la démo

| Étape | Commande | Ce qu'on montre |
|---|---|---|
| 0 | `python 00_seed.py` | Réinitialise la base et charge 10 produits, 3 clients, 4 commandes (batch) |
| 1 | `python 01_crud.py` | CREATE (`set`, `add`), READ, UPDATE (`update`, `Increment`, `ArrayUnion`, merge), DELETE |
| 2 | `python 02_requetes.py` | Filtres, tri, `in`, `array_contains`, `OR`, pagination, `count/sum/avg`, collection group, absence de JOIN |
| 3 | `python 03_transactions.py` | Transaction (stock + commande), échec propre, batch |
| 4 | `python 04_temps_reel.py` | Listener `on_snapshot` : ADDED / MODIFIED / REMOVED en direct |

Relancer `00_seed.py` entre deux passages pour repartir d'un état propre
(`01_crud.py` et `02_requetes.py` sont indépendants de l'ordre ; `03` et `04` modifient les stocks).

## Fichiers

- `firestore_client.py` : connexion (émulateur ou réel selon `FIRESTORE_EMULATOR_HOST`)
- `donnees.py` : jeu de données, `peupler()` et `vider()` (suppression récursive des sous-collections)
- `firebase.json` / `firestore.indexes.json` : config de l'émulateur et exemple d'index composite
