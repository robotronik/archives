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
    char val[LIST_SIZE];

};
typedef struct list list;

char add_list(list *myList, char val);
char pop_list(list *myList, char *val);
void init_list(list *myList);
char empty_list(list *myList);
#endif	/* LISTES_H */

