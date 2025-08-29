#ifndef CAMERA_H
#define CAMERA_H

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_scancode.h"
#include "geometry.h"

class Camera {

public:

    vec3d pos {0.0f, 0.0f, 0.0f};
    vec3d rot {0.0f, 0.0f, 0.0f};
    vec3d vel {0.0f, 0.0f, 0.0f};

    float z_near {};
    float z_far {};
    float fov {};
    float aspect_ratio {};
    float f_scaling {};
    float q_scaling {};

    Camera(){}

    void setPos(vec3d pos) {this->pos = pos;};

    mat4x4 getViewMatrix() {

        mat4x4 rotationViewMatrix = createRotationMatrix({-rot.x, -rot.y, -rot.z});

        mat4x4 translationViewMatrix = createTranslationMatrix({-pos.x, -pos.y, -pos.z}); 

        return multMatMat4x4(rotationViewMatrix, translationViewMatrix);
    }

    void updatePosition() {
        pos = pos + vel * 0.1;
        vel = {0.0f, 0.0f, 0.0f};
    }

    void takeKeyboardInput(double deltaTime) {
        const bool *state = SDL_GetKeyboardState(NULL);
        if(state[SDL_SCANCODE_W]) {
            vec3d forwards = normalize(rotate({0.0f, 0.0f, 1.0f}, rot, pos));
            vel = vel + (forwards * deltaTime);
        }
        if(state[SDL_SCANCODE_S]) {
            vec3d forwards = normalize(rotate({0.0f, 0.0f, 1.0f}, rot, pos));
            vel = vel - (forwards * deltaTime);
        }
        if(state[SDL_SCANCODE_D]) {
            vec3d right = normalize(rotate({1.0f, 0.0f, 0.0f}, rot, pos));
            vel = vel + (right * deltaTime);
        }
        if(state[SDL_SCANCODE_A]) {
            vec3d right = normalize(rotate({1.0f, 0.0f, 0.0f}, rot, pos));
            vel = vel - (right * deltaTime);
        }
        if(state[SDL_SCANCODE_LCTRL]) {
            vec3d up = normalize(rotate({0.0f, 1.0f, 0.0f}, rot, pos));
            vel = vel + (up * deltaTime);
        }
        if(state[SDL_SCANCODE_SPACE]) {
            vec3d up = normalize(rotate({0.0f, 1.0f, 0.0f}, rot, pos));
            vel = vel - (up * deltaTime);
        }

        vel = normalize(vel);

        updatePosition();
    }

    void takeMouseInput(SDL_Event *event) {
        int dx = event->motion.yrel;    
        int dy = event->motion.xrel;    

        rot.x -= dx * 0.002f;
        rot.y += dy * 0.002f;

        rot = normalize(rot);

        std::cout << rot << std::endl;
    }
};

#endif
