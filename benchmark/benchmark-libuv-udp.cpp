/*
 * Owllab License Agreement
 *
 * This software is provided by Owllab and may not be used, copied, modified,
 * merged, published, distributed, sublicensed, or sold without a valid and
 * explicit agreement with Owllab.
 *
 * Copyright (c) 2024 Owllab. All rights reserved.
 */

#include <cassert>
#include <cstdint>
#include <thread>

#include "uv.h"

namespace {
constexpr uint16_t port = 5555;
auto message = "This is a message";
constexpr auto num_messages_to_send = 5'000'000;
auto num_messages_send = 0;
auto num_messages_recv = 0;
}  // namespace

static void close_cb(uv_handle_t* handle) {}

static void on_send(uv_udp_send_t* req, int status) {
    if (++num_messages_send < num_messages_to_send) {
        const uv_buf_t msg = uv_buf_init(const_cast<char*>(message), static_cast<unsigned int>(strlen(message)));
        uv_udp_send(req, req->handle, &msg, 1, reinterpret_cast<const sockaddr*>(&req->addr), on_send);
    }
}

static void on_read(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) {
    delete[] buf->base;
    if (++num_messages_recv >= num_messages_to_send) {
        uv_close(reinterpret_cast<uv_handle_t*>(handle), close_cb);
        return;
    }
}

static void alloc_cb(uv_handle_t* handle, const size_t suggested_size, uv_buf_t* buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

int main() {
    auto loop = std::make_unique<uv_loop_t>();
    uv_loop_init(loop.get());

    uv_udp_t send_socket;
    uv_udp_t recv_socket;

    uv_udp_init(loop.get(), &send_socket);
    uv_udp_init(loop.get(), &recv_socket);

    sockaddr_in send_addr {};
    sockaddr_in recv_addr {};

    uv_ip4_addr("127.0.0.1", port, &send_addr);
    uv_ip4_addr("0.0.0.0", port, &recv_addr);

    uv_udp_send_t send_req;
    uv_buf_t msg = uv_buf_init(const_cast<char*>(message), static_cast<unsigned int>(strlen(message)));
    uv_udp_send(&send_req, &send_socket, &msg, 1, reinterpret_cast<const sockaddr*>(&send_addr), on_send);

    uv_udp_bind(&recv_socket, reinterpret_cast<const struct sockaddr*>(&recv_addr), UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_cb, on_read);

    auto ret = uv_run(loop.get(), UV_RUN_DEFAULT);

    assert(num_messages_send == num_messages_to_send);
    assert(num_messages_recv == num_messages_to_send);

    return ret;
}
