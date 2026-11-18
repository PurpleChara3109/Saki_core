"""Requêtes Firestore : filtres, tri, pagination, OR, agrégations, collection group."""
from google.cloud.firestore_v1.base_query import FieldFilter, Or

from firestore_client import get_db, titre

db = get_db()
produits = db.collection("produits")


def afficher(requete) -> None:
    for d in requete.stream():
        x = d.to_dict()
        print(f"  {d.id}  {x.get('titre'):<22} {x.get('categorie'):<16} {x.get('prix')} €  stock={x.get('stock')}")


titre("Égalité")
afficher(produits.where(filter=FieldFilter("categorie", "==", "bd")))

titre("Inégalité + tri + limite (les 3 moins chers à moins de 20 €)")
afficher(produits.where(filter=FieldFilter("prix", "<", 20)).order_by("prix").limit(3))

titre("Plusieurs filtres : catégorie ET prix (index composite requis en production)")
afficher(
    produits.where(filter=FieldFilter("categorie", "==", "science-fiction"))
            .where(filter=FieldFilter("prix", "<=", 9))
            .order_by("prix")
)

titre("IN : catégorie parmi une liste")
afficher(produits.where(filter=FieldFilter("categorie", "in", ["bd", "informatique"])))

titre("array-contains : recherche dans un tableau")
afficher(produits.where(filter=FieldFilter("tags", "array_contains", "jeunesse")))

titre("OR : roman OU stock faible (Or(...) disponible dans les versions récentes)")
afficher(produits.where(filter=Or([
    FieldFilter("categorie", "==", "roman"),
    FieldFilter("stock", "<", 5),
])))

titre("Pagination par curseur (page 2 de 3 produits, triés par prix)")
page1 = list(produits.order_by("prix").limit(3).stream())
dernier = page1[-1]
afficher(produits.order_by("prix").start_after(dernier).limit(3))

titre("Agrégations côté serveur : count / sum / avg (sans lire tous les documents)")
nb = produits.count(alias="nb").get()[0][0].value
stock_total = produits.sum("stock", alias="total").get()[0][0].value
prix_moyen = produits.avg("prix", alias="moyenne").get()[0][0].value
print(f"  {nb} produits, stock total = {stock_total}, prix moyen = {prix_moyen:.2f} €")

titre("Collection group : toutes les sous-collections « commandes » en une requête")
cmds = db.collection_group("commandes").where(filter=FieldFilter("statut", "==", "livree"))
for d in cmds.stream():
    print(f"  {d.reference.path}  total={d.get('total')} €")

titre("Ce que Firestore NE fait pas : pas de JOIN")
print("  -> pour afficher le client d'une commande, on lit le document parent (2e lecture)")
for d in db.collection_group("commandes").limit(1).stream():
    parent_client = d.reference.parent.parent.get()   # clients/{id}
    print(f"  commande {d.id} de {parent_client.get('nom')}")
