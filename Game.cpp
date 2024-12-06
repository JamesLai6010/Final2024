#include "Game.h"
#include "Utils.h"
#include "data/DataCenter.h"
#include "data/OperationCenter.h"
#include "data/SoundCenter.h"
#include "data/ImageCenter.h"
#include "data/FontCenter.h"
#include "Player.h"
#include "Level.h"
#include "Hero.h"  //快龍實作

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <cstring>
//角色1
#include "Character/Character1.h"
#include "Props/props.h"
#include "Props/Prop_GOD.h"

//git testing1
// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.jpg";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";
constexpr char background1_img_path[] = "./assets/image/background1.png";
constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg";
constexpr char mainpage_img_path[] = "./assets/image/mainpage.png";
/**
 * @brief Game entry.
 * @details The function processes all allegro events and update the event state to a generic data storage (i.e. DataCenter).
 * For timer event, the game_update and game_draw function will be called if and only if the current is timer.
 */
void
Game::execute() {
	DataCenter *DC = DataCenter::get_instance();
	// main game loop
	bool run = true;
	while(run) {
		// process all events here
		//printf("%lf %lf\n", DC->mouse.x, DC->mouse.y);
		al_wait_for_event(event_queue, &event);
		switch(event.type) {
			case ALLEGRO_EVENT_TIMER: {
				run &= game_update();
				game_draw();
				break;
			} case ALLEGRO_EVENT_DISPLAY_CLOSE: { // stop game
				run = false;
				break;
			} case ALLEGRO_EVENT_KEY_DOWN: {
				DC->key_state[event.keyboard.keycode] = true;
				break;
			} case ALLEGRO_EVENT_KEY_UP: {
				DC->key_state[event.keyboard.keycode] = false;
				break;
			} case ALLEGRO_EVENT_MOUSE_AXES: {
				DC->mouse.x = event.mouse.x;
				DC->mouse.y = event.mouse.y;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN: {
				DC->mouse_state[event.mouse.button] = true;
				break;
			} case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
				DC->mouse_state[event.mouse.button] = false;
				break;
			} default: break;
		}
	}
}

/**
 * @brief Initialize all allegro addons and the game body.
 * @details Only one timer is created since a game and all its data should be processed synchronously.
 */
Game::Game() {
	DataCenter *DC = DataCenter::get_instance();
	GAME_ASSERT(al_init(), "failed to initialize allegro.");

	// initialize allegro addons
	bool addon_init = true;
	addon_init &= al_init_primitives_addon();
	addon_init &= al_init_font_addon();
	addon_init &= al_init_ttf_addon();
	addon_init &= al_init_image_addon();
	addon_init &= al_init_acodec_addon();
	GAME_ASSERT(addon_init, "failed to initialize allegro addons.");

	// initialize events
	bool event_init = true;
	event_init &= al_install_keyboard();
	event_init &= al_install_mouse();
	event_init &= al_install_audio();
	GAME_ASSERT(event_init, "failed to initialize allegro events.");

	// initialize game body
	GAME_ASSERT(
		display = al_create_display(DC->window_width, DC->window_height),
		"failed to create display.");
	GAME_ASSERT(
		timer = al_create_timer(1.0 / DC->FPS),
		"failed to create timer.");
	GAME_ASSERT(
		event_queue = al_create_event_queue(),
		"failed to create event queue.");

	debug_log("Game initialized.\n");
	game_init();
}

/**
 * @brief Initialize all auxiliary resources.
 */
void
Game::game_init() {
    DataCenter *DC = DataCenter::get_instance();
    SoundCenter *SC = SoundCenter::get_instance();
    ImageCenter *IC = ImageCenter::get_instance();
    FontCenter *FC = FontCenter::get_instance();

    // Set window icon
    game_icon = IC->get(game_icon_img_path);
    al_set_display_icon(display, game_icon);

    // Register events
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    // Init sound and font settings
    SC->init();
    FC->init();

    // Init UI
    ui = new UI();
    ui->init();

	// 加入角色和縮放大小
	DC->character1->init();
	

    // 加載主頁背景
    main_page = IC->get(mainpage_img_path);
    GAME_ASSERT(main_page != nullptr, "Failed to load main page background image.");

    // Load all possible backgrounds
    background1 = IC->get(background1_img_path);
	GAME_ASSERT(background1 != nullptr, "Failed to load background1 image.");
    // 定義按鈕範圍（例：Start Game 按鈕）
    start_button = {577, 447, 1027, 561, "Start Game"};

    debug_log("Game state: change to MAIN_MENU\n");
    state = STATE::MAIN_MENU; // 設置遊戲初始狀態為主頁
    al_start_timer(timer);
}


