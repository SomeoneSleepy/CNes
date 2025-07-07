#include "gui.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

#include "gui.h"
#include <SDL2/SDL.h>

void gui_handle_event(SDL_Event* event, bool* running)
{
    if (event->type == SDL_QUIT)
    {
        *running = false;
    }
}
