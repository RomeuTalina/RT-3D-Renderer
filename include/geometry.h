#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cassert>
#include <cmath>
#include <iostream>
#include <ostream>

struct mat3x3 {
    float m[3][3];
};

struct mat4x4 {
    float m[4][4];     
};

struct vec3d {
    float x, y, z;   
};

inline vec3d normalize(vec3d input) {

    vec3d output {0.0f, 0.0f, 0.0f};
    float length = std::sqrt(input.x*input.x + input.y*input.y + input.z*input.z);

    if(length > 1e-6f){
        output.x = input.x/length;
        output.y = input.y/length;
        output.z = input.z/length;
    }

    return output;
}

inline vec3d operator+(vec3d vec1, vec3d vec2) {

    vec1.x = vec1.x + vec2.x; 
    vec1.y = vec1.y + vec2.y; 
    vec1.z = vec1.z + vec2.z; 

    return vec1;
}

inline vec3d operator-(vec3d vec1, vec3d vec2) {

    vec1.x = vec1.x - vec2.x; 
    vec1.y = vec1.y - vec2.y; 
    vec1.z = vec1.z - vec2.z; 

    return vec1;
}

inline vec3d operator*(vec3d vec, double mult) {

    vec.x *= mult;
    vec.y *= mult;
    vec.z *= mult;

    return vec;
}

inline vec3d operator*(vec3d vec1, vec3d vec2) {

    vec1.x *= vec2.x;
    vec1.y *= vec2.y;
    vec1.z *= vec2.z;

    return vec1;
}

inline std::ostream& operator<<(std::ostream& out, const vec3d in) {

    out << "{" << in.x << ", " << in.y << ", " << in.z << "}";

    return out;
}

struct vec2d {
    float x, y;
};

struct triangle {
    vec3d p[3];
};

inline vec3d multMatVec4x4(vec3d input, mat4x4 mat) {
    
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

inline mat4x4 multMatMat4x4(mat4x4 mat1, mat4x4 mat2) {
    
    mat4x4 outputMat {};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            outputMat.m[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                outputMat.m[i][j] += mat1.m[i][k] * mat2.m[k][j];
            }
        }
    }

    return outputMat;
}

inline vec3d translate(vec3d input, vec3d transVec, double deltaTime) {

    transVec = transVec * deltaTime;
    
    vec3d output;
    output.x = input.x + transVec.x;
    output.y = input.y + transVec.y;
    output.z = input.z + transVec.z;

    return output;
}

inline mat4x4 createTranslationMatrix(vec3d transVec) {

    mat4x4 translationMatrix = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        transVec.x, transVec.y, transVec.z, 1.0f
    };

    return translationMatrix;
}

inline mat4x4 createRotationMatrix(vec3d angles) {
    
    mat4x4 rotationX {
        1.0f,           0.0f,            0.0f,   0.0f,
        0.0f, cosf(angles.x), sinf(angles.x),   0.0f,
        0.0f, -sinf(angles.x),  cosf(angles.x),   0.0f,
        0.0f,           0.0f,            0.0f,   1.0f
    };

    // pitch
    mat4x4 rotationY {
        cosf(angles.y), 0.0f, -sinf(angles.y),   0.0f,
                   0.0f, 1.0f,           0.0f,   0.0f,
        sinf(angles.y), 0.0f, cosf(angles.y),   0.0f,
        0.0f,            0.0f,           0.0f,   1.0f
    };

    // yaw
    mat4x4 rotationZ {
        cosf(angles.z), sinf(angles.z),   0.0f,   0.0f,
        -sinf(angles.z),  cosf(angles.z),   0.0f,   0.0f,
                  0.0f,            0.0f,   1.0f,   0.0f,
                  0.0f,            0.0f,   0.0f,   1.0f
    };

    mat4x4 generalRotMat = multMatMat4x4(multMatMat4x4(rotationX, rotationY), rotationZ);

    return generalRotMat;
}

inline vec3d rotate(vec3d input, vec3d angles, vec3d pivot) {

    mat4x4 generalRotMat = createRotationMatrix(angles);

    mat4x4 transToOrigin {
        1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -pivot.x, -pivot.y, -pivot.z, 1
    };

    mat4x4 transToPivot {
        1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            pivot.x, pivot.y, pivot.z, 1
    };

    mat4x4 fullMat = multMatMat4x4(multMatMat4x4(transToOrigin, generalRotMat), transToPivot);

    vec3d outputVec = multMatVec4x4(input, fullMat);
    
    return outputVec;
}

inline vec3d rotate(vec3d input, vec3d angles, vec3d pivot, double deltaTime) {

    angles = angles * deltaTime;
    
    mat4x4 generalRotMat = createRotationMatrix(angles);

    mat4x4 transToOrigin {
        1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -pivot.x, -pivot.y, -pivot.z, 1
    };

    mat4x4 transToPivot {
        1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            pivot.x, pivot.y, pivot.z, 1
    };

    mat4x4 fullMat = multMatMat4x4(multMatMat4x4(transToOrigin, generalRotMat), transToPivot);

    vec3d outputVec = multMatVec4x4(input, fullMat);
    return outputVec;
}

#endif
