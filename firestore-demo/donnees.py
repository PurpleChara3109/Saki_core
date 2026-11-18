"""Jeu de données de la démo : une petite librairie en ligne.

Modélisation Firestore (cf. slide « Modélisation ») :

    produits/{produit_id}                      -> catalogue (collection racine)
    clients/{client_id}                        -> profil + adresse (map imbriquée)
    clients/{client_id}/commandes/{commande_id} -> sous-collection
        lignes: [ {produit_id, titre, prix_unitaire, quantite}, ... ]  (dénormalisé)
"""
import datetime as dt

from google.cloud import firestore

UTC = dt.timezone.utc

PRODUITS = {
    "p001": {"titre": "Dune", "auteur": "Frank Herbert", "categorie": "science-fiction",
             "prix": 9.9, "stock": 12, "tags": ["classique", "space-opera"]},
    "p002": {"titre": "Fondation", "auteur": "Isaac Asimov", "categorie": "science-fiction",
             "prix": 8.5, "stock": 4, "tags": ["classique"]},
    "p003": {"titre": "Neuromancien", "auteur": "William Gibson", "categorie": "science-fiction",
             "prix": 7.9, "stock": 20, "tags": ["cyberpunk"]},
    "p004": {"titre": "Le Petit Prince", "auteur": "Antoine de Saint-Exupéry", "categorie": "roman",
             "prix": 6.5, "stock": 30, "tags": ["classique", "jeunesse"]},
    "p005": {"titre": "L'Étranger", "auteur": "Albert Camus", "categorie": "roman",
             "prix": 7.2, "stock": 15, "tags": ["classique"]},
    "p006": {"titre": "Les Misérables", "auteur": "Victor Hugo", "categorie": "roman",
             "prix": 12.0, "stock": 3, "tags": ["classique", "pavé"]},
    "p007": {"titre": "Astérix le Gaulois", "auteur": "Goscinny & Uderzo", "categorie": "bd",
             "prix": 11.5, "stock": 25, "tags": ["humour", "jeunesse"]},
    "p008": {"titre": "Tintin au Tibet", "auteur": "Hergé", "categorie": "bd",
             "prix": 13.0, "stock": 8, "tags": ["aventure", "jeunesse"]},
    "p009": {"titre": "Persepolis", "auteur": "Marjane Satrapi", "categorie": "bd",
             "prix": 24.0, "stock": 6, "tags": ["autobiographie"]},
    "p010": {"titre": "Clean Code", "auteur": "Robert C. Martin", "categorie": "informatique",
             "prix": 39.9, "stock": 10, "tags": ["dev", "bonnes-pratiques"]},
}

CLIENTS = {
    "cl001": {"nom": "Camille Martin", "email": "camille@example.com",
              "adresse": {"rue": "12 rue des Lilas", "ville": "Angers", "cp": "49100"}},
    "cl002": {"nom": "Yanis Benali", "email": "yanis@example.com",
              "adresse": {"rue": "3 quai de la Fosse", "ville": "Nantes", "cp": "44000"}},
    "cl003": {"nom": "Inès Dupont", "email": "ines@example.com",
              "adresse": {"rue": "8 place Bellecour", "ville": "Lyon", "cp": "69002"}},
}


def _ligne(produit_id: str, quantite: int) -> dict:
    """Ligne de commande : on COPIE titre et prix (dénormalisation volontaire)."""
    p = PRODUITS[produit_id]
    return {"produit_id": produit_id, "titre": p["titre"],
            "prix_unitaire": p["prix"], "quantite": quantite}


def _commande(lignes: list, statut: str, jour: int) -> dict:
    total = round(sum(l["prix_unitaire"] * l["quantite"] for l in lignes), 2)
    return {"lignes": lignes, "total": total, "statut": statut,
            "date": dt.datetime(2026, 11, jour, 10, 0, tzinfo=UTC)}


# (client_id, commande_id, données)
COMMANDES = [
    ("cl001", "cmd001", _commande([_ligne("p001", 1), _ligne("p004", 2)], "livree", 2)),
    ("cl001", "cmd002", _commande([_ligne("p010", 1)], "en_cours", 10)),
    ("cl002", "cmd003", _commande([_ligne("p007", 2), _ligne("p008", 1)], "livree", 5)),
    ("cl003", "cmd004", _commande([_ligne("p009", 1)], "en_cours", 12)),
]


def vider(db: firestore.Client) -> None:
    """Supprime les collections de la démo, sous-collections comprises.

    Firestore ne supprime PAS les sous-collections quand on supprime un document :
    il faut un delete récursif.
    """
    for nom in ("produits", "clients"):
        db.recursive_delete(db.collection(nom))


def peupler(db: firestore.Client) -> None:
    """Insère le jeu de données avec un batch (écritures atomiques : tout ou rien)."""
    batch = db.batch()
    for pid, data in PRODUITS.items():
        batch.set(db.collection("produits").document(pid),
                  {**data, "cree_le": firestore.SERVER_TIMESTAMP})
    for cid, data in CLIENTS.items():
        batch.set(db.collection("clients").document(cid), data)
    for cid, cmd_id, data in COMMANDES:
        batch.set(db.collection("clients").document(cid)
                  .collection("commandes").document(cmd_id), data)
    batch.commit()
