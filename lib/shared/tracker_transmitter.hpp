#pragma once

#include <cstdint>

#ifdef TT_EXPORTS
#define TT_EXPORT __declspec(dllexport)
#else
#define TT_EXPORT
#endif

namespace tracker_transmitter {

    typedef struct tracker_position {
        float x, y, z;
    } tracker_position;

    typedef struct tracker_rotation {
        float w, x, y, z;
    } tracker_rotation;

    typedef enum tracker_type {
        UNKNOWN_TRACKER_TYPE = 0,
        CONTROLLER_LEFT = 1,
        CONTROLLER_RIGHT = 2,
        TRACKER_WAIST = 3, // 3 main tracker types are supported, since thats all we have to test with atm
        TRACKER_LEFT_LEG = 4,
        TRACKER_RIGHT_LEG = 5,
    } tracker_type;

    typedef enum tracker_targets {
        NO_TARGETS = 0,
        CONTROLLERS = 1, // mainly for testing purposes, since I don't own any body trackers
        TRACKERS = 2
    } tracker_targets;

    typedef struct tracker_info {
        tracker_type trackerType;
        tracker_position position;
        tracker_rotation rotation;
    } tracker_info;

    typedef struct transmitter {
        void *vrSystem;
        tracker_targets target_mode; // targets to track
        uint16_t socket_port; // port that the socket method will use
        uint8_t transmit_frequency; // frequency at which tracking data is updated, measured in Hz
        int32_t socket_descriptor;
    } transmitter;

    TT_EXPORT transmitter *create_transmitter();

    TT_EXPORT void free_transmitter(transmitter *transmitter);

    TT_EXPORT void set_targets(transmitter *transmitter, tracker_targets targets);

    TT_EXPORT void add_targets(transmitter *transmitter, tracker_targets targets);

    TT_EXPORT void set_socket_port(transmitter *transmitter, uint16_t port);

    TT_EXPORT void set_transmit_frequency(transmitter *transmitter, uint8_t frequency);

    TT_EXPORT void begin_server(transmitter *transmitter);

    TT_EXPORT void close_server(transmitter *transmitter);
}