#include "fonction.h"

#include <stdio.h>
#include <stdlib.h>

#include <stdbool.h>

#include <math.h>

#define p39 19683

int *c_ind_table(int indice){
/*
Permet de convertir la table au format int en format list
*/

int *table = malloc(9*sizeof(int));

for(int i = 0; i < 3; i++){
    for(int j = 0; j < 3; j++){
        table[i*3+j] = (indice%(int)(pow(3, i*3+j+1)))/(int)(pow(3, i*3+j));
        //printf("%d | %d\n", i*3+j, indice%(int)(pow(3, i*3+j+1))/(int)(pow(3, i*3+j)));
    }
}

return table;
}

void etat_finaux_(bool *E, int taille, bool gagnant){
/* Permet de déterminer les états tels que l'agent soit gagnant ou perdant.

Paramètres :
E : la liste des Etats Gagnant/Perdant à écrire
taille ; la taille de la liste
gagnant : doit on déterminer emplacements gagnant ou perdant
*/

    int pion = gagnant ? 2 : 1;

    for(int k = 0; k < taille; k++){

    int *t = c_ind_table(k);

        E[k] = false;

        for(int i = 0;i < 3; i++){
            if ((t[i*3] == pion && t[1+3*i] == pion && t[2+3*i] == pion) || (t[i+6] == pion && t[i] == pion && t[i+3] == pion) || (t[0] == pion && t[4] == pion && t[8] == pion) || (t[2] == pion && t[6] == pion && t[4] == pion)){
                E[k] = true;
            }
        }

    free(t);

    }

}

void evaluer_politique_(float* f_eval, int* politique, bool *E_gagnant, bool *E_perdant, int taille){
/* Ecris la fonction de valeur de la politique rentre en paramètre
Paramètre :
f_eval : le tableau correspondant à la fonction de valeur de la politique
politique : la politique
E_gagnant, E_perdant : la liste des etats perdant/gagnant
taille : la taille d'un des tableaux rentrés en paramètres
*/

float eps = 1.f;
float delta = 0.f;
int compteur = 0;

do{

delta = 0.f;
for(int k = 0; k < taille; k++){

    int r = 0; float prec = 0.f;

    if(politique[k] != -1){

        /*Calcul de recompense*/
    if(E_gagnant[politique[k]]){
    r = 100;
    }else if(E_perdant[politique[k]]){
    r = -100;
    }
        /*Iteration et calcul du delta*/
        prec = f_eval[k];
        f_eval[k] = r + f_eval[politique[k]];
        if(f_eval[k]-prec > 0 && f_eval[k]-prec > delta){
            delta = f_eval[k]-prec;
        }else if(f_eval[k]-prec < 0 && -f_eval[k]+prec > delta){
            delta = -f_eval[k]+prec;
        }
    }else{
    if(E_gagnant[k] == true){
        r = 100;
    }else if(E_perdant[k] ==true){
        r = -100;
    }
    f_eval[k] = r;
    }

}

compteur++;

}while(delta > eps && compteur < 15000);

}

void aff_fc_valeur(float* fc_valeur, int taille){
/* Affiche la fonction de valeur passé en paramètre*/
for(int k = 0;k < taille; k++){

    printf("%d | %f\n", k, fc_valeur[k]);

}

}

void aff_table(int *table){
/* Permet d'afficher le jeu passer en paramètre */
printf("  -------\n");
for(int k = 0; k < 3; k++){
    char c[] = {' ', ' ', ' '};
    for(int j = 0; j < 3; j++){
        if(table[k*3+j] == 2){
            c[j] = 'X';
        }
        if(table[k*3+j] == 1){
            c[j] = 'O';
        }
    }
    printf("  |%c|%c|%c|\n", c[0], c[1], c[2]);
}
printf("  -------\n");
}

struct liste_int *etats_possible(int indice, bool tourAgent){
/* Retourne la liste des etats possible

Paramètres :
indice : le fomat du jeu dont il faut déterminer les états possible au format int
tourAgent : est ce que cela est à l'agent de jouuer

Retourne ; la liste des etats possible */

    int *table = c_ind_table(indice);
    int compteur = 0;
    int pion = tourAgent ? 2 : 1;

    for(int i = 0; i< 9; i++){
        if(table[i] == 0){
            compteur++;
        }
    }

    int *retour = malloc(compteur*sizeof(int));
    compteur = 0;
    for(int i = 0; i< 9; i++){
        if(table[i] == 0){
            retour[compteur] = indice+(int)pow(3, i)*pion;
            compteur++;
        }
    }

    struct liste_int *r = malloc(sizeof(struct liste_int));
    r->taille = compteur;
    r->liste = retour;

    return r;
}

int *creer_politique(int taille){
/* Permet de creer une politique de départ

Paramètre :
taille : la taille de la politique

Retourne : la politique
*/

int *politique = malloc(taille*sizeof(int));
struct liste_int *l;

for(int i = 0; i < taille; i++){
l = etats_possible(i, true);
if(l->taille > 0){
politique[i] = l->liste[0];
}else{
politique[i] = -1;
}

}

free(l);

return politique;
}

int *opti_politique(bool *E_gagnant, bool *E_perdant){

bool eststable = true;

int *politique = creer_politique(p39);
float *fonction_valeur = malloc(p39*sizeof(float));
struct liste_int *liste;
int compteur = 0;

do{
eststable = true;

evaluer_politique_(fonction_valeur, politique, E_gagnant, E_perdant, p39);

for(int k = 0;k < p39; k++){

    liste = etats_possible(k, true);


    if(liste->taille > 0){
        ///Meilleur action

        float maxi = fonction_valeur[liste->liste[0]];
        int maxi_ind = 0;

        for(int i = 0; i < liste->taille; i++){
            if(maxi < fonction_valeur[liste->liste[i]]){
                maxi_ind = i;
                maxi = fonction_valeur[liste->liste[i]];
            }
        }

        if(politique[k] != liste->liste[maxi_ind]) eststable = false;
        politique[k] = liste->liste[maxi_ind];
    }

}

compteur++;

}while(!eststable && compteur < 2500);

free(fonction_valeur);

return politique;

}

