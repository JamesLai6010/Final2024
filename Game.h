#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <allegro5/allegro.h>
#include "UI.h"

class Game
{
public:
    void execute();
public:
    Game();
    ~Game();
    void game_init();
    bool game_update();
    void game_draw();
private:
    enum class STATE {
        MAIN_MENU, // -> START
        START,     // -> LEVEL
        LEVEL,     // -> PAUSE, END
        PAUSE,     // -> LEVEL
        END
    };
    STATE state;

    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *game_icon;
    ALLEGRO_BITMAP *main_page; // 主頁背景
	ALLEGRO_BITMAP *background1;
	ALLEGRO_BITMAP *background2;

    struct Button { // 按鈕結構體
        int x1, y1, x2, y2;   // 按鈕範圍
        const char *label;    // 按鈕文字
    };
    Button start_button; // "Start Game" 按鈕
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    UI *ui;
};

#endif
