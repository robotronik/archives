/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#ifndef DEBUG_H
#define DEBUG_H

/** \file debug.h
 * \brief Contient les fonctions utile pour le débug
 */


/** Code de débug à afficher sur les led de debug
 */
typedef enum {
    // Les leds seules
    DEL_1 = 0b001,
    DEL_2 = 0b010,
    DEL_3 = 0b100,

    // Des messages plus complexes
    DEBUG_MSG = 0x100,  // NB: cette valeur doit contenir un seul 1 et doit être
                        // plus grande que tout les autres codes

    // debug par numéro
    DEBUG_0 = 0                    ,
    DEBUG_1 = DEL_1                ,
    DEBUG_2 =         DEL_2        ,
    DEBUG_3 = DEL_1 | DEL_2        ,
    DEBUG_4 =                 DEL_3,
    DEBUG_5 = DEL_1         | DEL_3,
    DEBUG_6 =         DEL_2 | DEL_3,
    DEBUG_7 = DEL_1 | DEL_2 | DEL_3,
} CODE_DEBUG;

/** Initialise les ressources utilisées par le debug
 */
void initDebug(void);

/** Affiche un code de débug sur les LED
 *
 * Exemple d’utilisation :
 *  - Alumer la DEL1 et éteindre les autres : affiche_debug(DEL_1);
 *  - Eteindre toutes les DELs : affiche_debug(DEBUG_0);
 *  - Afficher les DEL 1 et 3, mais pas la 2 : affiche_debug(DEBUG_5)
 *  - Afficher un message de debug : affiche_debug(DEBUG_MSG | 0b11001010)
 *
 * NB : Un message de débug est une suite de 8 bits qui seront emis sur la DEL_1 pendant 250 ms chacuns.
 *
 */
void affiche_debug(CODE_DEBUG code);

/** Effectue une pause active de n milisecondes
 */
void pause_ms(unsigned short n);

/** Effectue une pause active de n secondes
 */
void pause_s(unsigned short n);

#endif /* DEBUG_H */
