#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>

#include "fonction.h"

#define p39 19683

void jouer_partie(bool *E_gagnant, bool *E_perdant, int* politique, bool premiere_partie);

int main()
{

    bool* E_gagnant = malloc(p39*sizeof(bool));
    bool* E_perdant = malloc(p39*sizeof(bool));

    etat_finaux_(E_perdant, p39, false); etat_finaux_(E_gagnant, p39, true);

    int *politique = opti_politique(E_gagnant, E_perdant);

    jouer_partie(E_gagnant, E_perdant, politique, true);

    free(E_gagnant); free(E_perdant);
    free(politique);

    return 0;
}

void jouer_partie(bool *E_gagnant, bool *E_perdant, int* politique, bool premiere_partie){

    ///Initialisation

    int table = 0;
    bool tourAgent = true;
    bool fin = false;
    char nouvelle_partie = 'N';

    // Necessaire pour valider le coups jouer par l'utilisateur

    int ana_table = 0;
    int pion = 2;
    bool coup_valide = false;
    struct liste_int *ana_coup_possible;

    int i = 0;

    if(premiere_partie){
        printf("-- Morpion Agent --\n\n Essayer de gagner au morpion face a l'I.A \n\n  Numerotation des cases \n\n");
        printf("   -------\n   |1|2|3| \n   ------- \n   |4|5|6| \n   ------- \n   |7|8|9| \n   -------\n\n");
        printf("----- Debut du jeu -----\n\n");
    }else{
        printf(" -- Nouvelle partie --\n\n");
    }


    while(!fin){

        ana_table = table;
        coup_valide = false;

        ///Entrer utilisateur & Validation du coup

        if(!tourAgent){
            int *t = c_ind_table(table);
            aff_table(t);
            free(t);

            printf("\nEntrer le numero de la case a jouer : ");
            scanf("%d", &i);

            if(0 < i < 10){
                ana_table = ana_table + (int)pow(3, i-1)*pion;
                ana_coup_possible = etats_possible(table, tourAgent);
                for(int i = 0; i < ana_coup_possible->taille; i++){
                    if(ana_coup_possible->liste[i]==ana_table) coup_valide = true;
                }
            }

        }else{
            ana_table = politique[table];
            coup_valide = true;
        }

        if(coup_valide){

            ///Application du coup & Fin de partie ?

            table = ana_table;
            tourAgent = tourAgent ? false:true;
            pion = tourAgent ? 2:1;

            if(tourAgent == false && E_gagnant[table] == true){

                int *t = c_ind_table(table);
                aff_table(t);
                free(t);

                printf("\nVous avez perdu -- Dommage\n");
                fin = true;

            }
            if(tourAgent == true && E_perdant[table] == true){

                printf("Vous avez gagne -- Bravo\n");
                fin = true;

            }else if(etats_possible(table, tourAgent)->taille == 0){
                printf("Egalite -- Fin de partie\n");
                fin = true;
            }
        }

}

free(ana_coup_possible);

printf("Voulez-vous faire une nouvelle partie ? O/N\n");
scanf(" %c", &nouvelle_partie);

if(nouvelle_partie == 'O'){
    jouer_partie(E_gagnant, E_perdant, politique, false);
}

}
