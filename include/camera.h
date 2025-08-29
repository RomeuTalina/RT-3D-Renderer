#ifndef CAMERA_H
#define CAMERA_H

#include "SDL3/SDL_events.h"
#include "SDL3/SDL_keyboard.h"
#include "SDL3/SDL_stdinc.h"
#include "geometry.h"
#include <iostream>

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

    void takeInput(double deltaTime) {
        const bool *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_W]) {
                vec3d forwards = normalize(rotate({0.0f, 0.0f, 1.0f}, rot, pos)) * 10; 
                setPos(this->pos + (forwards * deltaTime));
            }
            if(state[SDL_SCANCODE_S]) {
                vec3d forwards = normalize(rotate({0.0f, 0.0f, 1.0f}, rot, pos)) * 10; 
                setPos(this->pos - (forwards * deltaTime));
            }
        }
    };

#endif

