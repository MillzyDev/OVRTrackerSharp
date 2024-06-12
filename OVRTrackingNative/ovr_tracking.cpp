#include <cmath>

#include "openvr.h"

#define OVR_TRACKING_API extern "C" __declspec(dllexport)

struct Vector3
{
    float x;
    float y;
    float z;
};

struct Quaternion
{
    float x;
    float y;
    float z;
    float w;
};

struct Transform
{
    Vector3 position;
    Quaternion rotation;
};

Transform poseToTransform(const vr::TrackedDevicePose_t &pose) {
    const auto poseMatrix = pose.mDeviceToAbsoluteTracking.m;

    Vector3 position{
        poseMatrix[0][3],
        poseMatrix[1][3],
        -poseMatrix[2][3],
    };

    float rotationW = std::sqrt(1.0f + poseMatrix[0][0] + poseMatrix[2][2]) / 2.0f;
    float rotationW4 = 4 * rotationW;

    Quaternion rotation{
        -((poseMatrix[2][1] - poseMatrix[1][2]) / rotationW4),
        -((poseMatrix[0][2] - poseMatrix[2][0]) / rotationW4),
        ((poseMatrix[1][0] - poseMatrix[0][1]) / rotationW4),
        rotationW,
    };

    return Transform{position, rotation};
}

OVR_TRACKING_API vr::HmdError initialiseTracking() {
    constexpr vr::EVRApplicationType kApplicationType = vr::VRApplication_Background;
    constexpr vr::ETrackingUniverseOrigin kTrackingUniverseOrigin = vr::ETrackingUniverseOrigin::TrackingUniverseStanding;
    
    vr::HmdError error;
    VR_Init(&error, kApplicationType);

    if (error) {
        return error;
    }

    vr::VRCompositor()->SetTrackingSpace(kTrackingUniverseOrigin);

    return error;
}