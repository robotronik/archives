#ifndef TIMER_H
#define TIMER_H

#define PRESCALLEUR_MATCH 256
#define TEMPS_MATCH_SEC   90

/** La fin du match à lieu au bout de 
  * FREQUENCE_CLOCK/PRESCALLEUR_MATCH*TEMPS_MATCH_SEC/2 
  * = D48134,FC pour 90s en 2014
  * NB: la division est faite au début du calcul pour éviter l’overflow
  * NB: la division par 2 viens du fait que des fronts d’horloges
  */
#define NB_TIC_MATCH FREQUENCE_CLOCK/PRESCALLEUR_MATCH*TEMPS_MATCH_SEC/2



typedef union {
    unsigned long int all;
    struct {
            unsigned int lower;
            unsigned int higher;
        };
} Int16_32;

/** Initialise le timer utilisé pour mesuré le temps du match
 */
void initTimer();

/** Lire la valeur courante du timer
 * la valeur retourné est un nombre de tics correspondant à 20 milisecondes
 */
unsigned long int get_time();

/** Remet le timer du match à zéro
 */
void start_timer_match();


/* Renvoie vrai si le match est terminé
 * \warning Non testé
 */
bool timeEnd();

#endif /* TIMER_H */
