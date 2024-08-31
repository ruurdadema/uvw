/*
 * Owllab License Agreement
 *
 * This software is provided by Owllab and may not be used, copied, modified,
 * merged, published, distributed, sublicensed, or sold without a valid and
 * explicit agreement with Owllab.
 *
 * Copyright (c) 2024 Owllab. All rights reserved.
 */

#include <uvw/loop.h>
#include <uvw/udp.h>

#include <cassert>
#include <iostream>

namespace {
constexpr uint16_t port = 5555;
auto message = "This is a message";
constexpr auto num_messages_to_send = 5'000'000;
auto num_messages_send = 0;
auto num_messages_recv = 0;
const auto address = "127.0.0.1";
}  // namespace

static auto custom_alloc_callback(const uvw::udp_handle &, std::size_t suggested) {
    return std::make_pair(new char[suggested], suggested);
}

int main() {
    const auto loop = uvw::loop::create();
    if (loop == nullptr) {
        return 1;
    }

    auto recv_socket = loop->resource<uvw::udp_handle>();
    auto send_socket = loop->resource<uvw::udp_handle>();

    recv_socket->on<uvw::udp_data_event>(
        [](uvw::udp_data_event& event, [[maybe_unused]] uvw::udp_handle& handle) {
            if (++num_messages_recv >= num_messages_to_send) {
                handle.close();
            }
        }
    );

    if (recv_socket->bind("0.0.0.0", port, uvw::udp_handle::udp_flags::REUSEADDR) != 0) {
        return 1;
    }

    recv_socket->recv<custom_alloc_callback>();

    const auto addr = uvw::details::ip_addr(address, port);

    send_socket->on<uvw::send_event>([&send_socket, addr](const uvw::send_event&, uvw::udp_handle& handle) {
        if (++num_messages_send < num_messages_to_send) {
            send_socket->send(addr, const_cast<char*>(message), strlen(message));
        }
    });

    send_socket->send(addr, const_cast<char*>(message), strlen(message));

    const auto ret = loop->run();

    assert(num_messages_send == num_messages_to_send + 1);
    assert(num_messages_recv == num_messages_to_send + 1);

    return ret;
}
