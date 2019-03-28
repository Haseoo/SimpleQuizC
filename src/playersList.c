#include "headers/players.h"


int addPlayer (struct player **playersList, const char *playerName) {
    struct player *newElement = (struct player*)malloc(sizeof(struct player));
    if (newElement) {
        strncpy(newElement->playerName, playerName, BUFFER_SIZE);
        newElement->score = 0;
        newElement->isPlaying = true;
        if (*playersList == NULL) {
            newElement->next = newElement->prev = newElement;
            *playersList = newElement;
        }
        else {
            newElement->next = *playersList;
            newElement->prev = (*playersList)->prev;
            (*playersList)->prev->next = newElement;
            (*playersList)->prev = newElement;
        }
    }
    else {
        fprintf(stderr,"Could't add player to list- couldn't create new element\n");
        return -1;
    }
    return 0;
}

void removePlayersList(struct player **playersList) {
    if (*playersList) {
        struct player *start = *playersList;
        do {
            struct player *tmp = (*playersList)->next;
            free(*playersList);
            *playersList = tmp;
        }
        while (*playersList != start);
        *playersList = NULL;
    }
}
