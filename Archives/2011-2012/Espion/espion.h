
#define DISTANCE_MIN2 100 // Distance au CARRÉ a la quelle on concidère que le robot adverse prend l'obectif


typedef struct {
	unsigned int X;
	unsigned int Y;
	unsigned short int nbPoints;

} ERobot;

typedef struct Objectifs {
	unsigned int id;
	unsigned int X;
	unsigned int Y;
	unsigned int nbPoints;
	char type; // 0 : Jeton. 2 : Totem. 3 : Base.

	struct Objectifs *suiv;
} Objectifs;

void espion( Objectifs *obj, ERobot robot1, ERobot robot2);
unsigned short int prendObjectif(unsigned int X, unsigned int Y, ERobot rb );
void supprimeObjectif(Objectifs *obj, unsigned int id);
void supprimeObjectif(Objectifs *obj, unsigned int id);
void ajoutObjectif(Objectifs *obj,unsigned int X, unsigned int Y, unsigned int nbPoints, char type);

