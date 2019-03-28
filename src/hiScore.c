#include <stdio.h>
#include "headers/players.h"
#include <allegro5/allegro.h>
#include "headers/quiz_menu.h"
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>


static int findSpot(size_t score, char scoreTable[20][BUFFER_SIZE]) {
    int spot = -1;
    for (int i = 1; i < 20; i += 2) {
        if ((int)score > atoi(scoreTable[i])) {
            return spot = i - 1;
        }
    }
    return spot;
}


static void moveArray(char scoreTable[20][BUFFER_SIZE], int spot) {
    for(int i = 19; i >= spot; i--) {
        memcpy(scoreTable[i], scoreTable[i - 2], sizeof(char) * BUFFER_SIZE);
    }
}


int addScoreToBoard(size_t score, char playerName[BUFFER_SIZE], ALLEGRO_DISPLAY *display) {
    char scoreTable[20][BUFFER_SIZE], scoreChar[BUFFER_SIZE];
    memset(scoreTable, 0, sizeof(scoreTable));
    FILE *hiScoreFile = fopen("assets/hiStoreTable", "rw+");
    if (hiScoreFile) {
        fread(scoreTable, sizeof(char), sizeof(scoreTable) / sizeof(char), hiScoreFile);
        int spot = findSpot(score, scoreTable);
        if (spot == -1) {
            return 0;
        }
        moveArray(scoreTable, spot);
        snprintf(scoreChar, BUFFER_SIZE, "%zu", score);
        memcpy(scoreTable[spot],playerName, sizeof(char) * BUFFER_SIZE);
        memcpy(scoreTable[spot + 1],scoreChar, sizeof(char) * BUFFER_SIZE);
        fseek(hiScoreFile, 0, SEEK_SET);
        fwrite(scoreTable, sizeof(char), sizeof(scoreTable) / sizeof(char), hiScoreFile);
        fclose(hiScoreFile);
    }
    else {
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        fprintf(stderr,"Couldn't open file with score table: \"assets/hiStoreTable\".");
        return -1;
    }
    return 0;

}


int printHiScore(ALLEGRO_FONT *font, ALLEGRO_EVENT_QUEUE *event_queue, ALLEGRO_EVENT ev, ALLEGRO_DISPLAY *display) {
    al_flip_display();
    ALLEGRO_BITMAP *bmp = al_load_bitmap("assets/hiScore.bmp");
    if (!bmp) {
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        fprintf(stderr,"Couldn't open bitmap: \"assets/hiScore.bmp\".");
        return -1;
    }
    al_draw_bitmap(bmp, 0, 0, 0);
    int begTextY = 60, textX1 = 105, textX2 = 365, textX3 = 628;
    FILE *hiScoreFile = fopen("assets/hiStoreTable", "rb");
    char scoreTable[20][BUFFER_SIZE];
    if (hiScoreFile) {
        fread(scoreTable, sizeof(char), sizeof(scoreTable) / sizeof(char), hiScoreFile);
        al_draw_textf(font, al_map_rgb(0,0,0), 720 / 2, begTextY - 40, ALLEGRO_ALIGN_CENTER,"The best results for the all questions with falling off mode:");
        al_draw_textf(font, al_map_rgb(0,0,0), textX1, begTextY, ALLEGRO_ALIGN_CENTER,"No.");
        al_draw_textf(font, al_map_rgb(0,0,0), textX2, begTextY, ALLEGRO_ALIGN_RIGHT,"Player:");
        al_draw_textf(font, al_map_rgb(0,0,0), textX3, begTextY, ALLEGRO_ALIGN_RIGHT,"Score:");
        begTextY += 33;
        for (size_t i = 0; i < 10; i++) {
            al_draw_textf(font, al_map_rgb(0,0,0), textX1, begTextY, ALLEGRO_ALIGN_CENTER,"%zu",i + 1);
            al_draw_textf(font, al_map_rgb(0,0,0), textX2, begTextY, ALLEGRO_ALIGN_RIGHT,"%s", scoreTable[i * 2]);
            al_draw_textf(font, al_map_rgb(0,0,0), textX3, begTextY, ALLEGRO_ALIGN_RIGHT,"%d", atoi(scoreTable[i * 2 + 1]));
            begTextY += 33;
        }
        fclose(hiScoreFile);
    }
    else {
        al_show_native_message_box(display, "An error occurred" , "Missing file", "Check errors.log for more details" ,NULL, ALLEGRO_MESSAGEBOX_ERROR);
        fprintf(stderr,"Couldn't open file with score table: \"assets/hiStoreTable\".");
        return -1;
    }
    al_flip_display();
    do {
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            al_destroy_bitmap(bmp);
            return -1;
        }
        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && ev.mouse.x >= 670 && ev.mouse.y <= 50) {
            hiScoreFile = fopen("assets/hiStoreTable", "wb");
            memset(scoreTable, 0, sizeof(char) * 20 * BUFFER_SIZE);
            fwrite(scoreTable, sizeof(char), sizeof(scoreTable) / sizeof(char), hiScoreFile);
            fclose(hiScoreFile);
            printHiScore(font, event_queue, ev, display);
        }
    }
    while (ev.type != ALLEGRO_EVENT_MOUSE_BUTTON_DOWN);
    al_destroy_bitmap(bmp);
    return 0;
}
