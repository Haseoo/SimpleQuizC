#include <stdio.h>
#include "headers/quiz_game.h"
#include "headers/questions.h"


static char askQuestionAndGetAnswer(struct questionsList *front, struct category *categoriesList, ALLEGRO_FONT *font) {
    char correctAnswer = -1;
    size_t rep = 0;
    int questionY = 45, answerY = 240; //begin of text
    FILE *file = fopen(categoriesList[front->category].filePath,"r");
    if (file) {
        char textLine[75];
        memset(textLine, 0, sizeof(char) * 75);
        while ( !feof(file) || strncmp(textLine,"END_OF_LIST",11) != 0){
            fgets(textLine, 75, file);
            if (strncmp(textLine,"BEGIN_OF_QUESTION",17) == 0)
                rep++;
            if (rep == front->question + 1) {
                fgets(textLine,75, file);
                while ( strncmp(textLine,"END_OF_QUESTION",15) && !feof(file)) {
                    endStringProperly(textLine);
                    al_draw_text(font, al_map_rgb(0,0,0), 720 / 2 ,questionY ,ALLEGRO_ALIGN_CENTER, textLine);
                    questionY += 25; //new textline
                    fgets(textLine,75,file);
                }
                for(int i = 0; i < 4; i++) {
                    fgets(textLine,75,file);
                    endStringProperly(textLine);
                    al_draw_text(font, al_map_rgb(0,0,0), 720 / 2 ,answerY ,ALLEGRO_ALIGN_CENTER, textLine);
                    answerY += 45; //go to next box
                }
                if (strncmp(textLine,"END_OF_LIST",11) != 0)
                    correctAnswer=getc(file);
                else
                    fprintf(stderr, "Failed to read question no.%zu forom file %s\n", front->question + 1, categoriesList[front->category].filePath);
                fclose(file);
                return correctAnswer;
            }
        }
    }
    else {
        fprintf(stderr,"Couldn't open file with list of categories: %s \n",categoriesList[front->category].filePath);
        return -1;
    }
    return -1;
}


static size_t getThreeCategories(struct player *playersList, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    size_t choices[3] = {0, 0, 0};
    while (1) {
        choices[0] = rand() % NUMBER_OF_CATEGORIES;
        if (categoriesList[choices[0]].numberOfQuestions > categoriesList[choices[0]].numberOfUsedQuestions) {
            break;
        }
    }
    while (1) {
        choices[1] = rand() % NUMBER_OF_CATEGORIES;
        if (choices[1] != choices [0] && categoriesList[choices[1]].numberOfQuestions > categoriesList[choices[1]].numberOfUsedQuestions) {
            break;
        }
    }
    while (1) {
        choices[2] = rand() % NUMBER_OF_CATEGORIES;
        if (choices[2] != choices [0] && choices[2] != choices[1] && categoriesList[choices[2]].numberOfQuestions > categoriesList[choices[2]].numberOfUsedQuestions) {
            break;
        }
    }
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/cat_choice.bmp");
    if (!bmp) {
        fprintf(stderr, "Missing file assets/cat_choice.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    int begY = 140;
    int answer = 0;
    bool gotAnswer = false;
    al_draw_bitmap(bmp, 0, 0, 0);
    al_draw_textf(font, al_map_rgb(0,0,0), 720 / 2, begY, ALLEGRO_ALIGN_CENTER, "The category chooser: %s",playersList->playerName);
    begY += 49;
    for (size_t i = 0; i < 3; i++) {
        al_draw_textf(font, al_map_rgb(0,0,0), 720 / 2, begY, ALLEGRO_ALIGN_CENTER, "%zu - %s", i + 1, categoriesList[choices[i]].categoryName);
        begY += 49;
    }
    al_flip_display();
    do {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_bitmap(bmp);
            return -2;
        }
        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
            if (ev.mouse.x >= 163 && ev.mouse.x <= 560 && ev.mouse.y >= 185 && ev.mouse.y <= 320)  {
                int begY2 = 185;
                for (int i = 0; i < 3; i++) {
                    if (ev.mouse.y >= begY2 && ev.mouse.y <= begY2 + 49) {
                        answer = i;
                        gotAnswer = true;
                    }
                    begY2 += 49;
                }
            }
        }
    }
    while (!gotAnswer);
    categoriesList[choices[answer]].numberOfUsedQuestions += 1;
    al_destroy_bitmap(bmp);
    return choices[answer];
}


