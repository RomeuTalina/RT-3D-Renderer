#ifndef RENDERER_H
#define RENDERER_H

#include <SDL3/SDL.h>
#include "SDL3/SDL_render.h"
#include <cassert>
#include "camera.h"
#include <vector>
#include <geometry.h>

struct mesh {
    std::vector<triangle> tris;
};

struct renderableObject {
    mesh m;
    vec3d center;
};

class Renderer;

void renderObject(SDL_Renderer*, const Renderer&, renderableObject&);

class Renderer {
    
private:
    int m_screen_width {};
    int m_screen_height {};
    Camera m_cam {};

public:
    std::vector<renderableObject> activeObjects;
    mat4x4 proj_mat {};

    Renderer (){};

Renderer(int screen_width, int screen_height):
    m_screen_width {screen_width},
    m_screen_height {screen_height}
{
    m_cam.z_near = 0.1f;  
    m_cam.z_far = 1000.0f; 
    m_cam.fov = 90.0f;
    m_cam.aspect_ratio = (float)m_screen_height / (float)m_screen_width; 
    m_cam.f_scaling = 1.0f / (tanf(m_cam.fov*0.5f / 180.0f * 3.14159f));
    m_cam.q_scaling = m_cam.z_far / (m_cam.z_far - m_cam.z_near);

        proj_mat.m[0][0] = m_cam.aspect_ratio * m_cam.f_scaling;
        proj_mat.m[1][1] = m_cam.f_scaling;
        proj_mat.m[2][2] = m_cam.q_scaling;
        proj_mat.m[3][2] = -m_cam.z_near * m_cam.q_scaling;
        proj_mat.m[2][3] = 1.0f;

        for(int i = 0; i < 3; i++){
            assert(proj_mat.m[i][i] != 0.0f);
        }
    }

    int getScreenWidth() const {return m_screen_width;};
    int getScreenHeight() const {return m_screen_height;};
    Camera getCamera() const {return m_cam;};
    Camera& getCameraReference() {return m_cam;};

    void addObject(renderableObject o) {activeObjects.emplace_back(o);};

    void renderObjects(SDL_Renderer& renderer) {
        for(renderableObject& o : activeObjects) {
            renderObject(&renderer, *this, o);
        }
    };

    renderableObject& getLastObject() {return activeObjects.back();};
};


// Creates a cuboid !! 👍
// The first argument defines the center of the cuboid.
// You must pass the renderer that you want to render the cuboid. Yes I know it's always
// gonna be the same renderer but I don't want global variables this is cleaner.
inline renderableObject& createCube(Renderer& renderer, vec3d pos, float width, float height, float depth) {
    mesh cubeMesh {}; 
    cubeMesh.tris.reserve(12);
    cubeMesh.tris = { 
        // NORTH
        triangle {pos.x-width/2, pos.y-height/2, pos.z+depth/2,  pos.x-width/2, pos.y+height/2, pos.z+depth/2,  pos.x+width/2, pos.y+height/2, pos.z+depth/2},
        triangle {pos.x-width/2, pos.y-height/2, pos.z+depth/2,  pos.x+width/2, pos.y+height/2, pos.z+depth/2,  pos.x+width/2, pos.y-height/2, pos.z+depth/2},

        // EAST
        triangle {pos.x+width/2, pos.y-height/2,  pos.z-depth/2,  pos.x+width/2, pos.y+height/2, pos.z-depth/2,  pos.x+width/2, pos.y+height/2, pos.z+depth/2},
        triangle {pos.x+width/2, pos.y-height/2,  pos.z-depth/2,  pos.x+width/2, pos.y+height/2, pos.z+depth/2,  pos.x+width/2, pos.y-height/2, pos.z+depth/2},

        // WEST
        triangle {pos.x-width/2, pos.y-height/2,  pos.z+depth/2,  pos.x-width/2, pos.y+height/2,  pos.z+depth/2,  pos.x-width/2, pos.y+height/2,  pos.z-depth/2},
        triangle {pos.x-width/2, pos.y-height/2,  pos.z+depth/2,  pos.x-width/2, pos.y+height/2,  pos.z-depth/2,  pos.x-width/2, pos.y-height/2,  pos.z-depth/2},

        // TOP 
        triangle {pos.x-width/2, pos.y+height/2,  pos.z-depth/2,  pos.x-width/2, pos.y+height/2,  pos.z+depth/2,  pos.x+width/2, pos.y+height/2,  pos.z+depth/2},
        triangle {pos.x-width/2, pos.y+height/2,  pos.z-depth/2,  pos.x+width/2, pos.y+height/2,  pos.z+depth/2,  pos.x+width/2, pos.y+height/2,  pos.z-depth/2},

        // BOTTOM
        triangle {pos.x-width/2, pos.y-height/2,  pos.z-depth/2,  pos.x-width/2, pos.y-height/2,  pos.z+depth/2,  pos.x+width/2, pos.y-height/2,  pos.z+depth/2},
        triangle {pos.x-width/2, pos.y-height/2,  pos.z-depth/2,  pos.x+width/2, pos.y-height/2,  pos.z+depth/2,  pos.x+width/2, pos.y-height/2,  pos.z-depth/2},

        // SOUTH
        triangle {pos.x-width/2, pos.y-height/2, pos.z-depth/2,  pos.x-width/2, pos.y+height/2, pos.z-depth/2,   pos.x+width/2, pos.y+height/2, pos.z-depth/2},
        triangle {pos.x-width/2, pos.y-height/2, pos.z-depth/2,  pos.x+width/2, pos.y+height/2, pos.z-depth/2,   pos.x+width/2, pos.y-height/2, pos.z-depth/2},
    };

    assert(cubeMesh.tris.size() == 12);

    renderableObject cube {
        cubeMesh,
        pos
    };
    
    renderer.addObject(cube);

    return renderer.getLastObject();
}

