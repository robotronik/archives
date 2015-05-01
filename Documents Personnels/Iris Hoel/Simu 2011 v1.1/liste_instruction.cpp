
# include "main.hpp"





llist ajouterEnTete(llist liste, inst valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = (element*) malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On assigne l'adresse de l'élément suivant au nouvel élément */
    nouvelElement->nxt = liste;

    /* On retourne la nouvelle liste, i.e. le pointeur sur le premier élément */
    return nouvelElement;
}




llist ajouterEnFin(llist liste, inst valeur)
{
    /* On crée un nouvel élément */
    element* nouvelElement = (element*) malloc(sizeof(element));

    /* On assigne la valeur au nouvel élément */
    nouvelElement->val = valeur;

    /* On ajoute en fin, donc aucun élément ne va suivre */
    nouvelElement->nxt = NULL;

    if(liste == NULL)
    {
        /* Si la liste est videé il suffit de renvoyer l'élément créé */
        return nouvelElement;
    }
    else
    {
        /* Sinon, on parcourt la liste à l'aide d'un pointeur temporaire et on
        indique que le dernier élément de la liste est relié au nouvel élément */
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
        /* Si la liste est non vide, on se prépare à renvoyer l'adresse de
        l'élément en 2ème position */
        element* aRenvoyer = liste->nxt;
        /* On libère le premier élément */
        free(liste);
        /* On retourne le nouveau début de la liste */
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

    /* Si la liste contient un seul élément */
    if(liste->nxt == NULL)
    {
        /* On le libère et on retourne NULL (la liste est maintenant vide) */
        free(liste);
        return NULL;
    }

    /* Si la liste contient au moins deux éléments */
    element* tmp = liste;
    element* ptmp = liste;
    /* Tant qu'on n'est pas au dernier élément */
    while(tmp->nxt != NULL)
    {
        /* ptmp stock l'adresse de tmp */
        ptmp = tmp;
        /* On déplace tmp (mais ptmp garde l'ancienne valeur de tmp */
        tmp = tmp->nxt;
    }
    /* A la sortie de la boucle, tmp pointe sur le dernier élément, et ptmp sur
    l'avant-dernier. On indique que l'avant-dernier devient la fin de la liste
    et on supprime le dernier élément */
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
        /* On stocke l'élément suivant pour pouvoir ensuite avancer */
        tmpnxt = tmp->nxt;
        /* On efface l'élément courant */
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
            /* Si l'élément a la valeur recherchée, on renvoie son adresse */
            return tmp;
        }
        tmp = tmp->nxt;
    }
    return NULL;
}

