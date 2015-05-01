/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#ifndef DEBUG_H
#define DEBUG_H

/** \file debug.h
 * \brief Contient les fonctions utile pour le débug
 * \todo Réécrire allumer_del pour qu¿elle prenne en argument le nom de la del de débug (par exemple une enum)
 */


//#define DATA 500	// [per] Période d'envoie de donnees

// DEL
/* \deprecated Utiliser code_debug() à la place
 */
void allumer_del(void);
/* \deprecated Utiliser code_debug() à la place
 */
void eteindre_del(void);

/** Code de débug à afficher sur les led de debug
 */
typedef enum CODE_DEBUG {
    // Les leds seules
    DEL_1 = 1,
    DEL_2 = 2,
    DEL_3 = 4,
    // Code de debug
    DEBUG_0 = 0                    ,
    DEBUG_1 = DEL_1                ,
    DEBUG_2 =         DEL_2        ,
    DEBUG_3 = DEL_1 | DEL_2        ,
    DEBUG_4 =                 DEL_3,
    DEBUG_5 = DEL_1         | DEL_3,
    DEBUG_6 =         DEL_2 | DEL_3,
    DEBUG_7 = DEL_1 | DEL_2 | DEL_3,
} CODE_DEBUG;

/** Affiche un code de débug sur les LED
 */
void affiche_debug(CODE_DEBUG code);

/** Effectue une pause active de n milisecondes
 */
void pause_ms(unsigned short n);

/** Effectue une pause active de n secondes
 */
void pause_s(unsigned short n);


#endif /* DEBUG_H */
