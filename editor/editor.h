#ifndef ENGINE_EDITOR_H
#define ENGINE_EDITOR_H

#include "../remc2/sub_main.h"
#include <kiss_sdl.h>

int main_x();
void editor_run();
void SetPixelMapSurface(int x, int y, int nx, int ny, Bit8u* adress);
void SetPixelMapSurfacefeat(int x, int y, int nx, int ny, Bit8u* adress);
void SetPixelMapSurfacecheck(int x, int y, int nx, int ny, Bit8u* adress);
void init_pal();
void clean_tarrain();
void editor_loop();
void terrain_recalculate();
void loadlevel(int levelnumber);
void editor_run();
void terrain_recalculate();
void fillterrain(kiss_terrain* terrain, float zoom, int beginx, int beginy);
void fillterraincheck(float zoom, int beginx, int beginy);
void fillterrainfeat(float zoom, int beginx, int beginy);

/*
  kiss_sdl widget toolkit
  Copyright (c) 2016 Tarvo Korrovits <tkorrovi@mail.com>

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
	 claim that you wrote the original software. If you use this software
	 in a product, an acknowledgment in the product documentation would
	 be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not
	 be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
	 distribution.

  kiss_sdl version 1.2.0
*/

static void text_reset(kiss_textbox* textbox, kiss_vscrollbar* vscrollbar);
static void terrain_feat_append(kiss_textbox* textbox1, kiss_vscrollbar* vscrollbar1);
static void terrain_stages_append(kiss_textbox* textbox);
static void terrain_vars_append(kiss_textbox* textbox);

/* The widget arguments are widgets that this widget talks with */
static int textbox1_event(kiss_textbox* textbox, SDL_Event* e, kiss_vscrollbar* vscrollbar1, int mousex, int mousey, int* draw);
static int textbox2_event(kiss_textbox* textbox, SDL_Event* e, int mousex, int mousey, int* draw);
static void vscrollbar1_event(kiss_vscrollbar* vscrollbar, SDL_Event* e, kiss_textbox* textbox1, int* draw);

static void vscrollbar2_event(kiss_vscrollbar* vscrollbar, SDL_Event* e, kiss_textbox* textbox2, int* draw);

static void button_ok1_event(kiss_button* button, SDL_Event* e, kiss_window* window1, kiss_window* window2, kiss_label* label_sel, kiss_entry* entry, kiss_label* label_res, kiss_progressbar* progressbar, int* draw);

kiss_window window1, window2, window3, window_selecttype, window_selectsubtype, window_selectcheck;
kiss_hex4edit hex4edit1feat = { 0 };
kiss_hex4edit hex4edit2feat = { 0 };
kiss_hex4edit hex4edit3feat = { 0 };
kiss_hex4edit hex4edit4feat = { 0 };
kiss_hex4edit hex4edit5feat = { 0 };
kiss_hex4edit hex4edit6feat = { 0 };
kiss_hex4edit hex4edit7feat = { 0 };
kiss_hex4edit hex4edit8feat = { 0 };
kiss_hex4edit hex4edit9feat = { 0 };
kiss_hex4edit hex4edit10feat = { 0 };

kiss_hex2edit hex2edit1check = { 0 };
kiss_hex4edit hex4edit2check = { 0 };
kiss_hex4edit hex4edit3check = { 0 };
kiss_hex4edit hex4edit4check = { 0 };


static void button_cancel_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static void button_savelevel_event(kiss_button* button, SDL_Event* e, int* draw);
static int button_cleanlevelfeat_event(kiss_button* button, SDL_Event* e, int* draw);
static void button_ok_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static void button_ok_check_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static int button_type_event(kiss_button* button, SDL_Event* e, int* draw, int index, kiss_window* oldwindow, kiss_window* window);
static void button_selecttype_event(kiss_button* button, SDL_Event* e, int* draw);
static void button_selectcheck_event(kiss_button* button, SDL_Event* e, int* draw);

kiss_image img_creature;
kiss_image img_type05_01;
kiss_image img_type05_02;
kiss_image img_type05_03;
kiss_image img_type05_04;
kiss_image img_type05_09;
kiss_image img_type05_0D;
kiss_image img_type05_13;

kiss_image img_type0A_01;
kiss_image img_type0A_06;
kiss_image img_type0A_16;
kiss_image img_type0A_27;
kiss_image img_type0A_3B;
kiss_image img_type0A_3C;
kiss_image img_type0A_1D;

kiss_image img_castle;
kiss_image img_trigger;

kiss_image img_none;
int max_subtype_buttons = 64;
bool first = true;

static void button_selectsubtype_event(kiss_button* button, kiss_button* buttons, SDL_Event* e, int* draw);
static int button_del_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static int button_add_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static int button_clean_event(kiss_button* button, SDL_Event* e, int* quit, int* draw);
static void button_ok2_event(kiss_button* button, SDL_Event* e, kiss_window* window1, kiss_window* window2, kiss_progressbar* progressbar, int* draw);
static int select1_event(kiss_selectbutton* select1, SDL_Event* e, kiss_selectbutton* select2, kiss_selectbutton* select3, kiss_selectbutton* select4, int* draw);
static int select2_event(kiss_selectbutton* select2, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select3, kiss_selectbutton* select4, int* draw);
static int select3_event(kiss_selectbutton* select3, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select2, kiss_selectbutton* select4, int* draw);
static int select4_event(kiss_selectbutton* select4, SDL_Event* e, kiss_selectbutton* select1, kiss_selectbutton* select2, kiss_selectbutton* select3, int* draw);
static int select1_feat_event(kiss_selectbutton* select, SDL_Event* e, kiss_array* objects, int* draw);

void SetSelected(kiss_terrain* terrain, type_str_0x30311* temp303array, bool* selecedarray);
int kiss_terrain_event(kiss_terrain* terrain, SDL_Event* event, int* draw, int mousex, int mousey, type_str_0x30311* temp303array, bool* selecedarray);
int findFirstSelected(bool* selected);

#endif //ENGINE_EDITOR_H