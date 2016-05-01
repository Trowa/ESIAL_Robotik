#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include "srf04.h"

/*
 * Vu ce que fait ce programme, il est indispensable de le lancer avec
 * le scheduler SCHED_FIFO. Du coup, on va le faire ici et crasher
 * lamentablement si on ne réussit pas à changer le scheduler.
 * Si vous n'avez rien compris : man sched_setscheduler
 * et man 7 sched
 */
int main(int argc, char *argv[])
{
    int error_code = 0;

    // Nombre de capteurs
    int nb_capteurs = 0;

    // Les capteurs
    srf04_t *capteurs = NULL;

    // Il faut une structure spéciale pour sched_setscheduler
    struct sched_param scheduler_params;
    /*
     * Priorité temps réel. Si on est le seul process en SCHED_FIFO,
     * la valeur n'a pas d'importance
     */
    scheduler_params.sched_priority = 50;

    // On change de scheduler
    error_code = sched_setscheduler(getpid(), SCHED_FIFO, &scheduler_params);
    if(error_code)
    {
        // Ça ne sert à rien de continuer
        perror("Impossible de changer de scheduler!");
        exit(1);
    }

    // On va lire les GPIOs sur la ligne de commande.
    // On a besoin de 2 GPIO par capteurs.
    // Convention pour la ligne de commande:
    // srf04 <capteur1_in> <capteur1_out> <capteur2_in> <capteur2_out> ...

    // On vérife le nombre d'arguments. argv[0] (nom du programme) compte
    // comme un argument
    if(argc <= 1 || (argc - 1) % 2)
    {
        // Pas assez d'arguments, ou nombre d'arguments impair
        fprintf(stderr, "Mauvais nombre d'argument !\n");
        exit(1);
    }

    nb_capteurs = (argc - 1) / 2;

    capteurs = malloc(sizeof(srf04_t) * nb_capteurs);

    // On instancie les capteurs
    int capteur_index;
    for(capteur_index = 0; capteur_index < nb_capteurs; capteur_index++)
    {
        // Les numéros des GPIO
        int gpio_in = atoi(argv[capteur_index*2 + 1]);
        int gpio_out = atoi(argv[capteur_index*2 + 2]);

        if(gpio_in <= 0 || gpio_out <= 0)
        {
            // A priori, on a lu n'importe quoi, alors on crashe violemment !
            fprintf(stderr, "Arguments invalides !\n");
            exit(1);
        }

        // Un nouveau capteur
        printf("Création d'un srf, in = %d, out = %d\n", gpio_in, gpio_out);
        capteurs[capteur_index] = srf04_new(gpio_in, gpio_out);
    }


    while(1)
    {
        // On va balancer les résultats sur la sortie standard
        // Format de la sortie: une ligne par jeu de mesure, avec sur chaque
        // ligne, la mesure de tous les capteurs, séparés par le caractère '#'
        // Les valeurs seront en mm
        for(capteur_index = 0; capteur_index < nb_capteurs; capteur_index++)
        {
            long mesure = srf04_get_mesure(capteurs[capteur_index]);

            if(mesure < 0)
            {
                // Mesure invalide ? on renvoie 0
                mesure = 0;
            }
            else
            {
                // On divise par 5800 pour avoir une valeur en mm
                mesure /= 5800;
            }

            if(capteur_index > 0)
            {
                // C'est pas le premier capteur, donc on ajoute '#' comme séparateur
                // avant de renvoyer la mesure
                printf("#");
            }

            // La mesure
            printf("%ld", mesure);

            // Il faut attendre au moins 10ms entre 2 mesures
            usleep(12000);
        }

        // On termine la ligne
        printf("\n");

        // On attend pour la prochaine série
        usleep(50000);
    }

}
