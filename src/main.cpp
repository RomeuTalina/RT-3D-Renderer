#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "geometry.h"
#include <iostream>
#include <ostream>

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <chrono>
#include "renderer.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Renderer t_renderer;

static int screen_width = 800;
static int screen_height = 600;

static std::chrono::steady_clock::time_point lastTime;
static double deltaTime = 0;

static renderableObject* cube1;
static renderableObject* cube2;
static renderableObject* cube3;
static renderableObject* cube4;

static int MAX_FPS = 180;

void setFPS(int fps) {
    MAX_FPS = fps;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    
    SDL_SetAppMetadata("Idk Man", "0.0", "net.olikujy.idkman");

    t_renderer = Renderer(screen_width, screen_height);
    t_renderer.activeObjects.reserve(4);

    cube1 = &createCube(t_renderer, {-2.0f, 0.0f, 3.0f}, 1.0f, 2.0f, 1.0f);
    cube2 = &createCube(t_renderer, {0.0f, 1.0f, 3.0f}, 1.0f, 1.0f, 1.0f);
    cube3 = &createCube(t_renderer, {0.0f, -1.0f, 3.0f}, 1.0f, 1.0f, 1.0f);
    cube4 = &createCube(t_renderer, {2.0f, 0.0f, 3.0f}, 1.0f, 1.0f, 1.0f);

    lastTime = std::chrono::steady_clock::now();

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Idk Man", screen_width, screen_height, 0, &window, &renderer)) {
        SDL_Log("Couldn't initialize the window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetWindowRelativeMouseMode(window, true);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if(event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    // Mouse events are separated from keyboard events because of how sdl works.
    switch(event->type){
    case SDL_EVENT_MOUSE_MOTION:
        t_renderer.getCameraReference().takeMouseInput(event);
        break;
    }

    return SDL_APP_CONTINUE;
} 

SDL_AppResult SDL_AppIterate(void *appstate) {

    using namespace std::chrono;

    auto now = steady_clock::now();
    auto maxFrameDuration = milliseconds(1000 / MAX_FPS);
    while(now - lastTime < maxFrameDuration) {
        now = steady_clock::now();
    }

    // Gets the frame time, and stores it inside of a double variable. ⛷️
    duration<double> delta = now - lastTime;
    deltaTime = delta.count();

    // Keyboard Inputs have to be in the iterate function so movement is smooth.
    t_renderer.getCameraReference().takeKeyboardInput(deltaTime);

    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    rotateObject(*cube3, {0.0f, 1.0f, 0.0f}, deltaTime);

    rotateObject(*cube1, {1.0f, 1.0f, 1.0f}, deltaTime);

    t_renderer.renderObjects(*renderer);

    SDL_RenderPresent(renderer);

    lastTime = now;

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}
