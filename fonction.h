#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

#include <stdbool.h>

struct liste_int{
    int *liste;
    int taille;
};


int *c_ind_table(int indice);
void etat_finaux_(bool *E, int taille, bool gagnant);
void evaluer_politique_(float* f_eval, int* politique, bool *E_gagnant, bool *E_perdant, int taille);
void aff_fc_valeur(float* fc_valeur, int taille);
void aff_table(int *table);
struct liste_int *etats_possible(int indice, bool tourAgent);
int *creer_politique(int taille);
int *opti_politique(bool *E_gagnant, bool *E_perdant);

#endif // FONCTION_H_INCLUDED
