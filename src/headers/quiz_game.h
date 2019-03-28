#ifndef QUIZ_GAME_H_INCLUDED
#define QUIZ_GAME_H_INCLUDED

#include "players.h"
#include "questions.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>


int ask10Questions (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES,
                     ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev,ALLEGRO_DISPLAY *display, bool fallingOut);
int ask10QuestionsWithChoicingCategory (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES,
                                         ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display);
int askMaxQuestions (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES, const size_t NUMBER_OF_ALL_QUESTIONS,
                      ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display, bool fallingOut);

#endif // QUIZ_GAME_H_INCLUDED
