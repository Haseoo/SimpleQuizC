#include "headers/players.h"
#include "headers/questions.h"
#include "headers/quiz_menu.h"
#include "headers/quiz_game.h"

static int allegroInit(void) {
    if (!al_init()) {
        fprintf( stderr, "Failed to initialize Allegro5 library.\n" );
        return -1;
    }
    if (!al_init_image_addon()) {
        fprintf( stderr, "Failed to initialize Allegro5 image library.\n" );
        return -1;
    }
    al_init_font_addon();
    if (!al_init_ttf_addon()) {
        fprintf( stderr, "Failed to initialize Allegro5 font library.\n" );
        return -1;
    }
    if (!al_install_keyboard()) {
        fprintf( stderr, "Failed to initialize Allegro5 keyboard.\n" );
        return -1;
    }
    if (!al_install_mouse()) {
        fprintf( stderr, "Failed to initialize Allegro5 mouse.\n" );
        return -1;
    }
    return 0;
}


int main(void) {
    FILE *error = freopen("error.log","w",stderr);
    if (!error) {
        return EXIT_FAILURE;
    }
    if (allegroInit() == - 1) {
        return EXIT_FAILURE;
    }
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    event_queue = al_create_event_queue();
    ALLEGRO_EVENT ev;
    ALLEGRO_BITMAP *icon = al_load_bitmap("assets/icon.png");
    ALLEGRO_DISPLAY *gameWindow = al_create_display(720, 450);
    if (icon) {
        al_set_display_icon(gameWindow, icon);
    }
    ALLEGRO_FONT *font = al_load_ttf_font("assets/arial.ttf", 22, 0);
    ALLEGRO_FONT *questionFont = al_load_ttf_font("assets/arial.ttf", 20, 0);
    if (!font || !questionFont) {
        al_show_native_message_box(gameWindow, "Failed to load Simple Quiz" , "Font error","" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return EXIT_FAILURE;
    }
    al_register_event_source(event_queue, al_get_display_event_source(gameWindow));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_set_window_title(gameWindow ,"Simple Quiz");

	const size_t NUMBER_OF_CATEGORIES = countNumberOfCategories("assets/categories.txt");
    if (NUMBER_OF_CATEGORIES <= 0) {
        al_show_native_message_box(gameWindow, "Failed to load Simple Quiz" , "An error occurred while initializing questions", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return EXIT_FAILURE;
    }
    struct category *categoriesList = (struct category*)calloc(NUMBER_OF_CATEGORIES, sizeof(struct category));
    struct questionsList *questionList = NULL;
    if (initCategoriesList(categoriesList, "assets/categories.txt", NUMBER_OF_CATEGORIES) == -1) {
        al_show_native_message_box(gameWindow, "Failed to load Simple Quiz" , "An error occurred while initializing questions", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return EXIT_FAILURE;
    }
    const size_t NUMBER_OF_ALL_QUESTIONS = countMaxQuestion(categoriesList, NUMBER_OF_CATEGORIES);
    struct player *playersList = NULL;
    bool end = false;

    while (!end) {
        size_t gameModeChoice = 0;
        size_t numberOfPlayers = 0;
        setUsedQuestionToZero(categoriesList, NUMBER_OF_CATEGORIES);

        gameModeChoice = gameModeMenu(event_queue, ev, gameWindow);
        if (gameModeChoice == 0) {
            end = true;
        }
        if (gameModeChoice != 0 && gameModeChoice != 6) {
            numberOfPlayers = numberOfPlayersMenu(event_queue, ev, gameWindow);
        }
        if (numberOfPlayers == 0 && gameModeChoice != 6) {
            end = true;
        }
        if (numberOfPlayers != 0) {
            if (playersInfoMenu(numberOfPlayers, &playersList, font, event_queue, ev, gameWindow) == -1) {
                end = true;
            }
        }
        if (end == true)
            break;

        srand(time(0));

        switch (gameModeChoice) {
            case 1 : {
                if (ask10Questions(playersList, numberOfPlayers, &questionList,categoriesList, NUMBER_OF_CATEGORIES, questionFont, event_queue, ev,gameWindow, false) == -1) {
                    end = true;
                }
                break;
            }
            case 2 : {
                if (ask10Questions(playersList, numberOfPlayers, &questionList,categoriesList, NUMBER_OF_CATEGORIES, questionFont, event_queue, ev, gameWindow, true) == -1) {
                    end = true;
                }
                break;
            }
            case 3 : {
                if (ask10QuestionsWithChoicingCategory(playersList, numberOfPlayers, &questionList, categoriesList, NUMBER_OF_CATEGORIES, questionFont, event_queue, ev, gameWindow) == -1) {
                    end = true;
                }
                break;
            }
            case 4 : {
                if (askMaxQuestions(playersList, numberOfPlayers, &questionList, categoriesList, NUMBER_OF_CATEGORIES, NUMBER_OF_ALL_QUESTIONS, questionFont, event_queue, ev, gameWindow, false) == -1) {
                    end = true;
                }
                break;
            }
            case 5 : {
                if (askMaxQuestions(playersList, numberOfPlayers, &questionList, categoriesList, NUMBER_OF_CATEGORIES, NUMBER_OF_ALL_QUESTIONS, questionFont, event_queue, ev, gameWindow, true) == -1) {
                    end = true;
                }
                break;
            }
            case 6 : {
                if(printHiScore(font, event_queue, ev, gameWindow) == -1) {
                    end = true;
                }
            }
        }
        if (gameModeChoice == 5 && !end) {
            for (size_t i = 0; i < numberOfPlayers; i++) {
                if (addScoreToBoard(playersList->score, playersList->playerName, gameWindow) == -1) {
                    end = true;
                    break;
                }
                playersList = playersList->next;
            }
        }
        if (!end && gameModeChoice != 6) {
            if (printScore(playersList, font, numberOfPlayers, event_queue, ev, gameWindow) == -1) {
                end = true;
            }
        }
        removePlayersList(&playersList);
        deleteList(&questionList);
    }
    fclose(error);
    al_destroy_display(gameWindow);
    if (icon) {
        al_destroy_bitmap(icon);
    }
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(questionFont);
    removePlayersList(&playersList);
    deleteList(&questionList);
    free(categoriesList);
    return 0;
}
