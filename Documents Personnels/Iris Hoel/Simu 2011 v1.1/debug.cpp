

# include "main.hpp"

FILE* fichier=NULL;

void iniFichierDebug(){
    remove("C:\\Users\\H__o_l\\Desktop\\debug_robot.txt");
    fichier=fopen("C:\\Users\\H__o_l\\Desktop\\debug_robot.txt", "a");
    log("hello");
}

void closeFichierDebug(){
    log(" Fin");
    fclose(fichier);
}

void log(char* mot)
{
    fprintf(fichier,"\n%s", mot);
}

void log_arg(float i)
{
    fprintf(fichier," %lf", i);
}



void log_state(int i)
{
char* mot;
switch(i)
{
    case SORTIR_ZONE:
        mot="sortirZone";
        break;
    case SAISIR_1:
         mot="saisir1";
        break;
    case RETOUR_ZONE_DEP:
         mot="retourZoneDep";
        break;
    case AVANCER_LIGNE_1:
         mot="avancerLigne1";
        break;
    case SAISIR_2:
         mot="saisir2";
        break;
    case REMONTER_LIGNE_1:
         mot="remonterLigne1";
        break;
    case SAISIR_3:
         mot="saisir3";
        break;
    case DEPOT_ZONE_SURE:
         mot="depotZoneSure";
        break;
    case GO_LIGNE_2:
         mot="goLigne2";
        break;
    case SAISIR_4:
         mot="saisir4";
        break;
    case DEPOT_BONUS:
         mot="depotBonus";
        break;
    case AVANCER_LIGNE_2:
         mot="avancerLigne2";
        break;
    case SAISIR_5:
         mot="saisir5";
        break;
    case REDESCENDRE_LIGNE_2:
         mot="redescendreLigne2";
        break;
    case SAISIR_6:
         mot="saisir6";
        break;
    case DEPOT_4:
         mot="depot4";
        break;
    case FINIR_MONTER:
         mot="finirMonter";
        break;
    case FIN:
         mot = "fin";
        break;
    default:
     mot="?";
        break;
}
    fprintf(fichier,"\n                       %s", mot);
}





