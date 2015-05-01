xcopy "Simu\src\Carto\*.cpp" "Cartographie\cartopicsimu"
xcopy "Simu\src\Carto\*.h" "tmpCarto\"
del tmpCarto\config_carto.h /f /q
xcopy "*tmpCarto\.h" "Cartographie\cartopicsimu"

del Cartographie\cartopicsimu\astar.c /f /q
del Cartographie\cartopicsimu\carto.c /f /q
del Cartographie\cartopicsimu\data.c /f /q
del Cartographie\cartopicsimu\malloc.c /f /q
del Cartographie\cartopicsimu\help_main.c /f /q
del Cartographie\cartopicsimu\message.c /f /q
del Cartographie\cartopicsimu\uart.c /f /q

Ren "Cartographie\cartopicsimu\astar.cpp" "astar.c"
Ren "Cartographie\cartopicsimu\carto.cpp" "carto.c"
Ren "Cartographie\cartopicsimu\data.cpp" "data.c"
Ren "Cartographie\cartopicsimu\malloc.cpp" "malloc.c"
Ren "Cartographie\cartopicsimu\help_main.cpp" "help_main.c"
Ren "Cartographie\cartopicsimu\message.cpp" "message.c"
Ren "Cartographie\cartopicsimu\uart.cpp" "uart.c"