######################################### 
 	"IA" du robot principal de 2013 
#########################################

- Description des sructures de données (constantes.h) :

*La structure Action represente une action à réaliser, le type de l'action est
defini dans .t qui est de type TypeAction. 
Peut etre un deplacement classique DEPL on a alors dans .dep si il s'agit 
d'un DELTA ou d'un ALPHA et sa valeur. Cela peut être un deplacement plus
complexe (XY_POS) avec gettage de la pos puis depl ...
Une action peut être également une AUTRE_ACTION càd un appel de 
fonction void(void). (ex : bouger un servo, changer la valeur d'une 
variable globale...). La valeur du pointeur de fonction est dans .proc
En plus de ces actions un appel à une fonction void(int) (.procInt) 
peut être fait en plus d'une action de type precendent, 
l'argument est fourni dans .argProc. Cela set notammnet à mettre à jour 
certaine variable comme l'etat des verres ou des bougies.
Des fonctions générant 1 ou plusieurs action se situent dans deplacement.c/h
ou forge.c/h.

*La structure Mission reprense une mission complete elle est généralement
crée à partir d'une fonction de forge.c/h. Cela peut être aller 
chercher les VERRES les RAMMENER, les BOUGIES, ... (contenu dans .t)
Pour cela une Mission contient un tableau d'Action de 100. (.tabAction),
ainsi qu'un index courant sur ce tableau (.numero). Une variable .fini
et mis à un par la machine à etat (MAE) mind quand la mission est fini.

*La structure Objectif contient un type de mission à réaliser (.t)
ainsi que d'autres variable :
  onlyIf => fait l'objectif quand la valeur pointé vaut 1
  endIf => arrete l'objectif quand la valeur pointé vaut 1
  endIfSoft => arrete l'objectif quand la valeur pointé vaut 1, mais fini l'action en cours

*Une structure Objectifs contient un tableau d'Objectif (.tab) un index courant
sur ce tableu (.numero). Elle permet d'enchinaer les mission (dans machineMind)



- Description du fonctionnement :

Il y a 3 machine à etats qui tourne en "parallele" (cf for(;;))
dans le main )
	- la machine emplileur qui gere l'empilage dse verre
	elle verifie si il ya un verre et l'empile quand il faut
	elle maintient à jour une varaiblee nbVerresEmpile qui indique
	le nombre de  verre empilé. pileDe4 quand il y a une pile de 4 ...
	On peut lui de demander de libérer les verres en mettant
	à un libererVerreMAE
	
	- la machine Go est chager de réaliser les actions contenu dans la variable 
	globale missionCourante.
	Lorsque obstacleMAE vaut 1 elle passe en etat STOP puis directement 
	dans l'etat REPRISE et rélise alors les actions de missionReprise
	tant que repriseMAE n'est pas a 1.
	
	- la machine Mind est charger d'enchiner les misison quand elles sont finies
	c'est elle qui rempli missionCourante grace aux fonctions de forge.c/h
	Elle arrete une mission/objectif si necessaire (cf struct Objectif).
	Elle met à 1 obstacleMAE, si necessaire en fonction des sharp. Et lorsque
	il n'y a plus d'obsatcle genere une missionReprise pour repartir.
	Si l'obsatcle dure trop longtemps elle fait appelle à la cartographie 
	(algo A*) pour générer une missionReprise et contourner l'obstacle.
	De plus la machine à état passe par des état mettant  jour la position
	du robot en fonction de l'asser.
	