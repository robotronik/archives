xcopy "Simu\src\Soc\*.cpp" "PSoc_Coupe\Projet de base\CommandeAsser.cydsn"
xcopy "Simu\src\Soc\*.h" "tmpSoc\"
del "tmpSoc\config.h" /f /q
xcopy "tmpSoc\*.h" "PSoc_Coupe\Projet de base\CommandeAsser.cydsn"

del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\fonctionsPsoc.h" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\fonctionsToDo.h" /f /q

del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\choixObjectif.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\cluster.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\comAsser.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\comCarto.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\espion.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\machine.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\outils.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\recTourelle.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\remplirObj.c" /f /q
del "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\signaux.c" /f /q

Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\choixObjectif.cpp" "choixObjectif.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\cluster.cpp" "cluster.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\comAsser.cpp" "comAsser.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\comCarto.cpp" "comCarto.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\espion.cpp" "espion.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\machine.cpp" "machine.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\outils.cpp" "outils.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\recTourelle.cpp" "recTourelle.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\remplirObj.cpp" "remplirObj.c"
Ren "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\signaux.cpp" "signaux.c"

Pause