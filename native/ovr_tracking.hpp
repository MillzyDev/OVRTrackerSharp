#pragma once

#include "openvr/openvr.h"

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

extern "C" {
[[maybe_unused]] __declspec(dllexport) vr::HmdError init_tracking();
[[maybe_unused]] __declspec(dllexport) vr::TrackedDeviceIndex_t *get_generic_tracker_indices(size_t *size);
[[maybe_unused]] __declspec(dllexport) transform get_pose_for_tracker(vr::TrackedDeviceIndex_t index);
[[maybe_unused]] __declspec(dllexport) transform get_pose_for_hmd();
[[maybe_unused]] __declspec(dllexport) tracker_role get_role_for_tracker(vr::TrackedDeviceIndex_t index);
}