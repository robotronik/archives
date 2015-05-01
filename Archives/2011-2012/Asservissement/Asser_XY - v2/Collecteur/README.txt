CollecteurWin.exe : Exécutable pour Windows	(make)
CollecteurLin.exe : Exécutable pour Linux	(make PLATFORM=linux)
Compilation nécessaire si modification du programme d'asservissement
Les variables globales à afficher sont écrites dans Vars.c.
Le fichier d'entrée se génére par UARTerminal (voir Tutorials/Adaptation asser XY.txt, chapitre debug).
Pour voir les graphique, donner le fichier de sortie au Visionneur.
Le nom du fichier d'entrée peut être donné en argument avec "i=NOM". Par défaut, il vaut "Input.bin".
Le nom du fichier de sortie peut être donné en argument avec "o=NOM". Par défaut, il vaut "Output.bin".
Pour plus d'informations : xavier.gerault@grenoble-inp.org