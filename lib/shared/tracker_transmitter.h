#pragma once

#ifdef TT_EXPORTS
#define TT_EXPORT
#else
#define TT_EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct tracker_position {
        float x, y, z;
    } tracker_position;

    typedef struct tracker_rotation {
        float w, x, y, z;
    } tracker_rotation;

    typedef struct tracker_transform {
        tracker_position position;
        tracker_rotation rotation;
    } tracker_transform;

#ifdef __cplusplus
}
#endif