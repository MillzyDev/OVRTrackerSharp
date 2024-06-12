#include <cmath>
#include <vector>

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

/**
 * @brief Converts the pose matrix to a unity-style transform structure.
 * @param pose The pose to convert.
 * @return The converted transform.
 */
Transform poseToTransform(const vr::TrackedDevicePose_t &pose) {
    const auto poseMatrix = pose.mDeviceToAbsoluteTracking.m;

    const Vector3 position{
        poseMatrix[0][3],
        poseMatrix[1][3],
        -poseMatrix[2][3],
    };

    const float rotationW = std::sqrt(1.0f + poseMatrix[0][0] + poseMatrix[2][2]) / 2.0f;
    const float rotationW4 = 4 * rotationW;

    const Quaternion rotation{
        -((poseMatrix[2][1] - poseMatrix[1][2]) / rotationW4),
        -((poseMatrix[0][2] - poseMatrix[2][0]) / rotationW4),
        ((poseMatrix[1][0] - poseMatrix[0][1]) / rotationW4),
        rotationW,
    };

    return Transform{position, rotation};
}

/**
 * @brief Initialises the OVRTracking SteamVR application.
 * @return Any errors encountered during initialisation. Is 0 if successful.
 */
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

/**
 * @brief Gets all active generic tracker devices.
 * @param size A pointer to the variable that will hold the size of the returned array.
 * @return A pointer to the first element of an array of TrackedDeviceIndex_t
 */
OVR_TRACKING_API vr::TrackedDeviceIndex_t *getTrackerIndices(size_t *size) {
    static vr::IVRSystem *vrSystem = vr::VRSystem();

    std::vector<vr::TrackedDeviceIndex_t> trackerIndices {};

    for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
        if (vrSystem->GetTrackedDeviceClass(i) == vr::TrackedDeviceClass_GenericTracker)
            trackerIndices.push_back(i);
    }

    const size_t trackerCount = trackerIndices.size();
    if (!trackerCount) {
        return nullptr;
    }

    const vr::TrackedDeviceIndex_t *indicesPtr = trackerIndices.data();
    void *indices_alloc = calloc(trackerCount, sizeof(vr::TrackedDeviceIndex_t)); // TODO: This leaks memory.
    std::memcpy(indices_alloc, indicesPtr, trackerCount * sizeof(vr::TrackedDeviceIndex_t));

    *size = trackerCount;
    return static_cast<vr::TrackedDeviceIndex_t *>(indices_alloc);
}

/**
 * @brief Gets the transform for a tracked device.
 * @param index The index of the device to get the pose for.
 * @return The transform fetched.
 */
OVR_TRACKING_API Transform getPoseForTracker(vr::TrackedDeviceIndex_t index) {
    static vr::IVRCompositor *compositor = vr::VRCompositor();

    vr::TrackedDevicePose_t pose{};
    compositor->GetLastPoseForTrackedDeviceIndex(index, nullptr, &pose);

    return poseToTransform(pose);
}