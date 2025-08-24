#ifndef RENDERER_H
#define RENDERER_H

#include <cassert>
#include <cmath>
#include <vector>

struct mat4x4 {
    float m[4][4];     
};

struct vec3d {
    float x, y, z;   
};

struct vec2d {
    float x, y;
};

struct triangle {
    vec3d p[3];
};

struct mesh {
    std::vector<triangle> tris;
};

inline vec3d multMatVec(vec3d input, mat4x4 mat) {
    
    vec3d output;
    
    output.x = input.x * mat.m[0][0] + input.y * mat.m[1][0] + input.z * mat.m[2][0] + mat.m[3][0];
    output.y = input.x * mat.m[0][1] + input.y * mat.m[1][1] + input.z * mat.m[2][1] + mat.m[3][1];
    output.z = input.x * mat.m[0][2] + input.y * mat.m[1][2] + input.z * mat.m[2][2] + mat.m[3][2];
    float w = input.x * mat.m[0][3] + input.y * mat.m[1][3] + input.z * mat.m[2][3] + mat.m[3][3] ;

    assert(w != 0);

    output.x /= w;
    output.y /= w;
    output.z /= w;

    return output;
}

class TestRenderer {
    
private:
    mesh m_cube {};
    int m_screen_width {};
    int m_screen_height {};

public:
    mat4x4 proj_mat {};

    TestRenderer (){};

    TestRenderer(int screen_width, int screen_height):
        m_cube {},
        m_screen_width {screen_width},
        m_screen_height {screen_height}
    {
        m_cube.tris = {

            // NORTH
            {0.0f, 0.0f, 4.0f,  0.0f, 1.0f, 4.0f,   1.0f, 1.0f, 4.0f},
            {0.0f, 0.0f, 4.0f,  1.0f, 1.0f, 4.0f,   1.0f, 0.0f, 4.0f},


            // EAST
            {1.0f, 0.0f, 3.0f,  1.0f, 1.0f, 3.0f,   1.0f, 1.0f, 4.0f},
            {1.0f, 0.0f, 3.0f,  1.0f, 1.0f, 4.0f,   1.0f, 0.0f, 4.0f},

            // WEST
            {0.0f, 0.0f, 4.0f,  0.0f, 1.0f, 4.0f,   0.0f, 1.0f, 3.0f},
            {0.0f, 0.0f, 4.0f,  0.0f, 1.0f, 3.0f,   0.0f, 0.0f, 3.0f},

            // TOP 
            {0.0f, 1.0f, 3.0f,  0.0f, 1.0f, 4.0f,   1.0f, 1.0f, 4.0f},
            {0.0f, 1.0f, 3.0f,  1.0f, 1.0f, 4.0f,   1.0f, 1.0f, 3.0f},

            // BOTTOM
            {0.0f, 0.0f, 3.0f,  0.0f, 0.0f, 4.0f,   1.0f, 0.0f, 4.0f},
            {0.0f, 0.0f, 3.0f,  1.0f, 0.0f, 4.0f,   1.0f, 0.0f, 3.0f},

            // SOUTH
            {0.0f, 0.0f, 3.0f,  0.0f, 1.0f, 3.0f,   1.0f, 1.0f, 3.0f},
            {0.0f, 0.0f, 3.0f,  1.0f, 1.0f, 3.0f,   1.0f, 0.0f, 3.0f},
        };

        float z_near = 0.1f;  
        float z_far = 1000.0f; 
        float fov = 90.0f;
        float aspect_ratio = (float)m_screen_height / (float)m_screen_width; 
        float f_scaling = 1.0f / (tanf(fov*0.5f / 180.0f * 3.14159f));
        float q_scaling = z_far / (z_far - z_near);

        proj_mat.m[0][0] = aspect_ratio * f_scaling;
        proj_mat.m[1][1] = f_scaling;
        proj_mat.m[2][2] = q_scaling;
        proj_mat.m[3][2] = -z_near * q_scaling;
        proj_mat.m[2][3] = 1.0f;
        // What??
        proj_mat.m[3][3] = 0.0f;

        assert(m_cube.tris.size() == 12);
        for(int i = 0; i < 3; i++){
            assert(proj_mat.m[i][i] != 0.0f);
        }
    }

    int getScreenWidth() const {return m_screen_width;};
    int getScreenHeight() const {return m_screen_height;};
    mesh getCube() const {return m_cube;};
};

inline vec3d transByVec(vec3d input, vec3d transVec) {
    
    vec3d output;
    output.x = input.x - transVec.x;
    output.y = input.y - transVec.y;
    output.z = input.z - transVec.z;

    return output;
}

#endif