int ask10Questions (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES,
                     ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display, bool fallingOut) {
    char correctAnswer = 0, answer = 0;
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/ask_question.bmp");
	int numberOfPlayingPlayers = numberOfPlayers;
    if (!bmp) {
        fprintf(stderr, "Missing file assets/ask_question.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    bool gotAnswer = false;
    for (int i = 0; i < 10; i++) {
        getRandomQuestion(front,categoriesList, NUMBER_OF_CATEGORIES);
        for (int j = 0; j < numberOfPlayers; j++) {
			if (playersList->isPlaying) {
                al_draw_bitmap(bmp,0,0,0);
                al_draw_textf(font, al_map_rgb(0,0,0), 1, 8, ALLEGRO_ALIGN_LEFT, "Question %d. from category %s.", i + 1, categoriesList[(*front)->category].categoryName);
                al_draw_textf(font, al_map_rgb(0,0,0), 690, 8, ALLEGRO_ALIGN_RIGHT,"Now answering: %s", playersList->playerName);
                correctAnswer = askQuestionAndGetAnswer(*front, categoriesList, font);
                al_flip_display();
                if (correctAnswer == -1) {
                    al_show_native_message_box(display, "An error occurred" , "", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    al_destroy_bitmap(bmp);
                    return -1;
                }
                //getting answer
                do {
                    al_wait_for_event(event_queue, &ev);
                    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        al_destroy_bitmap(bmp);
                        return -1;
                    }
                    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
                        if (ev.mouse.x >= 35 && ev.mouse.x <= 680 && ev.mouse.y >= 230 && ev.mouse.y <= 400) {
                            int mouseY = 230; //first answer bar's y
                            for (int i = 0; i < 4; i++) {
                                if (ev.mouse.y >= mouseY && ev.mouse.y <= mouseY + 35) {
                                    answer = 'A' + i;
                                    gotAnswer = true;
                                }
                                mouseY += 45; //adding answer bar size
                            }
                        }

                    }
                }
            while (!gotAnswer);
            gotAnswer = false;
            if (answer == correctAnswer) {
                al_draw_text(font, al_map_rgb(25,140,40), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is correct, press any key to continiue...");
                al_flip_display();
                playersList->score++;
            }
            else {
				if (fallingOut) {
                    al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is incorrect, you lost, press any key to continiue...");
                    playersList->isPlaying = false;
                    numberOfPlayingPlayers--;
                    al_flip_display();
				}
				else {
					al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is incorrect, press any key to continiue...");
					al_flip_display();
				}
            }
            do {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    al_destroy_bitmap(bmp);
                    return -1;
                }
            }
            while (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
            answer = 0;
			}
            playersList = playersList->next;
			if (numberOfPlayingPlayers == 0) {
                al_destroy_bitmap(bmp);
                return 0;
            }
        }
    }
    al_destroy_bitmap(bmp);
    return 0;
}


int ask10QuestionsWithChoicingCategory (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES,
                                         ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    char correctAnswer = 0, answer = 0;
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/ask_question.bmp");
    struct player *choicingList = playersList;
    if (!bmp) {
        fprintf(stderr, "Missing file assets/ask_question.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    bool gotAnswer = false;
    for (int i = 0; i < 10; i++) {
        size_t categoryNum = getThreeCategories(choicingList, categoriesList, NUMBER_OF_CATEGORIES, font, event_queue, ev, display);
        choicingList = choicingList->next;
        if ((int) categoryNum == -1) {
            al_show_native_message_box(display, "An error occurred" , "", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
            al_destroy_bitmap(bmp);
            return -1;
        }
        if ((int) categoryNum == -2) {
            al_destroy_bitmap(bmp);
            return -1;
        }
        size_t questionNum = getQuestionNumber(categoriesList, *front, categoryNum);
        addToList(front, categoryNum, questionNum);
        for (int j = 0; j < numberOfPlayers; j++) {
            al_draw_bitmap(bmp,0,0,0);
            al_draw_textf(font, al_map_rgb(0,0,0), 1, 8, ALLEGRO_ALIGN_LEFT, "Pytanie %d. z kategorii %s.", i + 1, categoriesList[(*front)->category].categoryName);
            al_draw_textf(font, al_map_rgb(0,0,0), 690, 8, ALLEGRO_ALIGN_RIGHT,"Teraz odpowiada: %s", playersList->playerName);
            correctAnswer = askQuestionAndGetAnswer(*front, categoriesList, font);
            al_flip_display();
            if (correctAnswer == -1) {
                al_show_native_message_box(display, "An error occurred" , "", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
                al_destroy_bitmap(bmp);
                return -1;
            }
            //getting answer
            do {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    al_destroy_bitmap(bmp);
                    return -1;
                }
                if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
                    if (ev.mouse.x >= 35 && ev.mouse.x <= 680 && ev.mouse.y >= 230 && ev.mouse.y <= 400) {
                        int mouseY = 230; //first answer bar's y
                        for (int i = 0; i < 4; i++) {
                            if (ev.mouse.y >= mouseY && ev.mouse.y <= mouseY + 35) {
                                answer = 'A' + i;
                                gotAnswer = true;
                            }
                            mouseY += 45; //adding answer bar size
                        }
                    }

                }
            }
            while (!gotAnswer);
            gotAnswer = false;
            if (answer == correctAnswer) {
                al_draw_text(font, al_map_rgb(25,140,40), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is correct, press any key to continiue...");
                al_flip_display();
                playersList->score++;
            }
            else {
                al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is incorrect, press any key to continiue...");
                al_flip_display();
            }
            do {
                al_wait_for_event(event_queue, &ev);
                if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    al_destroy_bitmap(bmp);
                    return -1;
                }
            }
            while (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
            answer = 0;
            playersList = playersList->next;
        }
    }
    al_destroy_bitmap(bmp);
    return 0;
}

int askMaxQuestions (struct player *playersList, int numberOfPlayers, struct questionsList **front, struct category *categoriesList, const size_t NUMBER_OF_CATEGORIES, const size_t NUMBER_OF_ALL_QUESTIONS,
                      ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev,ALLEGRO_DISPLAY *display, bool fallingOut) {
    char correctAnswer = 0, answer = 0;
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/ask_question.bmp");
    ALLEGRO_BITMAP *resignButton = al_load_bitmap("assets/resign_button.bmp");
    int numberOfPlayingPlayers = numberOfPlayers;
    if (!bmp || !resignButton) {
        fprintf(stderr,"Missing file assets/ask_question.bmp or assets/resign_button.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    bool gotAnswer = false;
    for (size_t i = 0; i < NUMBER_OF_ALL_QUESTIONS; i++) {
        getRandomQuestion(front, categoriesList, NUMBER_OF_CATEGORIES);
        for (int j = 0; j < numberOfPlayers; j++) {
            if (playersList->isPlaying) {
                al_draw_bitmap(bmp,0,0,0);
                al_draw_bitmap(resignButton,690,0,0);
                al_draw_textf(font, al_map_rgb(0,0,0), 1, 8, ALLEGRO_ALIGN_LEFT, "Question %zu. from category %s.", i + 1, categoriesList[(*front)->category].categoryName);
                al_draw_textf(font, al_map_rgb(0,0,0), 690, 8, ALLEGRO_ALIGN_RIGHT,"Now answering: %s", playersList->playerName);
                correctAnswer = askQuestionAndGetAnswer(*front, categoriesList, font);
                al_flip_display();
                if (correctAnswer == -1) {
                    al_destroy_bitmap(bmp);
                    al_destroy_bitmap(resignButton);
                    al_show_native_message_box(display, "An error occurred" , "", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
                    return -1;
                }
                //getting answer
                do {
                    al_wait_for_event(event_queue, &ev);
                    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                        return -1;
                    if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1) {
                        if (ev.mouse.x >= 35 && ev.mouse.x <= 680 && ev.mouse.y >= 230 && ev.mouse.y <= 400) {
                            int mouseY = 230; //first answer bar's y
                            for (int i = 0; i < 4; i++) {
                                if (ev.mouse.y >= mouseY && ev.mouse.y <= mouseY + 35) {
                                    answer = 'A' + i;
                                    gotAnswer = true;
                                }
                                mouseY += 45; //adding answer bar size
                            }
                        }
                        if (ev.mouse.x >= 690 && ev.mouse.x <= 720 && ev.mouse.y >= 0 && ev.mouse.y <= 30) {
                            answer = -2;
                            gotAnswer = true;
                        }
                    }
                }
                while (!gotAnswer);
                gotAnswer = false;
                if (answer == correctAnswer) {
                    al_draw_text(font, al_map_rgb(25,140,40), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is correct, press any key to continiue...");
                    al_flip_display();
                    playersList->score++;
                }
                if (answer != correctAnswer && answer != -2) {
					if (fallingOut) {
                    al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Twoja odpowiedź jest niepoprawna, odpadasz! Kliknij aby kontynuować...");
                    playersList->isPlaying = false;
                    numberOfPlayingPlayers--;
                    al_flip_display();
					}
					else {
						al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "Your answer is incorrect, press any key to continiue...");
						al_flip_display();
					}
                }
                if (answer == -2) {
                    al_draw_text(font, al_map_rgb(255,0,0), 720 / 2 ,412 ,ALLEGRO_ALIGN_CENTER, "You've given up, press any key to continiue...");
                    al_flip_display();
                    playersList->isPlaying = false;
                    numberOfPlayingPlayers--;

                }
                do {
                    al_wait_for_event(event_queue, &ev);
                    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        al_destroy_bitmap(bmp);
                        al_destroy_bitmap(resignButton);
                        return -1;
                    }
                }
                while (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
                answer = 0;
            }
            playersList = playersList->next;
            if (numberOfPlayingPlayers == 0) {
                al_destroy_bitmap(bmp);
                al_destroy_bitmap(resignButton);
                return 0;
            }
        }
    }
    al_destroy_bitmap(bmp);
    al_destroy_bitmap(resignButton);
    return 0;
}
