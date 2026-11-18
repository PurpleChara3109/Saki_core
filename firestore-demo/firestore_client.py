"""Connexion à Firestore : émulateur local (par défaut pour la démo) ou projet réel."""
import os

from google.cloud import firestore

# Pour un projet réel, définir GCLOUD_PROJECT avec l'identifiant de votre projet.
PROJECT_ID = os.environ.get("GCLOUD_PROJECT", "demo-nosql-firestore")


def get_db() -> firestore.Client:
    """Retourne un client Firestore.

    Si la variable FIRESTORE_EMULATOR_HOST est définie (ex. localhost:8080),
    la bibliothèque cliente envoie automatiquement toutes les requêtes à
    l'émulateur, sans identifiants. Sinon, elle utilise les identifiants
    Google par défaut (GOOGLE_APPLICATION_CREDENTIALS).
    """
    hote = os.environ.get("FIRESTORE_EMULATOR_HOST")
    if hote:
        print(f"[connexion] émulateur Firestore sur {hote} (projet {PROJECT_ID})")
    else:
        print(f"[connexion] Firestore réel (projet {PROJECT_ID})")
    return firestore.Client(project=PROJECT_ID)


def titre(texte: str) -> None:
    print(f"\n=== {texte} ===")
