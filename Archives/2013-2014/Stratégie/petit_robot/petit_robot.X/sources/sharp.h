#ifndef SHARP_H
#define SHARP_H

/** file sharp.h
 *  \brief Contient les fonctions relatif à l’utilisation des Sharps
 */
#define DISTANCE_6CM             850
#define DISTANCE_18CM            400
#define DISTANCE_PROXIMITE       500
#define DISTANCE_ARRET_URGENCE   200 /*1500*/

/** Les shaps présent sur le petit robot
 *  \see SensMvt
 */
typedef enum {
    SHARP_1 = MARCHE_AVANT,
    SHARP_2 = MARCHE_ARRIERE
} Sharp;

/** Interruption qui permet d’obtenir la distance entre le robot et le sharp_1
 *  \todo Faire le même chose pour le sharp_2
 */
void __attribute__((__interrupt__)) _ADC1Interrupt(void);

/** Renvoie la distance avec le premier obstacle
 *  \note Plus la valeur est petite plus l’objet est loin
 *  \todo Utiliser le bon sharp (sur le robot de l’an dernier, il n’y en avait qu’un
 *  \warning En dessous de 8 cm les distances sont incohérentes
 *  \warning Pour le moment le résultat n’est pas en mm
 */
int getDistance(Sharp sharp);

/** Initialise le sharp
 */
void initSharp(void);

#endif /* SHARP_H */
