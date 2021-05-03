#pragma once

#ifndef GRAPHICS_SETTINGS_FORM_H
#define GRAPHICS_SETTINGS_FORM_H

#include "kiss_sdl.h"

class GraphicsSettingsForm
{

private:

	kiss_label lblTitle;
	kiss_combobox cboResolution;
	kiss_button btnSave;
	kiss_button btnCancel;

public:

	GraphicsSettingsForm(kiss_window* window, int w, int h);

};

#endif //GRAPHICS_SETTINGS_FORM_H