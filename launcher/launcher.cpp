#include "kiss_sdl.h"

void button_event(kiss_button* button, SDL_Event* e, int* draw, int* quit)
{
	if (kiss_button_event(button, e, draw)) *quit = 1;
}

int main(int argc, char** argv)
{
	SDL_Renderer* renderer;
	SDL_Event e;
	kiss_array objects;
	kiss_window window;
	kiss_label label = { 0 };
	kiss_button button = { 0 };
	char message[KISS_MAX_LENGTH];
	int draw, quit;
	quit = 0;
	draw = 1;
	kiss_array_new(&objects);
	char appName[] = "Hello kiss_sdl";
	renderer = kiss_init(appName, &objects, 320, 120);
	if (!renderer) return 1;
	kiss_window_new(&window, NULL, 0, 0, 0, kiss_screen_width,
		kiss_screen_height);
	strcpy(message, "Hello World!");

	kiss_label_new(&label, &window, message, 
		((window.rect.w / 2) - strlen(message)) * (kiss_textfont.advance / 2),
		((window.rect.h / 2) - ((kiss_textfont.fontheight + (2 * kiss_normal.h)) / 2)));
	label.textcolor.r = 255;
	char buttonName[] = "OK";
	kiss_button_new(&button, &window, buttonName,
		(window.rect.w / 2) - (kiss_normal.w / 2), 
		label.rect.y + kiss_textfont.fontheight + kiss_normal.h);
	window.visible = 1;
	while (!quit) {
		SDL_Delay(10);
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) quit = 1;
			kiss_window_event(&window, &e, &draw);
			button_event(&button, &e, &draw, &quit);
		}
		if (!draw) continue;
		SDL_RenderClear(renderer);
		kiss_window_draw(&window, renderer);
		kiss_label_draw(&label, renderer);
		kiss_button_draw(&button, renderer);
		SDL_RenderPresent(renderer);
		draw = 0;
	}
	kiss_clean(&objects);
	return 0;
}