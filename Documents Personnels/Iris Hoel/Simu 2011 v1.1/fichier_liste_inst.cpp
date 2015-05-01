

/*
# include "main.hpp"
extern int BORDURES_BLANCHES;
extern int LARGEUR_FENETRE;
extern int HAUTEUR_FENETRE;
extern float RAPPORT_LARGEUR;
extern float RAPPORT_HAUTEUR;
extern int EPAISSEUR_PIXEL;
extern int DELAY;
extern float VITESSE_TRANS_ROBOT_DEFAUT;
extern float VITESSE_ROT_ROBOT_DEFAUT;
extern float VITESSE_PINCE_DEFAUT;
extern int ANGLE_PINCE_DEFAUT;


FILE* ouverture_fichier_liste_inst(int n)
{
    if(n > 99 | n < 0 )
    {
        return fopen("bouh chez ta maman", "r");
    }
    char nom_fichier[18] = "liste_inst_";
    if(n/10 == 0)
    {
        nom_fichier[11]=48+n;
        nom_fichier[12]='.';
        nom_fichier[13]='t';
        nom_fichier[14]='x';
        nom_fichier[15]='t';
        //nom_fichier[16]='\0';
    }
    else
    {

        nom_fichier[11]=48+(n/10);
        nom_fichier[12]=48+(n%10);
        nom_fichier[13]='.';
        nom_fichier[14]='t';
        nom_fichier[15]='x';
        nom_fichier[16]='t';
        //nom_fichier[17]='\0';

    }


    FILE* fichier = NULL;
    fichier = fopen(nom_fichier, "r");
    return fichier;


}


int longueur(FILE* fichier)
{
    char c = '1';
    int i = -1;
    while(c!='\n')
    {
        c = fgetc(fichier);
        i ++;
    }
    fseek(fichier, -(i+2), SEEK_CUR);
    return i;
}


float conversion(char * tab, int n)
{
int val = 0, val2 =0;
int i = 0;
int j = 0;
while((tab[i] != '.') && (i<n))
{

    val = val*10 + (int)(tab[i]-48);
    i++;
}
i ++;
if(i!=n)
{
    while(i<n)
    {
        val2 = val2*10 + (int)(tab[i]-48);
        i ++;
        j ++;
    }
}
return (float)((double)val+((double)val2)*pow(0.1,j));

}


int fin_fichier(FILE* fichier)
{

	char c = fgetc(fichier);
    //fseek(fichier, -1, SEEK_CUR);
	if(c == EOF)
        return 1;
    else
    {
        fseek(fichier, -1, SEEK_CUR);
        return 0;
    }

}


int lecture_type(FILE* fichier)
{
char type[20] = "";
fgets(type, 20, fichier);
if(strcmp(type, "AVANCER\n") == 0)
    return AVANCER;
if(strcmp(type, "TOURNER\n")== 0)
    return TOURNER;
if(strcmp(type, "LIBERER_AVANT\n")== 0)
    return LIBERER_AVANT;
if(strcmp(type, "FERMER_AVANT\n")== 0)
    return FERMER_AVANT;
if(strcmp(type, "LIBERER_ARRIERE\n")== 0)
    return LIBERER_ARRIERE;
if(strcmp(type, "FERMER_ARRIERE\n")== 0)
    return FERMER_ARRIERE;
return -1;
}


int lecture_signe(FILE* fichier)
{
if(!fin_fichier(fichier))
{
    int taille = longueur(fichier);
    //fseek(fichier, -1, SEEK_CUR);
    char c = fgetc(fichier);
    if(c == '-')
    {
        fseek(fichier, taille+1, SEEK_CUR);
        return -1;
    }
    else if (c == '+')
    {
        fseek(fichier, taille+1, SEEK_CUR);
        return 1;
    }
    else
    {
        fseek(fichier, -1, SEEK_CUR);
        return 0;
    }
}
else
    return 0;
}


float lecture_valeur(FILE* fichier)
{
if(!fin_fichier(fichier))
{
    float val =0;
    int taille = longueur(fichier);
    char type[20] = "";
    fgets(type, 20, fichier);
    if(type[0]<58 && type[0]>47)
    {
        val = conversion(type, taille);
        if(val < 3)
        {
            fseek(fichier, -(taille+2), SEEK_CUR);
            val = 0;
        }
    }
    else
    {
        val = 0;
        fseek(fichier, -(taille+2), SEEK_CUR);
    }
    return val;
}
else
    return 0;
}


float lecture_vitesse(FILE* fichier)
{
if(!fin_fichier(fichier))
{
    float val =0;
    int taille = longueur(fichier);
    char type[20] = "";
    fgets(type, 20, fichier);
    if(type[0]<58 && type[0]>47)
    {
        val = conversion(type, taille);
        if(val > 3)
        {
            fseek(fichier, -(taille+2), SEEK_CUR);
            val = 0;
        }
    }
    else
    {
        val = 0;
        fseek(fichier, -(taille+2), SEEK_CUR);
    }
    return val;
}
else
    return 0;

}


void par_defaut(inst* inst_temp)
{
    if((*inst_temp).type == -1)
        exit(0);
    switch((*inst_temp).type)
    {
        case AVANCER:
            if((*inst_temp).signe == 0)
                (*inst_temp).signe = 1;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_TRANS_ROBOT_DEFAUT;
        break;
        case TOURNER:
            if((*inst_temp).signe == 0)
                (*inst_temp).signe = 1;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_ROT_ROBOT_DEFAUT;
        break;
        default:
            if((*inst_temp).valeur == 0)
                (*inst_temp).valeur = ANGLE_PINCE_DEFAUT;
            if((*inst_temp).vitesse == 0)
                (*inst_temp).vitesse = VITESSE_PINCE_DEFAUT;
        break;
    }
}


llist lecture_liste_inst(llist liste, int n)
{
FILE* fichier = ouverture_fichier_liste_inst(n);
inst inst_temp;
while(!fin_fichier(fichier))
{
    inst_temp.type = lecture_type(fichier);
    inst_temp.signe = lecture_signe(fichier);
    inst_temp.valeur = lecture_valeur(fichier);
    inst_temp.vitesse = lecture_vitesse(fichier);
    par_defaut(&inst_temp);
    liste = ajouterEnFin(liste, inst_temp);
}

fclose(fichier);
return liste;

}
*/

