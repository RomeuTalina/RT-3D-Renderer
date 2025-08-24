#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"

#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "renderer.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static TestRenderer t_renderer;

static int screen_width = 800;
static int screen_height = 600;

static int colorIdx = 0;
static int colors[6][3] {
    {255, 0, 0}, {0, 255, 0}, {0, 0, 255},
    {255, 0, 255}, {255, 255, 0}, {0, 255, 255}
};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    
    SDL_SetAppMetadata("Idk Man", "0.0", "net.olikujy.idkman");

    t_renderer = TestRenderer(screen_width, screen_height);

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Idk Man", screen_width, screen_height, 0, &window, &renderer)) {
        SDL_Log("Couldn't initialize the window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if(event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
} 

SDL_AppResult SDL_AppIterate(void *appstate) {
    
    SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    vec3d movementLeft = {0.0f, 0.0f, 0.0f};

    for(triangle& tri : t_renderer.getCube().tris) {

        tri.p[0] = transByVec(tri.p[0], movementLeft);
        tri.p[1] = transByVec(tri.p[1], movementLeft);
        tri.p[2] = transByVec(tri.p[2], movementLeft);

        int tempIdx = colorIdx/2;
        SDL_SetRenderDrawColor(renderer, 
                colors[tempIdx][0], colors[tempIdx][1], 
                colors[tempIdx][2], SDL_ALPHA_OPAQUE);

        colorIdx = (colorIdx + 1) % 12;

        triangle projected_tri;

        projected_tri.p[0] = multMatVec(tri.p[0], t_renderer.proj_mat); 
        projected_tri.p[1] = multMatVec(tri.p[1], t_renderer.proj_mat); 
        projected_tri.p[2] = multMatVec(tri.p[2], t_renderer.proj_mat); 

        for(vec3d& v : projected_tri.p) {
            
            v.x += 1.0f;
            v.y += 1.0f;

            v.x *= 0.5f * (float)screen_width;
            v.y *= 0.5f * (float)screen_height;
        }

        SDL_RenderLine(renderer, projected_tri.p[0].x, projected_tri.p[0].y, projected_tri.p[1].x, projected_tri.p[1].y);
        SDL_RenderLine(renderer, projected_tri.p[1].x, projected_tri.p[1].y, projected_tri.p[2].x, projected_tri.p[2].y);
        SDL_RenderLine(renderer, projected_tri.p[2].x, projected_tri.p[2].y, projected_tri.p[0].x, projected_tri.p[0].y);
    }
    
    SDL_RenderLine(renderer, 0, 0, 16, 16);

    SDL_RenderPresent(renderer);


    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {

}