inline void renderObject(SDL_Renderer* renderer, const Renderer& rt_renderer, renderableObject& obj) {

    mat4x4 viewMatrix = rt_renderer.getCamera().getViewMatrix();

    for(triangle& tri : obj.m.tris) {

        triangle projected_tri;

        projected_tri.p[0] = multMatVec4x4(tri.p[0], viewMatrix);
        projected_tri.p[1] = multMatVec4x4(tri.p[1], viewMatrix);
        projected_tri.p[2] = multMatVec4x4(tri.p[2], viewMatrix);

        projected_tri.p[0] = multMatVec4x4(projected_tri.p[0], rt_renderer.proj_mat); 
        projected_tri.p[1] = multMatVec4x4(projected_tri.p[1], rt_renderer.proj_mat); 
        projected_tri.p[2] = multMatVec4x4(projected_tri.p[2], rt_renderer.proj_mat); 

        for(vec3d& v : projected_tri.p) {

            v.x += 1.0f;
            v.y += 1.0f;

            v.x *= 0.5f * (float)rt_renderer.getScreenWidth();
            v.y *= 0.5f * (float)rt_renderer.getScreenHeight();
        }

        SDL_RenderLine(renderer, projected_tri.p[0].x, projected_tri.p[0].y, projected_tri.p[1].x, projected_tri.p[1].y);
        SDL_RenderLine(renderer, projected_tri.p[1].x, projected_tri.p[1].y, projected_tri.p[2].x, projected_tri.p[2].y);
        // Third edge. Probably necessary, but meh.🤷
        SDL_RenderLine(renderer, projected_tri.p[2].x, projected_tri.p[2].y, projected_tri.p[0].x, projected_tri.p[0].y);

        // std::printf("Original Vertices: (%.2f, %.2f, %.2f)\n", tri.p[0], tri.p[1], tri.p[2]);
        // std::printf("Projected Vertices: (%.2f, %.2f, %.2f)\n", projected_tri.p[0], projected_tri.p[1], projected_tri.p[2]);
    }
}

inline void rotateObject(renderableObject& obj, vec3d angles, double deltaTime) {
    
    for(triangle& tri : obj.m.tris) {
        for(vec3d& v : tri.p) {
            v = rotate(v, angles, obj.center, deltaTime);
        }        
    }
}

inline void translateObject(renderableObject& obj, vec3d translation, double deltaTime) {
    
    for(triangle& tri : obj.m.tris) {
        for(vec3d& v : tri.p) {
            v = translate(v, translation, deltaTime);
        }        
    }
}

#endif
