#ifndef QUIZ_MENU_H_INCLUDED
#define QUIZ_MENU_H_INCLUDED

#include "players.h"
#include "questions.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>


size_t gameModeMenu( ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);
size_t numberOfPlayersMenu( ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);
int playersInfoMenu(size_t numberOfPlayers, struct player **playersList, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);
int printScore(struct player *playersList, ALLEGRO_FONT *font, size_t numberOfPlayers, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);

int addScoreToBoard(size_t score,char playerName[BUFFER_SIZE], ALLEGRO_DISPLAY *display);
int printHiScore(ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);


#endif // QUIZ_MENU_H_INCLUDED
