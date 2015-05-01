#ifndef _ALPHADELTA_H
#define _ALPHADELTA_H

#define TAILLE_TAB_VIT	8

extern long delta, alpha;
extern short vitDelta, vitAlpha;
extern long deltaPrec[TAILLE_TAB_VIT];	// [tck]
extern unsigned short indexTab;

inline void getVitMoy(void);
inline void getDistArret(void);

#endif
