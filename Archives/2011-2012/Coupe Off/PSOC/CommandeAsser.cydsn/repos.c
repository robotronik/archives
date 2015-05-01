#include "repos.h"

void signalBloquant()
{
    stopAsserByBrake();
}

void calculerRepos()
{
    // Si la position demandee en cours est nulle
    // On en calcule une nouvelle
    // On prend le robot adverse
    // 5 choix d'angle de repos, on calcul un par un par lancer de rayon
    // avec la norme Dwarn - Drobot adverse
    // Si toujours rien, pareil mais avec la norme 20
}
