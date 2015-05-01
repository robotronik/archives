/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#include "main.h"
#include "debug.h"
#include "mapping.h"

void initDebug(void) {
    TRIS_DEBUG_1 = 0;
    TRIS_DEBUG_2 = 0;    // DEL en mode output
    TRIS_DEBUG_3 = 0;
}

void affiche_debug(CODE_DEBUG code) {
    // NB: Les doubles !! permettent de s’assurer que l’on a un booléen !
        // \note les LEDs sont actives niveaux haut

    if (code < DEBUG_MSG) {
    // On affiche le code
        DEL_DEBUG_1 = !!(code & DEL_1);
        DEL_DEBUG_2 = !!(code & DEL_2);
        DEL_DEBUG_3 = !!(code & DEL_3);
    } else {
    // On affiche le message
        unsigned char mask_bit;

        for(mask_bit = (DEBUG_MSG >> 1) ; mask_bit != 0; mask_bit >>= 1) {
            DEL_DEBUG_1 = !!(mask_bit & code);
            //DEL_DEBUG_1 = !DEL_DEBUG_1;
            pause_ms(250);
        }
    }
}

void pause_ms(unsigned short n) {
    unsigned short i,j;
    for(j=0; j<n; j++)     {
         for(i=0; i<4000; i++)
            Nop();
    }
}

void pause_s(unsigned short n) {
    unsigned short i;
    for(i=0; i<n; i++)
        pause_ms(1000);
}

