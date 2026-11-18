"""Réinitialise la base et charge le jeu de données de la démo."""
from donnees import peupler, vider
from firestore_client import get_db, titre

db = get_db()

titre("Nettoyage")
vider(db)

titre("Chargement des données (batch)")
peupler(db)

print("produits :", len(list(db.collection("produits").stream())))
print("clients  :", len(list(db.collection("clients").stream())))
print("commandes (toutes sous-collections) :",
      len(list(db.collection_group("commandes").stream())))
