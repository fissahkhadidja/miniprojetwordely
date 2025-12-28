Mini-projet en C : Wordle Solver avec liste chaînée dynamique et filtrage récursif. Inclut un mode joueur et un solveur intelligent.
## image mode humaine
<img width="285" height="138" alt="Capturedeterminalwordel" src="https://github.com/user-attachments/assets/c3302268-31c0-402a-9359-16c800e34c07" />.
## image mode solveur 
<img width="239" height="108" alt="CapturedeterminalwordelIA" src="https://github.com/user-attachments/assets/1b6be52d-3afd-4d6e-8541-7366cd327b49" />
<img width="288" height="135" alt="CapturedeterminalwordelIA2" src="https://github.com/user-attachments/assets/d7d872be-fe52-48cf-8012-530f9b65e039" />

# Wordle Solver - Mini-Projet en C

Ce dépôt contient un programme en langage C capable de résoudre le jeu **Wordle**. Le projet met en œuvre des structures de données dynamiques pour simuler le jeu et créer une intelligence artificielle capable de trouver le mot secret.

## 🌟 Fonctionnalités
- **Mode Joueur (Humain) :** Permet à l'utilisateur de deviner un mot de 5 lettres en 6 tentatives avec des indices textuels.
- **Mode Solveur (Automatique) :** Une IA utilise une liste chaînée et un algorithme récursif pour éliminer les mots impossibles et trouver la solution.
- **Moteur de jeu :** Analyse précise des lettres (Correctes, Mal placées ou Absentes).

## 🛠️ Concepts Informatiques Utilisés
- **Structures de données :** Utilisation de `struct` pour les nœuds de la liste et `enum` pour les états des lettres.
- **Gestion de la mémoire :** Allocation dynamique (`malloc`) et libération (`free`) pour manipuler le dictionnaire.
- **Algorithmique :** Filtrage récursif de la liste chaînée pour optimiser la recherche du solveur.
## 🚀 Installation et Utilisation
Pour compiler et exécuter le projet sur Linux ou macOS :
1. Compilez le code :
   ```bash
   gcc main.c -o wordle_solver
