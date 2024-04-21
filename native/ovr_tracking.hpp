#pragma once

#define OVR_TRACKING_API extern "C" __declspec(dllexport)

typedef struct quaternion {
    [[maybe_unused]] float x, y, z, w;
} quaternion;

typedef struct vector_3 {
    [[maybe_unused]] float x, y, z;
} vector_3;

typedef struct transform {
    [[maybe_unused]] vector_3 position;
    [[maybe_unused]] quaternion rotation;
} transform;

typedef enum tracker_role {
    INVALID = 0,
    WAIST = 1,
    LEFT_FOOT = 2,
    RIGHT_FOOT = 3,
} tracker_role;
