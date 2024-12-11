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

    //角色選擇
    void apply_character_selection();
    void set_player_roles();

private:
    enum class STATE {
        MAIN_MENU, // -> START
        START,     // -> LEVEL
        LEVEL,     // -> PAUSE, END
        PAUSE,     // -> LEVEL
        END,
        CHARACTER_SELECTION,  // 角色選擇
        SCENE_SELECTION,  // 場景選擇
        Fight_FINISH
    };
    STATE state;

    ALLEGRO_EVENT event;
    ALLEGRO_BITMAP *game_icon;
    ALLEGRO_BITMAP *main_page; // 主頁背景
    ALLEGRO_BITMAP *sceneSelection;   //場景選擇
    ALLEGRO_BITMAP *characterSelection;   //角色選擇
	ALLEGRO_BITMAP *background1;
	ALLEGRO_BITMAP *background2;
    ALLEGRO_BITMAP *background3;
    ALLEGRO_BITMAP *character1;
	ALLEGRO_BITMAP *character2;
    ALLEGRO_BITMAP *character3;
    ALLEGRO_BITMAP *character4;

    ALLEGRO_BITMAP *win_map1;
    ALLEGRO_BITMAP *win_map2;
    ALLEGRO_BITMAP *win_map3;
    ALLEGRO_BITMAP *win_map4;
    ALLEGRO_BITMAP *win_map5;

    ALLEGRO_BITMAP *playbtn;
    struct Button { // 按鈕結構體
        double x1, y1, x2, y2;   // 按鈕範圍
        const char *label;    // 按鈕文字
    };

    struct SceneThumbnail {     // 場景選擇畫面縮圖
        ALLEGRO_BITMAP *thumbnail;  // 圖片
        double x, y;                   // 圖片的顯示位置
        double width, height;          // 圖片的寬度和高度
        bool is_hovered;            // 判斷是否被滑鼠懸停
        int number;
    };
    int scene_number = 0;
    std::vector<SceneThumbnail> scene_thumbnails; //場景存起來

    struct SelectCharacter {     // 選角色
        ALLEGRO_BITMAP *character;  // 圖片
        double x, y;                   // 圖片的顯示位置
        double width, height;          // 圖片的寬度和高度
        bool is_hovered;            // 判斷是否被滑鼠懸停
        int number;
        int selected = 0;
    };
    // 選角色
    std::vector<SelectCharacter> select_character;

    SelectCharacter player1_character, player2_character;
    int player_turn = 1;   

    std::string player1_name = ""; // 玩家一名稱
    std::string player2_name = ""; // 玩家二名稱
    bool name_input_active = false; // 是否正在輸入名稱
    int name_input_player = 0; // 當前輸入名稱的玩家 (1 或 2)
    bool player1_name_done = false; // 玩家一名稱是否完成輸入
    bool player2_name_done = false; // 玩家一名稱是否完成輸入
    int typeChar;
    Button start_button; // "Start Game" 按鈕
    Button map_button1; // 場景1 按鈕
    Button map_button2; // 場景2 按鈕
    Button map_button3; // 場景3 按鈕
    Button map_button4; // 場景4 按鈕

    bool no_winner = false;
    bool player1_win = false;

    Button play_button; // 選完角色的按鈕

    bool sceneSelectionBGM = false;
private:
    ALLEGRO_DISPLAY *display;
    ALLEGRO_TIMER *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
    
    UI *ui;
};

#endif
