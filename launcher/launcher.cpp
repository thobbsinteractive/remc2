#include "kiss_sdl.h"

#include <tchar.h>

void btnEditor_event(kiss_button* button, SDL_Event* e, int* draw, int* quit)
{
	if (kiss_button_event(button, e, draw))
	{
		*quit = 1;
	}
}

void btnLaunch_event(kiss_button* button, SDL_Event* e, int* draw, int* quit)
{
	if (kiss_button_event(button, e, draw))
	{
		*quit = 1;

#ifdef _MSC_VER

		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		LPTSTR szCmdline = _tcsdup(TEXT("remc2"));

		if (!CreateProcess(NULL,   // No module name (use command line)
			szCmdline,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory 
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return;
		}
#endif
	}
}


void btnExit_event(kiss_button* button, SDL_Event* e, int* draw, int* quit)
{
	if (kiss_button_event(button, e, draw))
	{
		*quit = 1;
	}
}

int main(int argc, char** argv)
{
	int padding = 20;

	char titleText[] = "Magic Carpet HD Launcher";

	char lblTitleText[] = "Magic Carpet HD Launcher";
	kiss_label lblTitle = { 0 };

	SDL_Renderer* renderer;
	SDL_Event e;
	kiss_array objects;
	kiss_window window;

	char btnEditorText[] = "Launch Editor";
	kiss_button btnEditor = { 0 };
	char btnLaunchText[] = "Launch Game";
	kiss_button btnLaunch = { 0 };
	char btnExitText[] = "Exit";
	kiss_button btnExit = { 0 };

	char message[KISS_MAX_LENGTH];
	int draw, quit;
	quit = 0;
	draw = 1;

	kiss_array_new(&objects);

	renderer = kiss_init(titleText, &objects, 300, 600);

	if (!renderer) return 1;

	kiss_window_new(&window, NULL, 0, 0, 0, kiss_screen_width, kiss_screen_height);

	kiss_label_new(&lblTitle, &window, lblTitleText,
		(window.rect.w / 2) - (strlen(lblTitleText) * (kiss_textfont.advance / 2)),
		padding);

	lblTitle.textcolor.r = 255;

	kiss_button_new(&btnEditor, &window, btnEditorText,
		(window.rect.w / 2) - (kiss_normal.w / 2), 
		lblTitle.rect.y + padding + kiss_textfont.fontheight + kiss_normal.h);

	kiss_button_new(&btnLaunch, &window, btnLaunchText,
		(window.rect.w / 2) - (kiss_normal.w / 2),
		btnEditor.rect.y + kiss_textfont.fontheight + kiss_normal.h);

	kiss_button_new(&btnExit, &window, btnExitText,
		(window.rect.w / 2) - (kiss_normal.w / 2),
		window.rect.h - padding - kiss_normal.h);

	window.visible = 1;

	while (!quit) {

		SDL_Delay(10);

		while (SDL_PollEvent(&e)) 
		{
			if (e.type == SDL_QUIT) 
				quit = 1;

			kiss_window_event(&window, &e, &draw);

			btnEditor_event(&btnEditor, &e, &draw, &quit);
			btnLaunch_event(&btnLaunch, &e, &draw, &quit);
			btnExit_event(&btnExit, &e, &draw, &quit);
		}

		if (!draw)
			continue;

		SDL_RenderClear(renderer);
		kiss_window_draw(&window, renderer);

		kiss_label_draw(&lblTitle, renderer);

		kiss_button_draw(&btnEditor, renderer);
		kiss_button_draw(&btnLaunch, renderer);
		kiss_button_draw(&btnExit, renderer);

		SDL_RenderPresent(renderer);
		draw = 0;
	}

	kiss_clean(&objects);
	return 0;
}
