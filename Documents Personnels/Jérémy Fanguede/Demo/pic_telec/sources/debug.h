/*-----------------------------------------------------*
 * Utile pour du debug                                 *
 *-----------------------------------------------------*/

#ifndef _DEBUG_H
#define _DEBUG_H

//#define DATA 500	// [per] Période d'envoie de donnees

// DEL
void allumer_del(void);
void eteindre_del(void);

// Time
void pause_ms(unsigned short n);
void pause_s(unsigned short n);

#define debug 																		\
	/* if (cpt_boucle == 0) send_long(distL);										\
	if (cpt_boucle == 0) send_long(distR); */										\
																					\
	/* if (cpt_boucle == 0) send_long(delta); */									\
	/* if (cpt_boucle == 0) send_long(alpha); */									\
																					\
	/* if (cpt_boucle == 0) send_long((unsigned long)posX.part.high*3000/0xFFFF); */\
	/* if (cpt_boucle == 0) send_long((unsigned long)posY.part.high*3000/0xFFFF); */\
																					\
	/* if (cpt_boucle == 0) send_long((unsigned long)posAlpha*360/TOUR);			\
	if (cpt_boucle == 0) send_long(0xACCCCCCA); */									\
																					\
	/* if (cpt_boucle == 0) send_long((unsigned long)erreurAlpha*360/TOUR); */		\
	/* if (cpt_boucle == 0) send_long((unsigned long)consAlpha*360/TOUR); */		\
	/* if (cpt_boucle == 0) send_long(consMAlpha); */								\
																					\
	/* if (cpt_boucle == 0) send_long(vitAlpha*vitAlpha*COEF_FREIN_ALPHA); */		\
	/* if (cpt_boucle == 0) send_long(nbCommande); */								\
	/* if (cpt_boucle == 0) send_long(commandeCourante); */							\
	/* if (cpt_boucle == 0) send_long(consPrecision); */							\
	/* if (cpt_boucle == 0) send_long((unsigned long)consigneX*3000/0xFFFF); */		\
	/* if (cpt_boucle == 0) send_long((unsigned long)consigneY*3000/0xFFFF); */		\
	/* if (cpt_boucle == 0) send_long(vitAlpha); */									\
	/* if (cpt_boucle == 0) send_long(vitDelta); */									\
	/* if (cpt_boucle == 0) send_long(sinAlpha); */									\
	/* if (cpt_boucle == 0) send_long(erreurX); */									\
	/* if (cpt_boucle == 0) send_long(cosAlpha); */									\
	/* if (cpt_boucle == 0) send_long(erreurY); */									\
	/* if (cpt_boucle == 0) send_long(erreurX*3000/0xFFFF); */						\
	/* if (cpt_boucle == 10) send_long(erreurY*3000/0xFFFF); */						\
	/* if (cpt_boucle == 0) send_long(erreurCarre); */								\
	/* if (cpt_boucle == 0) send_long(dFreinageCarre); */							\
	/* if (cpt_boucle == 0) send_long(erreurCarre - dFreinageCarre); */				\
	/* if (cpt_boucle == 0) send_long(consMAlpha); */								\
	/* if (cpt_boucle == 0) send_long(consMDelta); */								\
	/* if (cpt_boucle == 0) send_long(consR); */									\
	/* if (cpt_boucle == 0) send_long(commandeL); */								\
	/* if (cpt_boucle == 0) send_long(commandeR); */								\
	/* if (cpt_boucle == 0) send_long(commandeR + commandeL); */					\
	/* if (cpt_boucle == 0) send_long(commandeR - commandeL); */					\
																					\
	cpt_boucle ++;																	\
	cpt_boucle %= DATA;

#define debugCourbe								\
	/* if (cpt_boucle == 0) send_long(diffX);	\
	if (cpt_boucle == 0) send_long(diffY);		\
	if (cpt_boucle == 0) send_long(rayon); */

#endif
