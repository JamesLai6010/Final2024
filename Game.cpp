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
#include "Character/Character2.h"
// Prop
#include "Props/Props.h"
#include "Props/Prop_GOD.h"
// background inf
#include "Background/GameInformation.h"

//git testing1
// fixed settings
constexpr char game_icon_img_path[] = "./assets/image/game_icon.jpg";
constexpr char game_start_sound_path[] = "./assets/sound/growl.wav";

constexpr char background_sound_path[] = "./assets/sound/BackgroundMusic.ogg";
constexpr char mainpage_img_path[] = "./assets/image/mainpage.png";
constexpr char sceneSelection_img_path[] = "./assets/image/SelectScene.jpg";

constexpr char background1_img_path[] = "./assets/image/background1.png";
constexpr char background2_img_path[] = "./assets/image/background2.jpg";
constexpr char background3_img_path[] = "./assets/image/background3.jpg";


constexpr char character1_img_path[] = "./assets/image/character1.png";
constexpr char character2_img_path[] = "./assets/image/character2.png";
constexpr char character3_img_path[] = "./assets/image/character3.png";
constexpr char character4_img_path[] = "./assets/image/character4.png";

constexpr char playbtn_img_path[] = "./assets/image/play_btn.png";

constexpr char click_sound_path[] = "./assets/sound/click.mp3";
constexpr char mainPage_sound_path[] = "./assets/sound/mainPage.mp3";
constexpr char sceneSelect_sound_path[] = "./assets/sound/select.mp3";
constexpr char victory_sound_path[] = "./assets/sound/victory.mp3";
constexpr char game_sound_path[] = "./assets/sound/game.mp3";

