#pragma once

#include <catch.hpp>
#include <zmq.hpp>

#if defined(ZMQ_CPP11)
#include <array>

class loopback_ip4_binder
{
  public:
    loopback_ip4_binder(zmq::socket_t &socket) { bind(socket); }
    std::string endpoint() { return endpoint_; }

  private:
    // Helper function used in constructor
    // as Gtest allows ASSERT_* only in void returning functions
    // and constructor/destructor are not.
    void bind(zmq::socket_t &socket)
    {
        REQUIRE_NOTHROW(socket.bind("tcp://127.0.0.1:*"));
        std::array<char, 100> endpoint{};
        size_t endpoint_size = endpoint.size();
        REQUIRE_NOTHROW(
          socket.getsockopt(ZMQ_LAST_ENDPOINT, endpoint.data(), &endpoint_size));
        REQUIRE(endpoint_size < endpoint.size());
        endpoint_ = std::string{endpoint.data()};
    }
    std::string endpoint_;
};

struct common_server_client_setup
{
    common_server_client_setup(bool initialize = true)
    {
        if (initialize)
            init();
    }

    void init()
    {
        endpoint = loopback_ip4_binder{server}.endpoint();
        REQUIRE_NOTHROW(client.connect(endpoint));
    }

    zmq::context_t context;
    zmq::socket_t server{context, zmq::socket_type::pair};
    zmq::socket_t client{context, zmq::socket_type::pair};
    std::string endpoint;
};
#endif
