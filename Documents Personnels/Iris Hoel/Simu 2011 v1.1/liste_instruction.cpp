
# include "main.hpp"





llist ajouterEnTete(llist liste, inst valeur)
{
    /* On cr�e un nouvel �l�ment */
    element* nouvelElement = (element*) malloc(sizeof(element));

    /* On assigne la valeur au nouvel �l�ment */
    nouvelElement->val = valeur;

    /* On assigne l'adresse de l'�l�ment suivant au nouvel �l�ment */
    nouvelElement->nxt = liste;

    /* On retourne la nouvelle liste, i.e. le pointeur sur le premier �l�ment */
    return nouvelElement;
}




llist ajouterEnFin(llist liste, inst valeur)
{
    /* On cr�e un nouvel �l�ment */
    element* nouvelElement = (element*) malloc(sizeof(element));

    /* On assigne la valeur au nouvel �l�ment */
    nouvelElement->val = valeur;

    /* On ajoute en fin, donc aucun �l�ment ne va suivre */
    nouvelElement->nxt = NULL;

    if(liste == NULL)
    {
        /* Si la liste est vide� il suffit de renvoyer l'�l�ment cr�� */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste � l'aide d'un pointeur temporaire et on
        indique que le dernier �l�ment de la liste est reli� au nouvel �l�ment */
        element* temp=liste;
        while(temp->nxt != NULL)
        {
            temp = temp->nxt;
        }
        temp->nxt = nouvelElement;
        return liste;
    }
}



void afficherListe(llist liste)
{
    element *tmp = liste;
    if(tmp == NULL)
    log("   null");
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        switch(tmp-> val.type)
        {
            case AVANCER:
            log("   Avancer ");
            break;
            case TOURNER:
            log("   tourner ");
            break;
            case FERMER_AVANT:
            log("   fermer avant ");
            break;
            case LIBERER_AVANT:
            log("   liberer avant ");
            break;
            case FERMER_ARRIERE:
            log("   fermer arriere");
            break;
            case LIBERER_ARRIERE:
            log("   liberer arriere");
            break;
            default:
            break;

        }

        /* On affiche */
        log_arg(tmp-> val.signe);
        log_arg(tmp-> val.valeur);
        log_arg(tmp-> val.vitesse);

        /* On avance d'une case */
        tmp = tmp->nxt;
    }
}


int estVide(llist liste)
{
    return (liste == NULL)? 1 : 0;
}



llist supprimerElementEnTete(llist liste)
{
    if(liste != NULL)
    {
        /* Si la liste est non vide, on se pr�pare � renvoyer l'adresse de
        l'�l�ment en 2�me position */
        element* aRenvoyer = liste->nxt;
        /* On lib�re le premier �l�ment */
        free(liste);
        /* On retourne le nouveau d�but de la liste */
        return aRenvoyer;
    }
    else
    {
        return NULL;
    }
}


llist supprimerElementEnFin(llist liste)
{
    /* Si la liste est vide, on retourne NULL */
    if(liste == NULL)
        return NULL;

    /* Si la liste contient un seul �l�ment */
    if(liste->nxt == NULL)
    {
        /* On le lib�re et on retourne NULL (la liste est maintenant vide) */
        free(liste);
        return NULL;
    }

    /* Si la liste contient au moins deux �l�ments */
    element* tmp = liste;
    element* ptmp = liste;
    /* Tant qu'on n'est pas au dernier �l�ment */
    while(tmp->nxt != NULL)
    {
        /* ptmp stock l'adresse de tmp */
        ptmp = tmp;
        /* On d�place tmp (mais ptmp garde l'ancienne valeur de tmp */
        tmp = tmp->nxt;
    }
    /* A la sortie de la boucle, tmp pointe sur le dernier �l�ment, et ptmp sur
    l'avant-dernier. On indique que l'avant-dernier devient la fin de la liste
    et on supprime le dernier �l�ment */
    ptmp->nxt = NULL;
    free(tmp);
    return liste;
}


llist effacerListe(llist liste)
{
    element* tmp = liste;
    element* tmpnxt;

    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        /* On stocke l'�l�ment suivant pour pouvoir ensuite avancer */
        tmpnxt = tmp->nxt;
        /* On efface l'�l�ment courant */
        free(tmp);
        /* On avance d'une case */
        tmp = tmpnxt;
    }
    /* La liste est vide : on retourne NULL */
    return NULL;
}


llist rechercherElement(llist liste, int type)
{
    element *tmp=liste;
    /* Tant que l'on n'est pas au bout de la liste */
    while(tmp != NULL)
    {
        if(tmp->val.type == type)
        {
            /* Si l'�l�ment a la valeur recherch�e, on renvoie son adresse */
            return tmp;
        }
        tmp = tmp->nxt;
    }
    return NULL;
}