constexpr char win1_img_path[] = "./assets/image/WIN_IMG/win1.jpg";
constexpr char win2_img_path[] = "./assets/image/WIN_IMG/win2.jpg";
constexpr char win3_img_path[] = "./assets/image/WIN_IMG/win3.jpg";
constexpr char win4_img_path[] = "./assets/image/WIN_IMG/win4.jpg";
constexpr char win5_img_path[] = "./assets/image/WIN_IMG/win.jpg";

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

		// 處理輸入邏輯（名稱輸入）
        if (name_input_active) {
            if (event.type == ALLEGRO_EVENT_KEY_CHAR) {
                char key = event.keyboard.unichar;
                //debug_log("Key pressed: %c (code: %d)\n", key, event.keyboard.unichar);

                // 處理特殊鍵
                if (key == '\b') { // Backspace
                    if (name_input_player == 1 && !player1_name.empty()) {
                        player1_name.pop_back();
                    } else if (name_input_player == 2 && !player2_name.empty()) {
                        player2_name.pop_back();
                    }
                } else if (key == '\r') { // Enter 完成輸入
                    name_input_active = false;
                    if (name_input_player == 1) {
                        player1_name_done = true; // 玩家一名稱輸入完成
                    }
                    if (name_input_player == 2) {
                        player2_name_done = true; // 玩家二名稱輸入完成
                    }
                    name_input_player = 0;
                } else if (isprint(key)) { // 一般可見字符
                    if (name_input_player == 1) {
                        player1_name += key;
                    } else if (name_input_player == 2) {
                        player2_name += key;
                    }
                }
            }
        }

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
				//printf("keydown\n");
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
			} case ALLEGRO_EVENT_KEY_CHAR: {
    			//printf("Event Type: %d\n", event.type);
    			//printf("Key pressed: %c (code: %d)\n", event.keyboard.unichar, event.keyboard.keycode);
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
	DC->character2->init();

	DC->background_inf->init();
    // 加載主頁背景
    main_page = IC->get(mainpage_img_path);
    
	// 場景選擇畫面
	sceneSelection = IC->get(sceneSelection_img_path);
	
    // Load all possible backgrounds
    background1 = IC->get(background1_img_path);
	background2 = IC->get(background2_img_path);
	background3 = IC->get(background3_img_path);

	// 場景圖
	character1 = IC->get(character1_img_path);
	character2 = IC->get(character2_img_path);
	character3 = IC->get(character3_img_path);
	character4 = IC->get(character4_img_path);

	// win img
	win_map1 = IC->get(win1_img_path);
	win_map2 = IC->get(win2_img_path);
	win_map3 = IC->get(win3_img_path);
	win_map4 = IC->get(win4_img_path);
	win_map5 = IC->get(win5_img_path);

	// player btn in character_selected
	playbtn = IC->get(playbtn_img_path);
    
    debug_log("Game state: change to MAIN_MENU\n");
    state = STATE::MAIN_MENU; // 設置遊戲初始狀態為主頁
    al_start_timer(timer);

	// 場景縮圖
	double mid_y = DC->window_height / 2;
	double mid_x = DC->window_width / 2;
	scene_thumbnails.push_back({IC->get("./assets/image/background1_small.jpg"), mid_x - 520 - 160, mid_y - 90, 320, 180, false, 1});
    scene_thumbnails.push_back({IC->get("./assets/image/background2_small.jpg"), mid_x - 160, mid_y - 90, 320, 180, false, 2});
    scene_thumbnails.push_back({IC->get("./assets/image/background3_small.jpg"), mid_x - 160 + 520, mid_y - 90, 320, 180, false, 3});

	select_character.push_back({character1, mid_x - 500, mid_y - 430, 300, 430, false, 1, 0});
	select_character.push_back({character2, mid_x + 200, mid_y - 430, 300, 430, false, 2, 0});
	select_character.push_back({character3, mid_x - 150, mid_y, 300, 430, false, 3, 0});
	//select_character.push_back({character4, mid_x + 200, mid_y, 300, 430, false, 4, 0});

	// 定義按鈕範圍（例：Start Game 按鈕）
    start_button = {577, 447, 1027, 561, "Start Game"};
	play_button = {mid_x - 127, mid_y - 40, mid_x + 127, mid_y + 40, "Play"};
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
	static ALLEGRO_SAMPLE_INSTANCE *click = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *mainPage = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *sceneSelect = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *gamebackgroundBGM = nullptr;
	static ALLEGRO_SAMPLE_INSTANCE *victorySceneBGM = nullptr;
	switch(state) {
		case STATE::MAIN_MENU: { // 主頁邏輯
			sceneSelectionBGM = false;
			gameBGM = false;
			victoryBGM = false;
            if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) { // 左鍵點擊
				printf("mouse_x = %lf mouse_y = %lf\n", DC->mouse.x, DC->mouse.y);
                if (DC->mouse.x >= start_button.x1 && DC->mouse.x <= start_button.x2 &&
                    DC->mouse.y >= start_button.y1 && DC->mouse.y <= start_button.y2) {
					click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                    debug_log("<Game> state: change to SCENE_SELECTION\n");
					SC->toggle_playing(mainPage);   //把mainPage bgm暫停
					//SC->toggle_playing(sceneSelect);
                    state = STATE::SCENE_SELECTION; // 切換狀態
                }
				if (DC->mouse.x >= 650 && DC->mouse.x <= 950 &&
                    DC->mouse.y >= 767 && DC->mouse.y <= 880) {
						click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                   		debug_log("<Game> state: change to END\n");
						state = STATE::END; // 切換狀態
				}
            }

			static bool BGM_played = false;
			if(!BGM_played) {
				mainPage = SC->play(mainPage_sound_path, ALLEGRO_PLAYMODE_LOOP);
				BGM_played = true;
			}

            break;
        }
		case STATE::SCENE_SELECTION: {
			
            // 選擇場景
			// 檢查場景縮圖是否被滑鼠懸停
			if (!sceneSelectionBGM) {
    			sceneSelect = SC->play(sceneSelect_sound_path, ALLEGRO_PLAYMODE_LOOP);
    			sceneSelectionBGM = true;
			}
    		for (auto &scene : scene_thumbnails) {
        		// 檢查鼠標是否在該縮圖範圍內
        		if (DC->mouse.x >= scene.x && DC->mouse.x <= scene.x + scene.width &&
            		DC->mouse.y >= scene.y && DC->mouse.y <= scene.y + scene.height) {
            		scene.is_hovered = true;  // 滑鼠懸停
					//printf("Hovered over scene at (%f, %f)\n", scene.x, scene.y); // 調試輸出
        		} else {
            		scene.is_hovered = false; // 滑鼠不懸停
        		}

				if (DC->mouse_state[1] && !DC->prev_mouse_state[1] && scene.is_hovered) { // 左鍵點擊
					scene_number = scene.number;
					click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
					debug_log("<Game> state: change to CHARACTER_SELECTION, scene is %d\n", scene_number);
					player_turn = 1;
					SC->toggle_playing(sceneSelect);   //暫停
					sceneSelectionBGM = false;
					name_input_active = false; // 是否正在輸入名稱
    				name_input_player = 0; // 當前輸入名稱的玩家 (1 或 2)
    				player1_name_done = false; // 玩家一名稱是否完成輸入
    				player2_name_done = false; // 玩家一名稱是否完成輸入
                    state = STATE::CHARACTER_SELECTION; // 切換到遊戲開始狀態
            	}
    		}
            //back to last scene
			if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) { // 左鍵點擊
                if (DC->mouse.x >= 25 && DC->mouse.x <= 225 &&
                    DC->mouse.y >= 810 && DC->mouse.y <= 880) {
					click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                    debug_log("<Game> state: change to MAIN\n");
					sceneSelectionBGM = true;
					SC->toggle_playing(mainPage);
					SC->toggle_playing(sceneSelect);
                    state = STATE::MAIN_MENU; // 切換狀態
                }
            }
            break;
        }
		case STATE::CHARACTER_SELECTION: {
    		if (!sceneSelectionBGM) {
        		SC->toggle_playing(sceneSelect);   //繼續播放
        		sceneSelectionBGM = true;
    		}
			for (auto &character : select_character) {
        		// 檢查鼠標是否在該縮圖範圍內
        		if (DC->mouse.x >= character.x && DC->mouse.x <= character.x + character.width &&
            		DC->mouse.y >= character.y && DC->mouse.y <= character.y + character.height) {
            		character.is_hovered = true;  // 滑鼠懸停
					//printf("Hovered over scene at (%f, %f)\n", scene.x, scene.y); // 調試輸出
        		} else {
            		character.is_hovered = false; // 滑鼠不懸停
        		}

				// 當玩家1或玩家2選擇角色時處理
                if (DC->mouse_state[1] && !DC->prev_mouse_state[1] && character.is_hovered) {
                    if (player_turn == 1) {
						click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                        player1_character = character; // 記錄玩家1選擇的角色
                        debug_log("<Game> Player 1 selected character %d\n", character.number);

						// 啟動玩家一輸入名稱
                		name_input_active = true;
                		name_input_player = 1;

						player_turn = 2;
                    } else if (player_turn == 2 && player1_name_done){
						click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                        player2_character = character; // 記錄玩家2選擇的角色
                        debug_log("<Game> Player 2 selected character %d\n", character.number);

						// 啟動玩家二輸入名稱
                		name_input_active = true;
                		name_input_player = 2;

						player_turn = 3;
                    }
                    // 讓玩家切換，或者繼續進行遊戲
                }

    		}
			

    		// 如果按下開始按鈕且玩家名稱已完成輸入
    		if (player_turn == 3 && !name_input_active &&
        		DC->mouse_state[1] && !DC->prev_mouse_state[1] &&
        		DC->mouse.x >= play_button.x1 && DC->mouse.x <= play_button.x2 &&
        		DC->mouse.y >= play_button.y1 && DC->mouse.y <= play_button.y2 && player2_name_done) {
        		debug_log("<Game> state: change to LEVEL\n");
        		apply_character_selection();
				//之後加入分配技能
        		click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
        		SC->toggle_playing(sceneSelect);

				debug_log("Player 1 Name: %s\n", player1_name.c_str());
    			debug_log("Player 2 Name: %s\n", player2_name.c_str());
				set_player_roles();
				
        		state = STATE::LEVEL;
				DC->background_inf->_set_time();
   			}
			//back to last scene
			if (DC->mouse_state[1] && !DC->prev_mouse_state[1]) { // 左鍵點擊
                if (DC->mouse.x >= 25 && DC->mouse.x <= 225 &&
                    DC->mouse.y >= 810 && DC->mouse.y <= 880) {
					click = SC->play(click_sound_path, ALLEGRO_PLAYMODE_ONCE);
                    debug_log("<Game> state: change to SCENE_SELECTION\n");
					gameBGM = false;
                    state = STATE::SCENE_SELECTION; // 切換狀態
                }
            }
			break;
		}
		case STATE::LEVEL: {    //遊戲進行中的地方
			//加入角色
			DC->character1->update(); // 更新角色邏輯
			DC->character2->update(); // 更新角色邏輯
			DC->prop_god->update();
			DC->background_inf->update();

			if (!gameBGM) {
    			gamebackgroundBGM = SC->play(game_sound_path, ALLEGRO_PLAYMODE_LOOP);
    			gameBGM = true;
			}

			if(DC->key_state[ALLEGRO_KEY_P] && !DC->prev_key_state[ALLEGRO_KEY_P]) {
				SC->toggle_playing(background);
				debug_log("<Game> state: change to PAUSE\n");
				state = STATE::PAUSE;
			}
			// if(DC->level->remain_monsters() == 0 && DC->monsters.size() == 0) {
			// 	debug_log("<Game> state: change to END\n");
			// 	state = STATE::END;
			// }
			// if(DC->player->HP == 0) {
			// 	debug_log("<Game> state: change to END\n");
			// 	state = STATE::END;
			// }
			if (DC->character1->_get_HP() <= 0 || DC->character2->_get_HP() <= 0 || DC->background_inf->get_Time() <= 0){
				double player1_hp = DC->character1->_get_HP();
				double player2_hp = DC->character2->_get_HP();
				
				if (player1_hp == player2_hp){
					no_winner = true;
				}else if (player1_hp > player2_hp){
					player1_win = true;
				}else{
					player1_win = false;
				}
				SC->toggle_playing(gamebackgroundBGM);
				DC->prop_god->clear_all_props();
				victoryBGM = false;
				state = STATE::Fight_FINISH;
			}
			break;
		} 
		case STATE::Fight_FINISH:{
			if(!victoryBGM) {
				victorySceneBGM = SC->play(victory_sound_path, ALLEGRO_PLAYMODE_LOOP);
				victoryBGM = true;
			}
			if (DC->key_state[ALLEGRO_KEY_ENTER] && !DC->prev_key_state[ALLEGRO_KEY_ENTER]){
				SC->toggle_playing(victorySceneBGM);
				SC->toggle_playing(mainPage);
				reset_game(); // 重新初始化遊戲
				state = STATE::MAIN_MENU;
			}
			break;
		}
		case STATE::PAUSE: {
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
		OC->update();

		//快龍實作
		//DC->hero->update();
		// if(state != STATE::START) {
		// 	DC->level->update();
		// 	OC->update();
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
		case STATE::SCENE_SELECTION: {
			al_draw_bitmap(sceneSelection, 0, 0, 0);
			
			for (auto &scene : scene_thumbnails) {
                if (scene.is_hovered) {
    				// 使用不同顏色的邊框或修改顏色亮度
					al_draw_filled_rectangle(scene.x-20, scene.y-20, scene.x + scene.width+20, scene.y + scene.height+20, al_map_rgba(255, 255, 255, 128));
					//printf("tinted");
    				al_draw_bitmap(scene.thumbnail, scene.x, scene.y, 0);
				} else {
    				al_draw_bitmap(scene.thumbnail, scene.x, scene.y, 0);
				}
            }
			al_draw_text(
                FC->SuperMarioBros[FontSize::XL], al_map_rgb(255, 255, 255),
                30, DC->window_height-FontSize::XL - 10,
                ALLEGRO_ALIGN_LEFT, "BACK");
			
			break;
		}
		case STATE::CHARACTER_SELECTION: {
			if (scene_number == 1) {
				al_draw_bitmap(background1, 0, 0, 0);
			} else if (scene_number == 2) {
				al_draw_bitmap(background2, 0, 0, 0);
			} else if (scene_number == 3) {
				al_draw_bitmap(background3, 0, 0, 0);
			}
			
			
			// 繪製玩家1選擇的角色高亮
            for (auto &character : select_character) {

				//選中的高亮留著
				if (player1_character.number == character.number) 
					al_draw_filled_rectangle(character.x, character.y, character.x + character.width, character.y + character.height / 2, al_map_rgba(169, 169, 169, 0.8));
				if (player2_character.number == character.number)
					al_draw_filled_rectangle(character.x, character.y + character.height / 2, character.x + character.width, character.y + character.height, al_map_rgba(247, 247, 1, 0.8));
                //選中的高亮留著

				if (character.is_hovered) {
                    if (player_turn == 1) {
                        // 玩家1高亮 (白色，角色上半部)
                        al_draw_filled_rectangle(character.x, character.y, character.x + character.width, character.y + character.height / 2, al_map_rgba(169, 169, 169, 0.8));
                    } else if (player_turn == 2) {
                        // 玩家2高亮 (灰色，角色下半部)
                        al_draw_filled_rectangle(character.x, character.y + character.height / 2, character.x + character.width, character.y + character.height, al_map_rgba(247, 247, 1, 0.8));
                    }
                    al_draw_bitmap(character.character, character.x, character.y, 0);
                } else {
                    al_draw_bitmap(character.character, character.x, character.y, 0);
                }
            }
			if (player_turn == 3 && player2_name_done) {
				al_draw_bitmap(playbtn, play_button.x1, play_button.y1, 0);
			}

			al_draw_text(
                FC->SuperMarioBros[FontSize::XL], al_map_rgb(255, 255, 255),
                30, DC->window_height-FontSize::XL - 10,
                ALLEGRO_ALIGN_LEFT, "BACK");
			
			if (player_turn == 1 && !name_input_active) {
				al_draw_text(
                FC->pirulen[FontSize::XL], al_map_rgb(0, 0, 0),
                DC->window_width/2, DC->window_height/2,
                ALLEGRO_ALIGN_CENTER, "Player 1 TURN");	
			} else if (player_turn == 2 && !name_input_active) {
				al_draw_text(
                FC->pirulen[FontSize::XL], al_map_rgb(0, 0, 0),
                DC->window_width/2, DC->window_height/2,
                ALLEGRO_ALIGN_CENTER, "Player 2 TURN");
			}

			// 如果正在輸入名稱，繪製輸入框和提示文字
    		if (name_input_active) {
        		float box_width = 400;
        		float box_height = 50;

        		// 輸入框位置（正中間）
        		float box_x = DC->window_width / 2 - box_width / 2;
        		float box_y = DC->window_height / 2 - box_height / 2;

        		// 提示文字位置
        		float text_x = DC->window_width / 2;
        		float text_y = box_y - 60;

        		// 繪製背景框
        		al_draw_filled_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgb(0, 0, 0));
        		al_draw_rectangle(box_x, box_y, box_x + box_width, box_y + box_height, al_map_rgb(255, 255, 255), 2);

        		// 繪製提示文字
        		al_draw_text(
            		FC->SuperMarioBros[FontSize::XL], al_map_rgb(255, 255, 255),
            		text_x, text_y, ALLEGRO_ALIGN_CENTER, "NickName");

        		// 繪製輸入的名稱
        		std::string current_name = (name_input_player == 1) ? player1_name : player2_name;
        		al_draw_text(
            		FC->SuperMarioBros[FontSize::LARGE], al_map_rgb(255, 255, 255),
            		DC->window_width / 2, box_y + 10, ALLEGRO_ALIGN_CENTER,
            		current_name.c_str());
				
				al_draw_text(
            		FC->SuperMarioBros[FontSize::LARGE], al_map_rgb(0, 0, 0),
            		text_x, box_y + 60, ALLEGRO_ALIGN_CENTER, "PRESS ENTER");
    		}

    		al_draw_text(
        		FC->SuperMarioBros[FontSize::XL], al_map_rgb(255, 255, 255),
        		30, DC->window_height - FontSize::XL - 10,
        		ALLEGRO_ALIGN_LEFT, "BACK");

			break;
		}
        case STATE::START: {
            al_draw_bitmap(background1, 0, 0, 0);
            break;
        }
        case STATE::LEVEL: {
            // 遊戲繪製邏輯
			if (scene_number == 1) {
				al_draw_bitmap(background1, 0, 0, 0);
			} else if (scene_number == 2) {
				al_draw_bitmap(background2, 0, 0, 0);
			} else if (scene_number == 3) {
				al_draw_bitmap(background3, 0, 0, 0);
			}
			// show time
			int mins = DC->background_inf->get_Time() / 60;
			int secs = DC->background_inf->get_Time() % 60;
			char txt[6];txt[0] = mins/10 + '0';txt[1] = mins%10 + '0';txt[3] = secs/10 + '0';txt[4] = secs%10 + '0';txt[2] = ':';txt[5] = '\0';				
			al_draw_text(FC->SuperMarioBros[FontSize::XL], al_map_rgb(255, 255, 255), 800, 50, ALLEGRO_ALIGN_CENTER, txt);
    		
			//debug_log("<Game> Drawing background for LEVEL state.\n");
			//畫出角色
            //畫出角色，攻擊者在前
			if (DC->character1->_get_state() == CharacterState::ATTACK1 ||
				DC->character1->_get_state() == CharacterState::ATTACK2 ||
				DC->character1->_get_state() == CharacterState::ATTACK3) {
				DC->character2->draw();		//2在後
				DC->character1->draw();		//1在前
			} else {
				DC->character1->draw();		//1在後
				DC->character2->draw();		//2在前
			}
			DC->background_inf->draw();
			OC->draw();
            break;
        }
		case STATE::Fight_FINISH:{
			char txt[10000];
			if (no_winner){
				std::strcpy(txt, "NO WINNER!");
			}else if (player1_win){
				std::string ss = player1_name + " WIN!";
				std::strcpy(txt,ss.c_str());
			}else{
				std::string ss = player2_name + " WIN!";
				std::strcpy(txt,ss.c_str());
			}
			
			
			if ((player1_win && player1_character.number == 1) 
			       || (!player1_win && player2_character.number == 1)){
				al_draw_bitmap(win_map1, 0, 0, 0);
			}else if ((player1_win && player1_character.number == 2) 
			       || (!player1_win && player2_character.number == 2)){
				al_draw_bitmap(win_map2, 0, 0, 0);
			}else if ((player1_win && player1_character.number == 3) 
			       || (!player1_win && player2_character.number == 3)){
				al_draw_bitmap(win_map2, 0, 0, 0);
			}else if ((player1_win && player1_character.number == 4) 
			       || (!player1_win && player2_character.number == 4)){
				al_draw_bitmap(win_map2, 0, 0, 0);
			}else{
				al_draw_bitmap(win_map5, 0, 0, 0);
			}
			al_draw_text(FC->SuperMarioBros[FontSize::XXL], al_map_rgb(255, 255, 255), 800, 200, ALLEGRO_ALIGN_CENTER, txt);

			al_draw_text(FC->SuperMarioBros[FontSize::LARGE], al_map_rgb(250, 255, 20), 800, 800, ALLEGRO_ALIGN_CENTER, "PRESS ENTER TO MAIN");
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

void Game::set_player_roles() {
    OperationCenter* OC = OperationCenter::get_instance();
    OC->set_player_roles(player1_character.number, player2_character.number);
}

void Game::apply_character_selection() {
    DataCenter* DC = DataCenter::get_instance();

    // 玩家1選擇角色
    if (player1_character.number == 1) {
        DC->character1->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character1/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character1/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character1/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character1/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character1/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character1/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character1/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character1/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character1/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character1/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character1/Ice.gif"}
        });
    } else if (player1_character.number == 2) {    //之後有3、4
        DC->character1->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character2/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character2/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character2/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character2/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character2/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character2/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character2/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character2/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character2/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character2/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character2/Ice.gif"}
        });
    } else if (player1_character.number == 3) {    //之後有3、4
        DC->character1->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character3/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character3/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character3/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character3/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character3/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character3/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character3/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character3/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character3/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character3/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character3/Ice.gif"}
        });
	}

    // 玩家2選擇角色
    if (player2_character.number == 1) {
        DC->character2->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character1/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character1/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character1/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character1/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character1/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character1/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character1/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character1/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character1/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character1/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character1/Ice.gif"}
        });
    } else if (player2_character.number == 2) {
        DC->character2->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character2/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character2/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character2/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character2/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character2/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character2/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character2/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character2/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character2/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character2/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character2/Ice.gif"}
        });
    } else if (player2_character.number == 3) {    //之後有3、4
        DC->character2->reset_gif_paths({
            {CharacterState::WALK, "./assets/gif/Character3/Walk.gif"},
            {CharacterState::RUN, "./assets/gif/Character3/Run.gif"},
            {CharacterState::JUMP, "./assets/gif/Character3/Jump.gif"},
            {CharacterState::ATTACK1, "./assets/gif/Character3/Attack1.gif"},
            {CharacterState::ATTACK2, "./assets/gif/Character3/Attack2.gif"},
            {CharacterState::ATTACK3, "./assets/gif/Character3/Attack3.gif"},
            {CharacterState::STOP, "./assets/gif/Character3/Stop.gif"},
            {CharacterState::SHIELD, "./assets/gif/Character3/Shield.gif"},
            {CharacterState::HURT, "./assets/gif/Character3/Hurt.gif"},
            {CharacterState::DEAD, "./assets/gif/Character3/Dead.gif"},
			{CharacterState::FREEZE, "./assets/gif/Character3/Ice.gif"}
        });
	}
}


void Game::reset_game() {
    DataCenter* DC = DataCenter::get_instance();
    OperationCenter* OC = OperationCenter::get_instance();
	DC->character1->reset();
	DC->character2->reset();
	DC->background_inf->init();
	DC->prop_god->reset();
    // 重設遊戲狀態
    
	player_turn = 1;
    player1_win = false;
    no_winner = false;
	player1_name = ""; // 玩家一名稱
    player2_name = ""; // 玩家二名稱
    name_input_active = false; // 是否正在輸入名稱
    name_input_player = 0; // 當前輸入名稱的玩家 (1 或 2)
    player1_name_done = false; // 玩家一名稱是否完成輸入
    player2_name_done = false; // 玩家一名稱是否完成輸入
    

	sceneSelectionBGM = false;
    gameBGM = false;
    victoryBGM = false;
	state = STATE::MAIN_MENU; // 返回主選單

    // 重置數據中心
    //DC->reset();
    // 重置操作中心狀態
    //OC->reset();
}