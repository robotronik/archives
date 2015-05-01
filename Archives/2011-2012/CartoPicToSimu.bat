xcopy "Cartographie\cartopicsimu\*.c" "Simu\src\Carto"
xcopy "Cartographie\cartopicsimu\*.h" "\tmpCarto"
del "tmpCarto\config_carto.h" /f /q
xcopy "tmpCarto\*.h" "Simu\src\Carto\"

del Simu\src\Carto\astar.cpp /f /q
del Simu\src\Carto\carto.cpp /f /q
del Simu\src\Carto\data.cpp /f /q
del Simu\src\Carto\malloc.cpp /f /q
del Simu\src\Carto\help_main.cpp /f /q
del Simu\src\Carto\message.cpp /f /q
del Simu\src\Carto\uart.cpp /f /q

Ren "Simu\src\Carto\astar.c" "astar.cpp"
Ren "Simu\src\Carto\carto.c" "carto.cpp"
Ren "Simu\src\Carto\data.c" "data.cpp"
Ren "Simu\src\Carto\malloc.c" "malloc.cpp"
Ren "Simu\src\Carto\help_main.c" "help_main.cpp"
Ren "Simu\src\Carto\message.c" "message.cpp"
Ren "Simu\src\Carto\uart.c" "uart.cpp"
pause