#include "tracker_transmitter.hpp"
#include "openvr.h"

tracker_transmitter::transmitter *tracker_transmitter::create_transmitter() {
    return new transmitter();
}

void tracker_transmitter::free_transmitter(tracker_transmitter::transmitter *transmitter) {
    free(transmitter);
}

void tracker_transmitter::set_targets(tracker_transmitter::transmitter *transmitter,
                                      tracker_transmitter::tracker_targets targets) {
    transmitter->target_mode = targets;
}

void tracker_transmitter::add_targets(tracker_transmitter::transmitter *transmitter,
                                      tracker_transmitter::tracker_targets targets) {
    transmitter->target_mode = static_cast<tracker_transmitter::tracker_targets>(
            static_cast<int>(transmitter->target_mode) | static_cast<int>(targets)
            );
}

void tracker_transmitter::set_socket_port(tracker_transmitter::transmitter *transmitter, uint16_t port) {
    transmitter->socket_port = port;
}

void tracker_transmitter::set_transmit_frequency(tracker_transmitter::transmitter *transmitter, uint8_t frequency) {
    transmitter->transmit_frequency = frequency;
}

void tracker_transmitter::begin_server(tracker_transmitter::transmitter *transmitter) {
    // TODO: start server
}

void tracker_transmitter::close_server(tracker_transmitter::transmitter *transmitter) {
    // TODO: stop server
}
