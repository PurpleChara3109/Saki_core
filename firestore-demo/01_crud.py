"""CRUD sur Firestore : Create, Read, Update, Delete."""
from google.cloud import firestore

from firestore_client import get_db, titre

db = get_db()
produits = db.collection("produits")

# ---------------------------------------------------------------- CREATE
titre("CREATE : set() avec un identifiant choisi")
ref = produits.document("p100")
ref.set({
    "titre": "Le Hobbit",
    "auteur": "J.R.R. Tolkien",
    "categorie": "fantasy",
    "prix": 10.5,
    "stock": 7,
    "tags": ["classique", "aventure"],
    "cree_le": firestore.SERVER_TIMESTAMP,   # horodatage posé par le serveur
})
print("créé :", ref.path)

titre("CREATE : add() avec un identifiant généré automatiquement")
_, ref_auto = produits.add({"titre": "Livre test", "categorie": "test", "prix": 1.0, "stock": 1})
print("créé :", ref_auto.path)

# ---------------------------------------------------------------- READ
titre("READ : lecture d'un document")
snap = ref.get()
print(snap.id, "->", snap.to_dict())

titre("READ : document inexistant (pas d'erreur, juste exists = False)")
print(produits.document("n-existe-pas").get().exists)

titre("READ : lister une collection (3 premiers)")
for doc in produits.limit(3).stream():
    print(doc.id, doc.get("titre"))

titre("READ : un seul champ d'un document")
print(ref.get(field_paths=["titre"]).to_dict())

# ---------------------------------------------------------------- UPDATE
titre("UPDATE : update() modifie des champs sans toucher aux autres")
ref.update({
    "prix": 11.0,
    "stock": firestore.Increment(-1),        # incrément atomique côté serveur
    "tags": firestore.ArrayUnion(["promo"]),  # ajoute sans doublon (comme $addToSet)
})
print(ref.get().to_dict())

titre("UPDATE : champ imbriqué avec la notation pointée")
client = db.collection("clients").document("cl001")
client.update({"adresse.ville": "Nantes"})   # si la démo est chargée (00_seed.py)
print(client.get().get("adresse") if client.get().exists else "(lancer 00_seed.py d'abord)")

titre("UPDATE : set(..., merge=True) = upsert (crée si absent, fusionne sinon)")
produits.document("p101").set({"titre": "Upsert", "stock": 5}, merge=True)
print(produits.document("p101").get().to_dict())

titre("UPDATE : supprimer un champ")
ref.update({"tags": firestore.DELETE_FIELD})
print(ref.get().to_dict())

# ---------------------------------------------------------------- DELETE
titre("DELETE : supprimer des documents")
for r in (ref, ref_auto, produits.document("p101")):
    r.delete()
print("p100 existe encore ?", ref.get().exists)
