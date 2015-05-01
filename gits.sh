#!/bin/sh
ROOTDIR=`pwd`
OrgName="robotronik"

echo "Récupération de la liste des dépôts en cours…"
RepoList="$(curl -i https://api.github.com/orgs/$OrgName/repos)"

RepoRootUrl="https://github.com/$OrgName/"
RepoNames=$(echo "$RepoList" | grep \"name\": | awk -F\" '{print $4}')

cloneall() {
    for repo in $RepoNames ; do
        echo "Dépôt $repo"
        if [ -d "$repo" ]; then
            echo "Le dépôt existe déjà"
        else
            git clone $RepoRootUrl$repo "$@"
        fi
    done
}
pullall() {
    for repo in $RepoNames ; do
        echo "Dépôt $repo"
        if [ -d "$repo" ]; then
            cd $repo
            git pull "$@"
            cd ..
        else
            echo "Le dépôt n'existe pas encore ! Veuillez le cloner d'abord."
        fi
    done
}
statusall() {
    for repo in $RepoNames ; do
        echo "Dépôt $repo"
        if [ -d "$repo" ]; then
            cd $repo
            git status "$@"
            cd ..
        else
            echo "Le dépôt n'existe pas encore ! Veuillez le cloner d'abord."
        fi
    done
}

if [[ -z "$1" ]]; then
    echo "Pas d'arguments donnés !"
else
    case "$1" in
        clone|--clone|-c)
            shift
            cloneall
            ;;
        pull|--pull|-p)
            shift
            pullall
            ;;
        status|--status|-s)
            shift
            statusall
            ;;
        *)
            echo "Argument \"$1\" inconnu !"
            help
            ;;
    esac

fi

