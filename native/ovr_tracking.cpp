#include "ovr_tracking.hpp"

#include <vector>
#include <cmath>

#include <windows.h>

transform pose_to_transform(vr::TrackedDevicePose_t pose) {
    auto pose_matrix = pose.mDeviceToAbsoluteTracking.m;

    // get position
    vector_3 position {
            pose_matrix[0][3],
            pose_matrix[1][3],
            -pose_matrix[2][3]
    };

    // get rotation (quaternion)
    float rot_w = std::sqrt(1.0f + pose_matrix[0][0] + pose_matrix[1][1] + pose_matrix[2][2]) / 2.0f;
    float rot_4w = 4 * rot_w;
    quaternion rotation {
            -((pose_matrix[2][1] - pose_matrix[1][2]) / rot_4w),
            -((pose_matrix[0][2] - pose_matrix[2][0]) / rot_4w),
            ((pose_matrix[1][0] - pose_matrix[0][1]) / rot_4w),
            rot_w
    };

    return { position, rotation };
}

#pragma region API Functions

OVR_TRACKING_API [[maybe_unused]] vr::HmdError init_tracking() {
    vr::HmdError error;
    vr::VR_Init(&error, vr::VRApplication_Background);

    vr::VRCompositor()->SetTrackingSpace(vr::ETrackingUniverseOrigin::TrackingUniverseStanding);

    return error;
}

OVR_TRACKING_API [[maybe_unused]] vr::TrackedDeviceIndex_t *get_tracker_indices(size_t *size) {
    static vr::IVRSystem *vr_system = vr::VRSystem();

    std::vector<vr::TrackedDeviceIndex_t> tracker_indices {};

    for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
        if (vr_system->GetTrackedDeviceClass(i) == vr::TrackedDeviceClass_GenericTracker)
            tracker_indices.push_back(i);
    }

    size_t tracker_count = tracker_indices.size();

    if (tracker_count == 0) {
        return nullptr;
    }

    vr::TrackedDeviceIndex_t *indices_ptr = tracker_indices.data();
    void *indices_alloc = calloc(tracker_count, sizeof(vr::TrackedDeviceIndex_t)); // windows is probably clever enough to free the heap for me
    std::memcpy(indices_alloc, indices_ptr, tracker_count * sizeof(vr::TrackedDeviceIndex_t));

    *size = tracker_count;
    return reinterpret_cast<vr::TrackedDeviceIndex_t *>(indices_alloc);
}

OVR_TRACKING_API [[maybe_unused]] transform get_pose_for_tracker(vr::TrackedDeviceIndex_t index) {
    static vr::IVRCompositor *compositor = vr::VRCompositor();

    vr::TrackedDevicePose_t pose{};
    compositor->GetLastPoseForTrackedDeviceIndex(index, nullptr, &pose);

    return pose_to_transform(pose);
}

OVR_TRACKING_API [[maybe_unused]] char *get_tracker_serial(vr::TrackedDeviceIndex_t index) {
    static vr::IVRSystem *vr_system = vr::VRSystem();
    static char *pch_buffer = new char[vr::k_unMaxPropertyStringSize];

    (void)vr_system->GetStringTrackedDeviceProperty(index, vr::Prop_SerialNumber_String, pch_buffer, vr::k_unMaxPropertyStringSize);

    return pch_buffer;
}

#pragma endregion // API Functions