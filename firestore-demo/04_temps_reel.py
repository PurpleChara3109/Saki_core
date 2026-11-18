"""Temps réel : un listener (on_snapshot) est notifié à chaque changement.

Principe : on « s'abonne » à une requête (ici : produits dont le stock est <= 5).
Le callback reçoit d'abord l'état initial (ADDED), puis chaque changement
(ADDED / MODIFIED / REMOVED) sans avoir à interroger la base en boucle.
"""
import time

from google.cloud import firestore
from google.cloud.firestore_v1.base_query import FieldFilter

from firestore_client import get_db, titre

db = get_db()
produits = db.collection("produits")
alerte_stock = produits.where(filter=FieldFilter("stock", "<=", 5))


def sur_changement(snapshots, changements, heure_lecture):
    for ch in changements:
        d = ch.document
        print(f"  [{ch.type.name:<8}] {d.id}  {d.get('titre')}  stock={d.get('stock')}")


titre("Abonnement : produits en alerte de stock (stock <= 5)")
abonnement = alerte_stock.on_snapshot(sur_changement)
time.sleep(2)   # laisse arriver l'état initial

titre("On modifie des documents depuis le même script, le listener réagit")
print("-> p003 passe à 2 (entre dans la requête)")
produits.document("p003").update({"stock": 2})
time.sleep(2)

print("-> p002 remonte à 50 (sort de la requête)")
produits.document("p002").update({"stock": 50})
time.sleep(2)

print("-> p003 supprimé")
produits.document("p003").delete()
time.sleep(2)

abonnement.unsubscribe()
print("\nListener arrêté.")
