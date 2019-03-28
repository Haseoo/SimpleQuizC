#include "headers/quiz_menu.h"

static size_t getGameModeChoice(int mouseX, int mouseY) {
    if (mouseX >= 90 && mouseX <= 635) {
        if (mouseY >= 135 && mouseY <= 180)
            return 1;
        if (mouseY >= 203 && mouseY <= 245)
            return 2;
        if (mouseY >= 265 && mouseY <= 305)
            return 3;
        if (mouseY >= 325 && mouseY <= 367)
            return 4;
        if (mouseY >= 390 && mouseY <= 435)
            return 5;
    }
    else if (mouseX >= 640 && mouseY <= 80)
        return 6;
    return 0;
}


size_t gameModeMenu( ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/menu.bmp");
    size_t gameModeChoice = 0;
     if (!bmp) {
        fprintf(stderr,"Missing file assets/menu.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
     }
    al_draw_bitmap(bmp,0,0,0);
    al_flip_display();
        while (gameModeChoice == 0) {
            al_wait_for_event(event_queue, &ev);
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                al_destroy_bitmap(bmp);
                return 0;
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1)
                gameModeChoice = getGameModeChoice(ev.mouse.x, ev.mouse.y);
        }
    al_destroy_bitmap(bmp);
    return gameModeChoice;
}


static size_t getNumberOfPlayers (int mouseX, int mouseY) {
    int begX = 170, barHeight = 35, offset = 50;
    if (mouseX >= 167 && mouseX <= 560) {
        for(size_t i = 0; i < 4; i++) {
            if (mouseY >= begX  && mouseY <= begX + barHeight)
                return i + 1;
            begX += offset;
        }
    }
    return 0;
}


size_t numberOfPlayersMenu(ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/menu2.bmp");
    size_t numberOfPlayers = 0;
    if (!bmp) {
        fprintf(stderr,"Missing file assets/menu2.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return 0;
    }
    al_draw_bitmap(bmp,0,0,0);
    al_flip_display();
    while (numberOfPlayers == 0) {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_bitmap(bmp);
            return 0;
        }
        if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.button == 1)
            numberOfPlayers = getNumberOfPlayers(ev.mouse.x, ev.mouse.y);
        }
    al_destroy_bitmap(bmp);
    return numberOfPlayers;
}


static int getNick(size_t numberOfPlayers, struct player **playersList, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    char nick[BUFFER_SIZE];
    ALLEGRO_BITMAP *whiteBar = al_load_bitmap("assets/whitebar.bmp");
    if (!whiteBar) {
        fprintf(stderr, "Missing file assets/white_bar.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    al_draw_textf(font, al_map_rgb(0,0,0), 110, 190, ALLEGRO_ALIGN_CENTRE, "Player: %zu",numberOfPlayers);
    al_flip_display();
    for (int i = 0; i < BUFFER_SIZE - 1; i++) {
        al_wait_for_event(event_queue, &ev);
        while (ev.type != ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                al_destroy_bitmap(whiteBar);
                return -2;
            }
            al_wait_for_event(event_queue, &ev);
        }
        int tmp = ev.keyboard.keycode;
        if ((tmp < 0 || tmp > 36) && tmp!= 67 && tmp!=63) {
            i--;
            continue;
        }
        if (tmp == 63) {
            if (i > 0) {
                nick[i - 1] = 0;
                i -= 2;
                al_draw_bitmap(whiteBar,179,190,0);
                al_draw_text(font, al_map_rgb(0,0,0), 180, 190,ALLEGRO_ALIGN_LEFT, nick);
                al_flip_display();
                continue;
            }
            else {
                i--;
                continue;
            }
        }
        char oneChar = 0;
        if ((tmp >= 27 && tmp <= 36))
            oneChar = (char)tmp - 27 + '0';
        else
            oneChar = (char)tmp + 'a' - 1;
        if (tmp == 67)
            break;
        nick[i] = oneChar;
        nick[i + 1] = 0;
        al_draw_bitmap(whiteBar,180,188,0);
        al_draw_text(font, al_map_rgb(0,0,0), 180, 190,ALLEGRO_ALIGN_LEFT, nick);
        al_flip_display();
    }
    al_destroy_bitmap(whiteBar);
    return addPlayer(playersList,nick);

}


int playersInfoMenu(size_t numberOfPlayers, struct player **playersList, ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    for (size_t i = 0; i < numberOfPlayers; i++) {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            return -1;
        }
        ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/menu3.bmp");
        if (!bmp) {
            fprintf(stderr,"Missing file assets/menu3.bmp\n");
            al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
            return -1;
        }
        al_draw_bitmap(bmp,0,0,0);
        al_draw_textf(font, al_map_rgb(0,0,0), 720 / 2, 130, ALLEGRO_ALIGN_CENTER, "Enter your nickname (max %d chars, only small letters and digits)", BUFFER_SIZE - 1);
        int result = getNick(i + 1, playersList, font, event_queue, ev, display);
        if (result == -1) {
            al_destroy_bitmap(bmp);
			al_show_native_message_box(display, "An error occurredd" , "", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
            return -1;
        }
        if (result == -2) {
            al_destroy_bitmap(bmp);
            return -1;
        }
        al_destroy_bitmap(bmp);
    }
    return 0;
}

int printScore(struct player *playersList, ALLEGRO_FONT *font, size_t numberOfPlayers, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/scoreboard.bmp");
    if (!bmp) {
        fprintf(stderr, "Missing file assets/scoreboard.bmp\n");
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    al_draw_bitmap(bmp,0,0,0);
    int textX1 = 355, textX2 = 625, textY = 170;
    al_draw_text(font, al_map_rgb(0,0,0), textX1, textY, ALLEGRO_ALIGN_RIGHT,"Gracz:");
    al_draw_text(font, al_map_rgb(0,0,0), textX2, textY, ALLEGRO_ALIGN_RIGHT, "wynik:");
    textY += 33; //go to next row
    for (size_t i = 0; i < numberOfPlayers; i++) {
        al_draw_text(font, al_map_rgb(0,0,0), textX1, textY, ALLEGRO_ALIGN_RIGHT, playersList->playerName);
        al_draw_textf(font, al_map_rgb(0,0,0), textX2, textY, ALLEGRO_ALIGN_RIGHT, "%zu",playersList->score);
        textY += 33; //go to next row
        playersList = playersList->next;
    }
    al_flip_display();
    do {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_bitmap(bmp);
            return -1;
        }
    }
    while (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
    al_destroy_bitmap(bmp);
    return 0;
}
