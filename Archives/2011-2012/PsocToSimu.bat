xcopy "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\*.c" "Simu\src\Soc"
xcopy "PSoc_Coupe\Projet de base\CommandeAsser.cydsn\*.h" "tmpSoc\"
del "tmpSoc\device.h" /f /q
del "tmpSoc\main.h" /f /q
del "tmpSoc\config.h" /f /q
del "Simu\src\Soc\main.c" /f /q
xcopy "tmpSoc\*.h" "Simu\src\Soc\"

del Simu\src\Soc\choixObjectif.cpp /f /q
del Simu\src\Soc\cluster.cpp /f /q
del Simu\src\Soc\comAsser.cpp /f /q
del Simu\src\Soc\comCarto.cpp /f /q
del Simu\src\Soc\espion.cpp /f /q
del Simu\src\Soc\machine.cpp /f /q
del Simu\src\Soc\outils.cpp /f /q
del Simu\src\Soc\recTourelle.cpp /f /q
del Simu\src\Soc\remplirObj.cpp /f /q
del Simu\src\Soc\signaux.cpp /f /q

Ren "Simu\src\Soc\choixObjectif.c" "choixObjectif.cpp"
Ren "Simu\src\Soc\cluster.c" "cluster.cpp"
Ren "Simu\src\Soc\comAsser.c" "comAsser.cpp"
Ren "Simu\src\Soc\comCarto.c" "comCarto.cpp"
Ren "Simu\src\Soc\espion.c" "espion.cpp"
Ren "Simu\src\Soc\machine.c" "machine.cpp"
Ren "Simu\src\Soc\outils.c" "outils.cpp"
Ren "Simu\src\Soc\recTourelle.c" "recTourelle.cpp"
Ren "Simu\src\Soc\remplirObj.c" "remplirObj.cpp"
Ren "Simu\src\Soc\signaux.c" "signaux.cpp"

pause