/**
 * @brief The function processes all data update.
 * @details The behavior of the whole game body is determined by its state.
 * @return Whether the game should keep running (true) or reaches the termination criteria (false).
 * @see Game::STATE
 */
bool
Game::game_update() {
	DataCenter *DC = DataCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();
	SoundCenter *SC = SoundCenter::get_instance();
	static ALLEGRO_SAMPLE_INSTANCE *background = nullptr;

	switch(state) {
		case STATE::MAIN_MENU: { // 主頁邏輯
            if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) { // 左鍵點擊
                if (DC->mouse.x >= start_button.x1 && DC->mouse.x <= start_button.x2 &&
                    DC->mouse.y >= start_button.y1 && DC->mouse.y <= start_button.y2) {
                    debug_log("<Game> state: change to START\n");
                    state = STATE::START; // 切換到遊戲開始狀態
                }
            }
            break;
        }
		case STATE::START: {
			static bool is_played = false;
			static ALLEGRO_SAMPLE_INSTANCE *instance = nullptr;
			if(!is_played) {
				instance = SC->play(game_start_sound_path, ALLEGRO_PLAYMODE_ONCE);
				DC->level->load_level(1);
				is_played = true;
			}

			if(!SC->is_playing(instance)) {
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			break;
		} case STATE::LEVEL: {    //遊戲進行中的地方
			//加入角色
			DC->character1->update(); // 更新角色邏輯
			DC->prop_god->update();

			static bool BGM_played = false;
			if(!BGM_played) {
				background = SC->play(background_sound_path, ALLEGRO_PLAYMODE_LOOP);
				BGM_played = true;
			}

			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to PAUSE\n");
				state = STATE::PAUSE;
			}
			if(DC->level->remain_monsters() == 0 && DC->monsters.size() == 0) {
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			if(DC->player->HP == 0) {
				debug_log("<Game> state: change to END\n");
				state = STATE::END;
			}
			break;
		} case STATE::PAUSE: {
			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to LEVEL\n");
				state = STATE::LEVEL;
			}
			break;
		} case STATE::END: {
			return false;
		}
	}
	// If the game is not paused, we should progress update.
	if(state != STATE::PAUSE) {
		DC->player->update();
		SC->update();
		ui->update();

		//快龍實作
		//DC->hero->update();
		// if(state != STATE::START) {
		// 	DC->level->update();
		OC->update();
		// }
	}
	// game_update is finished. The states of current frame will be previous states of the next frame.
	memcpy(DC->prev_key_state, DC->key_state, sizeof(DC->key_state));
	memcpy(DC->prev_mouse_state, DC->mouse_state, sizeof(DC->mouse_state));
	return true;
}

/**
 * @brief Draw the whole game and objects.
 */
void Game::game_draw() {
    DataCenter *DC = DataCenter::get_instance();
    FontCenter *FC = FontCenter::get_instance();
	OperationCenter *OC = OperationCenter::get_instance();

    al_clear_to_color(al_map_rgb(100, 100, 100));

    switch(state) {
        case STATE::MAIN_MENU: {
            // 繪製主頁背景
            al_draw_bitmap(main_page, 0, 0, 0);
            break;
        }
        case STATE::START: {
            al_draw_bitmap(background1, 0, 0, 0);
            break;
        }
        case STATE::LEVEL: {
            // 遊戲繪製邏輯
			al_draw_bitmap(background1, 0, 0, 0); // 使用與 START 相同的背景圖片
    		//debug_log("<Game> Drawing background for LEVEL state.\n");
			//畫出角色
            DC->character1->draw();                      // 繪製角色
			OC->draw();
            break;
        }
        case STATE::PAUSE: {
            al_draw_filled_rectangle(0, 0, DC->window_width, DC->window_height, al_map_rgba(50, 50, 50, 64));
            al_draw_text(
                FC->caviar_dreams[FontSize::LARGE], al_map_rgb(255, 255, 255),
                DC->window_width / 2., DC->window_height / 2.,
                ALLEGRO_ALIGN_CENTRE, "GAME PAUSED");
            break;
			
        }
    }

    al_flip_display();
}


Game::~Game() {
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
}
