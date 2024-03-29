#include "ovr_tracking.hpp"

#include "openvr/openvr.h"

#include <vector>
#include <cmath>

extern "C" [[maybe_unused]] vr::HmdError init_tracking() {
    vr::HmdError error;
    vr::VR_Init(&error, vr::VRApplication_Background);

    // if more than 99% of people can stand, im gonna assume more than 99% of people will stand
    vr::VRCompositor()->SetTrackingSpace(vr::ETrackingUniverseOrigin::TrackingUniverseStanding);

    return error;
}

extern "C" [[maybe_unused]] vr::TrackedDeviceIndex_t *get_generic_tracker_indices(size_t *size) {
    static vr::IVRSystem *vr_system = vr::VRSystem();

    std::vector<vr::TrackedDeviceIndex_t> tracker_indices {};

    for (vr::TrackedDeviceIndex_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
        if (vr_system->GetTrackedDeviceClass(i) == vr::TrackedDeviceClass_GenericTracker)
            tracker_indices.push_back(i);
    }

    // store tracker indices on the heap, since the vector gets popped from the stack
    size_t tracker_count = tracker_indices.size();
    vr::TrackedDeviceIndex_t *indices_ptr = tracker_indices.data();
    void *indices_alloc = calloc(tracker_count, sizeof(vr::TrackedDeviceIndex_t)); // windows is probably clever enough to free the heap for me
    std::memcpy(indices_alloc, indices_ptr, tracker_count * sizeof(vr::TrackedDeviceIndex_t));

    *size = tracker_count;
    return reinterpret_cast<vr::TrackedDeviceIndex_t *>(indices_alloc);
}

extern "C" [[maybe_unused]] transform get_pose_for_tracker(vr::TrackedDeviceIndex_t index) {
    static vr::IVRCompositor *compositor = vr::VRCompositor();

    vr::TrackedDevicePose_t pose{};
    compositor->GetLastPoseForTrackedDeviceIndex(index, nullptr, &pose);
    auto pose_matrix = pose.mDeviceToAbsoluteTracking.m;

    vector_3 position {
        pose_matrix[0][3],
        pose_matrix[1][3],
        -pose_matrix[2][3] // right hand coord moment
    };

    // this is straight up fucking sourcery who came up with this?
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

extern "C" [[maybe_unused]] tracker_role get_role_for_tracker(vr::TrackedDeviceIndex_t index) {
    static vr::IVRSystem *vr_system = vr::VRSystem();

    char *device_name_buf = nullptr;
    vr::ETrackedPropertyError error;
    unsigned int size = vr_system->GetStringTrackedDeviceProperty(index, vr::Prop_ManufacturerName_String, device_name_buf, vr::k_unMaxPropertyStringSize, &error);

    if (!size) {
        return tracker_role::INVALID;
    }

    std::string device_name(device_name_buf, size);

    if (device_name == "vive_tracker_waist") {
        return tracker_role::WAIST;
    }
    else if (device_name == "vive_tracker_left_foot") {
        return tracker_role::LEFT_FOOT;
    }
    else if (device_name == "vive_tracker_right_foot") {
        return tracker_role::RIGHT_FOOT;
    }

    return tracker_role::INVALID;
}