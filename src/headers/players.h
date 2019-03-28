#ifndef PLAYERS_H_INCLUDED
#define PLAYERS_H_INCLUDED


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFER_SIZE 11 

struct player {
    char playerName[BUFFER_SIZE]; 
    size_t score; 
    bool isPlaying; 
    struct player *prev; 
    struct player *next; 
};


int addPlayer (struct player**, const char *playerName);
void removePlayersList(struct player **playersList);


#endif // PLAYERS_H_INCLUDED
