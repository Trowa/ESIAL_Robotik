#include "srf04.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <time.h>

#define GPIO_PATH   "/sys/class/gpio/"
#define GPIO_EXPORT GPIO_PATH "export"

#define GPIO_DIRECTION  GPIO_PATH "gpio%d/direction"
#define GPIO_VALUE      GPIO_PATH "gpio%d/value"
#define GPIO_EDGE       GPIO_PATH "gpio%d/edge"

#define BUFFER_SIZE 128

struct s_srf04
{
    int fd_gpio_in;  // Input pour nous: ce qui vient du capteur
    int fd_gpio_out; // Output pour nous: permet l'activation du capteur
};

static int wait_value(int fd, char value)
{
    // Pour la fonction poll, il faut une structure spéciale
    struct pollfd poll_fd;
    int error_code = 0;
    char value_read = '\0';

    // On limite le nombre d'itération, en cas de problème
    while(value_read != value)
    {
        // On a configuré des interruptions, donc on va les utiliser

        poll_fd.fd = fd;
        poll_fd.events = POLLPRI | POLLERR;
        poll_fd.revents = 0;

        // On attend...
        error_code = poll(&poll_fd, 1, 100);

        if(error_code <= 0)
        {
            // Un problème est survenu
            return -1;
        }

        // On a reçu une interruption ! On lit la valeur pour vérifier...
        lseek(fd, 0, SEEK_SET);
        read(fd, &value_read, 1);
    }


    return 0;
}

/**
 * Constructeur, parce qu'on fait du C orienté objet
 */
srf04_t srf04_new(int gpio_in, int gpio_out)
{
    srf04_t srf04 = NULL;
    int     fd_export = -1;
    int     fd_direction = -1;
    int     fd_edge = -1;
    char    buffer[BUFFER_SIZE];

    // La mémoire pour l'objet
    srf04 = malloc(sizeof(struct s_srf04));

    // Il faut "exporter" les GPIOs
    fd_export = open(GPIO_EXPORT, O_WRONLY);

    snprintf(buffer, BUFFER_SIZE, "%d\n", gpio_in);
    write(fd_export, buffer, strlen(buffer));

    // Oui, c'est du code copié collé
    snprintf(buffer, BUFFER_SIZE, "%d\n", gpio_out);
    write(fd_export, buffer, strlen(buffer));

    close(fd_export);

    // On met le GPIO in en "input"
    snprintf(buffer, BUFFER_SIZE, GPIO_DIRECTION, gpio_in);
    fd_direction = open(buffer, O_WRONLY);
    write(fd_direction, "in\n", strlen("in\n"));
    close(fd_direction);

    // Pour mettre le GPIO en output, il ne doit pas avoir d'interruption configuré
    snprintf(buffer, BUFFER_SIZE, GPIO_EDGE, gpio_out);
    fd_edge = open(buffer, O_WRONLY);
    write(fd_edge, "none\n", strlen("none\n"));
    close(fd_edge);

    // On met le GPIO out en "output"
    snprintf(buffer, BUFFER_SIZE, GPIO_DIRECTION, gpio_out);
    fd_direction = open(buffer, O_WRONLY);
    write(fd_direction, "out\n", strlen("out\n"));
    close(fd_direction);

    // Pour le GPIO in, on configure des interruptions
    snprintf(buffer, BUFFER_SIZE, GPIO_EDGE, gpio_in);
    fd_edge = open(buffer, O_WRONLY);
    write(fd_edge, "both\n", strlen("both\n"));
    close(fd_edge);

    // On ouvre enfin les fichiers "value"
    snprintf(buffer, BUFFER_SIZE, GPIO_VALUE, gpio_in);
    srf04->fd_gpio_in = open(buffer, O_RDONLY);

    snprintf(buffer, BUFFER_SIZE, GPIO_VALUE, gpio_out);
    srf04->fd_gpio_out = open(buffer, O_WRONLY);
    // On met la sortie à 0
    write(srf04->fd_gpio_out, "0\n", strlen("0\n"));

    return srf04;
}

/**
 * Renvoie la durée de l'impulsion de réponse du capteur.
 * C'est la mesure brute, en nanosecondes. D'après la doc, il faut diviser
 * par 5800 pour avoir une valeur en mm.
 */
long srf04_get_mesure(srf04_t srf04)
{
    long mesure = -1;
    int  error_code = 0;

    // Pour la fonction clock_gettime, il faut aussi une structure spéciale
    struct timespec time_start;
    struct timespec time_end;

    time_t diff_seconds = 0;

    /*
     * Pour faire la mesure, il faut:
     * - envoyer une impulsion d'au moins 10 us sur la GPIO out,
     * - attendre la réponse sur la GPIO in
     * Le capteur répond en mettant à 1 le GPIO pendant une certaine
     * durée, c'est cette durée qui donne la mesure de distance
     * Il est rudement conseillé de faire fonctionner le programme
     * avec le scheduler SCHED_FIFO: Linux schedulera le processus
     * dès que la GPIO in change de valeur, et donc la mesure de temps
     * sera plus précise.
     */

    // Activation de la GPIO "out"
    write(srf04->fd_gpio_out, "1\n", strlen("1\n"));
    usleep(15); // Petite pause...
    write(srf04->fd_gpio_out, "0\n", strlen("0\n")); // Et on coupe !

    // On attend le front montant
    error_code = wait_value(srf04->fd_gpio_in, '1');
    if(error_code < 0)
    {
        // Erreur !
        return -1;
    }

    // On enregistre le temps
    clock_gettime(CLOCK_MONOTONIC_RAW, &time_start);

    // Et on réattend le front descendant!
    error_code = wait_value(srf04->fd_gpio_in, '0');
    if(error_code < 0)
    {
        // Erreur !
        return -1;
    }

    // On enregistre le temps
    clock_gettime(CLOCK_MONOTONIC_RAW, &time_end);

    // On compare les durées
    diff_seconds = time_end.tv_sec - time_start.tv_sec;
    if(diff_seconds > 1)
    {
        // WTF ? On a attendu plus d'une seconde !
        fprintf(stderr, "temps > 1s\n");
        return -1;
    }

    // On calcule tout en nanosecondes
    mesure = 1000000000 * diff_seconds;
    mesure += (time_end.tv_nsec - time_start.tv_nsec);

    return mesure;
}
