"""Écritures atomiques : batch (écritures seules) et transaction (lecture + écriture)."""
import datetime as dt

from google.cloud import firestore

from firestore_client import get_db, titre

db = get_db()


# ------------------------------------------------------------- TRANSACTION
@firestore.transactional
def acheter(transaction, produit_ref, client_ref, quantite: int):
    """Décrémente le stock ET crée la commande, ou ne fait rien du tout.

    Les lectures doivent précéder les écritures. Si un autre client modifie le
    produit entre-temps, la transaction est rejouée automatiquement.
    """
    produit = produit_ref.get(transaction=transaction)
    stock = produit.get("stock")
    if stock < quantite:
        raise ValueError(f"Stock insuffisant pour « {produit.get('titre')} » ({stock} < {quantite})")

    transaction.update(produit_ref, {"stock": stock - quantite})
    commande_ref = client_ref.collection("commandes").document()  # id auto
    transaction.set(commande_ref, {
        "lignes": [{"produit_id": produit_ref.id, "titre": produit.get("titre"),
                    "prix_unitaire": produit.get("prix"), "quantite": quantite}],
        "total": round(produit.get("prix") * quantite, 2),
        "statut": "en_cours",
        "date": dt.datetime.now(dt.timezone.utc),
    })
    return commande_ref.id


produit = db.collection("produits").document("p002")      # Fondation, stock = 4
client = db.collection("clients").document("cl002")

titre("Transaction : achat de 2 exemplaires (OK)")
print("stock avant :", produit.get().get("stock"))
cmd_id = acheter(db.transaction(), produit, client, 2)
print("commande créée :", cmd_id, "| stock après :", produit.get().get("stock"))

titre("Transaction : achat de 10 exemplaires (rejeté, rien n'est écrit)")
try:
    acheter(db.transaction(), produit, client, 10)
except ValueError as e:
    print("erreur :", e)
print("stock inchangé :", produit.get().get("stock"))

# ------------------------------------------------------------------- BATCH
titre("Batch : 3 écritures atomiques sans lecture")
batch = db.batch()
for pid in ("p001", "p003", "p005"):
    batch.update(db.collection("produits").document(pid), {"stock": firestore.Increment(10)})
batch.commit()
for pid in ("p001", "p003", "p005"):
    print(" ", pid, "stock =", db.collection("produits").document(pid).get().get("stock"))
