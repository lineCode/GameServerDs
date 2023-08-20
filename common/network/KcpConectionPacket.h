#ifndef MINIPROJECT_KCPPACKET_HPP
#define MINIPROJECT_KCPPACKET_HPP

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <stdio.h>

#define ASIO_KCP_CONNECT_PACKET "asio_kcp_connect_package get_conv"
#define ASIO_KCP_SEND_BACK_CONV_PACKET "asio_kcp_connect_back_package get_conv:"
#define ASIO_KCP_DISCONNECT_PACKET "asio_kcp_disconnect_package"

namespace MiniProject
{
    std::string making_connect_packet(void);

    bool is_connect_packet(const char *data, size_t len);

    bool is_send_back_conv_packet(const char *data, size_t len);

    std::string making_send_back_conv_packet(uint32_t conv);

    uint32_t grab_conv_from_send_back_conv_packet(const char *data, size_t len);

    std::string making_disconnect_packet(uint32_t conv);

    bool is_disconnect_packet(const char *data, size_t len);

    uint32_t grab_conv_from_disconnect_packet(const char *data, size_t len);

}

#endif