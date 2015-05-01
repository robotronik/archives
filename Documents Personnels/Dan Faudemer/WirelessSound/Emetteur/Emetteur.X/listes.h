/* 
 * File:   listes.h
 * Author: Dan
 *
 * Created on 15 octobre 2013, 17:12
 */

#ifndef LISTES_H
#define	LISTES_H

#define LIST_SIZE 255
struct list
{
    int start;
    int end;
    int val[LIST_SIZE];

};
typedef struct list list;

#endif	/* LISTES_H */

