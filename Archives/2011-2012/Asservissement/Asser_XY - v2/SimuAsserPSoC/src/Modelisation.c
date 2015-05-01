#include "Modelisation.h"
#include "../../sources/motors.h"

#include <unistd.h>

#define CONFIG_PERTES
#define CONFIG_DERAPAGE

#define FREQ		ASSER_FREQ

// s = FREQ(z-1)/z

#define R_ROUE		((double) 0.035)				// [m/rad] et [m]
#define VIT_MAX		((double) 5/R_ROUE)				// [rad/s]		(<=> 5 m/s)
#define K			((double) VBAT_MAX/VIT_MAX)		// [mV/(rad/s)]	(= VBAT_MAX à VIT_MAX)

#ifdef CONFIG_PERTES
#define POIDS_ROUE	((double) POIDS/3)				// [Kg]			Masse du robot portée par une roue motrice
#define J			((double) 0.000712)				// [Kg.m²]		Moment d'inertie sur l'axe du rotor
#define R			((double) 5)					// [Ohm]
#define L			((double) 0.05)					// [H]
// Moteur : 0.36 N.m pour 1.5 A
#define F			((double) 0.001)				// [N.m.s/rad]	Coéfficient de frottement visqueux
#define TP_MAX		((double) 0.05)					// [N.m]		Pertes statiques

#else	// #ifdef CONFIG_PERTES
#define POIDS_ROUE	((double) 0)					// [Kg]			Poids du robot
#define J			((double) 0)					// [Kg.m²]		Moment d'inertie de l'axe du rotor
#define R			((double) 0)					// [Ohm]
#define L			((double) 0)					// [H]
// Moteur : 0.36 N.m pour 1.5 A
#define F			((double) 0)					// [N.m.s/rad]	Coéfficient de frottement visqueux
#define TP_MAX		((double) 0)					// [N.m]		Pertes statiques
#endif	// #ifdef CONFIG_PERTES

#define COEFF_ADHERENCE		((double) 0.005)		// Dérape facilement
#define COEFF_FROTTEMENT	((double) 0.001)		// COEFF_FROTTEMENT < ou = COEFF_ADHERENCE

typedef struct {
	int u[2];
	double a[3];	// [rad/s²] Accélération de la roue (même si dérapage)
	double v;		// [m/s]
	double tp[3];	// [N.m]
	double j[2];	// [Kg.m²]
	boolean is_slipping;
} motor_t;

inline double motors_run(int u, boolean side, double vit_sol) {
	static motor_t motor[2] = {{{0}, {0}, 0, {0}, {J + POIDS_ROUE / R_ROUE / R_ROUE}, FALSE}, 
		{{0}, {0}, 0, {0}, {J + POIDS_ROUE / R_ROUE / R_ROUE}, FALSE}};
	motor_t *m = motor + side;
	double a;
	
	m->u[1] = m->u[0];
	m->u[0] = u;
	m->j[1] = m->j[0];
	m->j[0] = J + POIDS_ROUE / R_ROUE / R_ROUE;
	m->a[2] = m->a[1];
	m->a[1] = m->a[0];
	// m->a[0] = ((m->u[0] - m->u[1])*K + m->a[1]*(j*(R+2*L*FREQ)+F*L) - m->a[2]*L*FREQ*j) / (R*j + L*FREQ*j + L*F + (R*F + K*K)/FREQ);
	m->a[0] = ((m->u[0] - m->u[1])*K + m->a[1]*FREQ*(m->j[0]*R+(3*m->j[0] - m->j[1])*L*FREQ+F*L) - m->a[2]*L*FREQ*FREQ*m->j[0]) 
		/ (FREQ*((R + L*FREQ)*(2*m->j[0] - m->j[1])) + (R + L*FREQ)*F + K*K);
	
#ifdef CONFIG_DERAPAGE
	// Dérapage
	if (m->a[0] * R_ROUE * m->j[0] > COEFF_ADHERENCE * POIDS_ROUE || m->a[0] * R_ROUE * m->j[0] < -COEFF_ADHERENCE * POIDS_ROUE || (m->is_slipping && (m->v < vit_sol - 0.000001 || m->v > vit_sol + 0.000001))) {
		m->is_slipping = TRUE;
		m->j[0] = J; // Pas de pertes par l'inertie du robot sur la rotation de la roue
		
		// Calcul de l'accélération de la roue
		m->a[0] = ((m->u[0] - m->u[1])*K + m->a[1]*FREQ*(m->j[0]*R+(3*m->j[0] - m->j[1])*L*FREQ+F*L) - m->a[2]*L*FREQ*FREQ*m->j[0]) 
			/ (FREQ*((R + L*FREQ)*(2*m->j[0] - m->j[1])) + (R + L*FREQ)*F + K*K);
		
		// Pertes par frottement
		m->tp[2] = m->tp[1];
		m->tp[1] = m->tp[0];
		if      (m->v < vit_sol && m->a[0]*m->j[0] < -COEFF_FROTTEMENT * POIDS_ROUE) m->tp[0] = -COEFF_FROTTEMENT * POIDS_ROUE;
		else if (m->v > vit_sol && m->a[0]*m->j[0] >  COEFF_FROTTEMENT * POIDS_ROUE) m->tp[0] =  COEFF_FROTTEMENT * POIDS_ROUE;
		else                                                                         m->tp[0] = m->a[0]*m->j[0];
		m->a[0] -= (R*FREQ*(m->tp[0] - m->tp[1]) + L*FREQ*FREQ*(m->tp[0] - 2*m->tp[1] + m->tp[2])) 
			/ (FREQ*((R + L*FREQ)*(2*m->j[0] - m->j[1])) + (R + L*FREQ)*F + K*K);
	} else {
		m->is_slipping = FALSE;
#endif	// #ifdef CONFIG_DERAPAGE
	
	// Pertes statiques
	m->tp[2] = m->tp[1];
	m->tp[1] = m->tp[0];
	if (m->a[0]*m->j[0] < -TP_MAX)     m->tp[0] = -TP_MAX;
	else if (m->a[0]*m->j[0] > TP_MAX) m->tp[0] =  TP_MAX;
	else                               m->tp[0] = m->a[0]*m->j[0];
	m->a[0] -= (R*FREQ*(m->tp[0] - m->tp[1]) + L*FREQ*FREQ*(m->tp[0] - 2*m->tp[1] + m->tp[2])) 
		/ (FREQ*((R + L*FREQ)*(2*m->j[0] - m->j[1])) + (R + L*FREQ)*F + K*K);
	
#ifdef CONFIG_DERAPAGE
	}
	
	if (m->is_slipping) {
		/* a = F / POIDS_ROUE; F = COEFF_FROTTEMENT * POIDS_ROUE; */
		if      (vit_sol - COEFF_FROTTEMENT / ASSER_FREQ > m->v) a = -COEFF_FROTTEMENT;
		else if (vit_sol + COEFF_FROTTEMENT / ASSER_FREQ < m->v) a =  COEFF_FROTTEMENT;
		else                                                     a =  ASSER_FREQ * (m->v - vit_sol);
	} else
#endif	// #ifdef CONFIG_DERAPAGE
	{
		a = m->a[0] * R_ROUE;
	}
	m->v += m->a[0] * R_ROUE / ASSER_FREQ;
	return a;
}
