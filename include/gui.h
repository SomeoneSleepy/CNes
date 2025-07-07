// gui.h
#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <stdbool.h>

void gui_handle_event(SDL_Event* event, bool* should_quit);
void gui_render(SDL_Renderer* renderer);

#endif