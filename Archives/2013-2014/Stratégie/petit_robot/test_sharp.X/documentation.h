#ifndef DOCUMENTATION_H
#define DOCUMENTATION_H


/*
 *    Little robot
 *    Copyright (C) 2014 Robotronik
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


/** \mainpage Programation du petit robot pour la coupe 2014
 *
 * \section intro_sec Introduction
 *
 * Cette documentation concerne la partie intelligente du petit robot pour la coupe 2014.
 *
 * \section running_sec Éxécuter le programme
 *
 * Outils necessaires :
 * - mplabx
 * - xc16
 *
 * Des fichiers de projet fournis. Vous pouvez les ouvrirs avec mplab.
 *
 * \section organisation_sec Organisation du code
 *
 * Le code est organisé de la manière suivante :
 *  - main.h        Contient la liste de tout ce qui doit être inclus dans tout les fichiers.
 *  - ailleurs.h    Contient des binding vers les fonctions écrite les années d’avant. Son contenu devrait être déplacé à une place approprié.
 *  - coordonees.h
 *  - strategie.h   Contient les fonctions qui gère la startégie du robot. La statégie est défini par un tableau d’objectif.
 *  - mouvement.h   Tout ce qui permet de controler les mouvements du robot (le type de mouvement à faire)
 *
 * Les autres fichiers contient les fonctions provenant des années d’avant et qui sont documentés :
 *
 *  - debug.h
 *
 * Les autres fichiers contient les fonctions provenant des années d’avant et qui sont non documentées hélas.
 *
 *  - bitsConfiguration.h
 *  - comAsser.h
 *  - comXbee.h
 *  - init.h
 *  - sharp.h
 *  - uart.h
 *  - uart_xbee.h
 *
 *
 * \section copyright_sec Copyright and License
 *
 * Ce logiciel est distribué sous la licence GNU GPL v3.

 \verbatim

 Little robot
 Copyright (C) 2014 Robotronik

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

\endverbatim


*
*/

#endif /* DOCUMENTATION_H */
