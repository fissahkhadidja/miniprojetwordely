#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TAILLE_MOT 5 

// --- STRUCTURES ---
typedef struct Element {
    char mot[6];           
    struct Element* suivant;
} Element;

typedef enum { ABSENT, MAL_PLACE, CORRECT } EtatLettre;

typedef struct { 
    EtatLettre resultats[TAILLE_MOT]; 
} Analyse;

// --- FONCTIONS DE GESTION DU DICTIONNAIRE ---

// Ajout en début de liste (Utilisé par le chargeur et le solveur)
Element* ajouter_au_debut(Element* liste, const char* m) {
    Element* nouveau = malloc(sizeof(Element));
    if(!nouveau) return liste;
    strncpy(nouveau->mot, m, 5);
    nouveau->mot[5] = '\0';
    nouveau->suivant = liste;
    return nouveau;
}

// NOUVEAU : Charge les mots depuis le fichier word.txt
Element* charger_depuis_fichier(const char* nom_fichier, int* nb_mots) {
    FILE* f = fopen(nom_fichier, "r");
    if (!f) {
        printf("Erreur : Impossible de trouver le fichier %s\n", nom_fichier);
        return NULL;
    }
    Element* liste = NULL;
    char tampon[10];
    *nb_mots = 0;
    while (fscanf(f, "%s", tampon) == 1) {
        if (strlen(tampon) == 5) {
            liste = ajouter_au_debut(liste, tampon);
            (*nb_mots)++;
        }
    }
    fclose(f);
    return liste;
}

// Convertit la liste en tableau pour le choix aléatoire du mot secret
void copier_liste_vers_tableau(Element* L, char tab[][6]) {
    int i = 0;
    while (L) {
        strcpy(tab[i++], L->mot);
        L = L->suivant;
    }
}

// --- LOGIQUE DU JEU ---

Analyse comparer_mots(const char* essai, const char* cible) {
    Analyse bilan;
    bool cible_utilisee[5] = {false}, essai_utilise[5] = {false};
    for (int i = 0; i < 5; i++) {
        if (essai[i] == cible[i]) {
            bilan.resultats[i] = CORRECT;
            cible_utilisee[i] = essai_utilise[i] = true;
        } else {
            bilan.resultats[i] = ABSENT;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (essai_utilise[i]) continue;
        for (int j = 0; j < 5; j++) {
            if (!cible_utilisee[j] && essai[i] == cible[j]) {
                bilan.resultats[i] = MAL_PLACE;
                cible_utilisee[j] = true;
                break;
            }
        }
    }
    return bilan;
}

bool verifier_compatibilite(const char* mot_candidat, const char* dernier_essai, Analyse bilan_obtenu) {
    Analyse test = comparer_mots(dernier_essai, mot_candidat);
    for (int i = 0; i < 5; i++) {
        if (test.resultats[i] != bilan_obtenu.resultats[i]) return false;
    }
    return true;
}

Element* filtrer_dictionnaire(Element* L, const char* essai, Analyse bilan) {
    if (L == NULL) return NULL;
    L->suivant = filtrer_dictionnaire(L->suivant, essai, bilan);
    if (!verifier_compatibilite(L->mot, essai, bilan)) {
        Element* reste = L->suivant; 
        free(L); 
        return reste;
    }
    return L;
}

void afficher_bilan_textuel(const char* essai, Analyse bilan) {
    const char* texte_etat[] = {"ABSENTE", "MAL PLACEE", "CORRECTE"};
    printf("\nAnalyse de '%s' :\n", essai);
    for(int i = 0; i < 5; i++) {
        printf("  - %c : %s\n", essai[i], texte_etat[bilan.resultats[i]]);
    }
}

void liberer_memoire(Element* L) {
    while(L) {
        Element* a_supprimer = L;
        L = L->suivant;
        free(a_supprimer);
    }
}

// --- MAIN (Conserve ta structure d'origine) ---
int main() {
    srand(time(NULL));
    char rejouer;
    
    // Initialisation du dictionnaire depuis le fichier
    int nombre_total_mots = 0;
    Element* dictionnaire_complet = charger_depuis_fichier("word.txt", &nombre_total_mots);
    
    if (!dictionnaire_complet) return 1;

    // Création d'un tableau temporaire pour le tirage aléatoire
    char mots_pour_tirage[nombre_total_mots][6];
    copier_liste_vers_tableau(dictionnaire_complet, mots_pour_tirage);

    do {
        char* mot_secret = mots_pour_tirage[rand() % nombre_total_mots];
        int choix_mode;

        printf("\n========== WORDLE==========");
        printf("\n1. Mode Joueur (Humain)");
        printf("\n2. Mode Solveur (Automatique)");
        printf("\nVotre choix : ");
        scanf("%d", &choix_mode);

        if (choix_mode == 1) {
            char tentative[10];
            bool gagne = false;
            for (int tour = 1; tour <= 6; tour++) {
                printf("\nTour %d/6. Entrez un mot (MAJUSCULES) : ", tour);
                scanf("%s", tentative);
                Analyse res = comparer_mots(tentative, mot_secret);
                afficher_bilan_textuel(tentative, res);
                if (strcmp(tentative, mot_secret) == 0) { 
                    printf("\n>>> BRAVO ! Vous avez gagne.\n"); 
                    gagne = true; break; 
                }
            }
            if(!gagne) printf("\n>>> PERDU ! Le mot etait : %s\n", mot_secret);
        } 
        else {
            // On recrée la liste des possibles à partir du dictionnaire chargé
            Element* possibles = NULL;
            for (int i = 0; i < nombre_total_mots; i++) 
                possibles = ajouter_au_debut(possibles, mots_pour_tirage[i]);
            
            for (int tour = 1; tour <= 6; tour++) {
                if (!possibles) break;
                char* essai_ia = possibles->mot;
                Analyse res = comparer_mots(essai_ia, mot_secret);
                printf("\nTour %d. L'IA tente : %s", tour, essai_ia);
                afficher_bilan_textuel(essai_ia, res);
                if (strcmp(essai_ia, mot_secret) == 0) { 
                    printf("\n>>> L'IA a gagne en %d tours !\n", tour); 
                    break; 
                }
                possibles = filtrer_dictionnaire(possibles, essai_ia, res);
            }
            liberer_memoire(possibles);
        }

        printf("\nVoulez-vous refaire une partie ? (o/n) : ");
        scanf(" %c", &rejouer);

    } while (rejouer == 'o' || rejouer == 'O');

    liberer_memoire(dictionnaire_complet);
    printf("\nMerci et au revoir !\n");
    return 0;
}
