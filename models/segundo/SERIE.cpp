#include <boost/format.hpp>
#include "SERIE.h"

SERIE::SERIE()
    : io(),
      port(io, "/dev/ttyACM0")

{
    port.set_option(boost::asio::serial_port_base::parity()); // default none
    port.set_option(boost::asio::serial_port_base::character_size(8));
    port.set_option(boost::asio::serial_port_base::stop_bits()); // default one
    port.set_option(boost::asio::serial_port_base::baud_rate(115200));
}

void SERIE::start()
{
    comm_thread = std::thread([this]()
                              { this->run(); });
}

void SERIE::stop()
{
    port.cancel();
    comm_thread.join();
}

void SERIE::run()

{
    read_some();
    io.run();
}

// para recibir datos por el puerto serie
void SERIE::read_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
    if (error)
    {
        std::cout << error.message() << std::endl;
    }
    else
    {

        for (size_t i = 0; i < bytes_transferred; i++)
        {

            ::protocol::packet_decoder::feed(rx_buffer[i]);
            ::protocol::packet_decoder::check_timeouts();
        }
        read_some();
    }
}

void SERIE::read_some()
{
    port.async_read_some(boost::asio::buffer(rx_buffer, BUFSIZE), boost::bind(&SERIE::read_handler, this,
                                                                              boost::asio::placeholders::error,
                                                                              boost::asio::placeholders::bytes_transferred));
}

void SERIE::write_some()
{
    port.async_write_some(boost::asio::buffer(tx_buffer, 12), boost::bind(&SERIE::write_handler, this,
                                                                          boost::asio::placeholders::error,
                                                                          boost::asio::placeholders::bytes_transferred));
}

void SERIE::write_handler(const boost::system::error_code &error, size_t bytes_transferred)
{
    if (error)
    {
        std::cout << error.message() << std::endl;
    }
    else
    {
        ::protocol::packet_encoder::send(0x4);
        ::protocol::packet_decoder::check_timeouts();
    }
}

void SERIE::send_impl(const uint8_t *buf, uint8_t n)
{

    for (size_t i = 0; i < n; i++)
    {
        tx_buffer[i] = *buf++;
    }
    get_payload_buffer()[5] = '0';
    get_payload_buffer()[6] = Message_send->data[0];
    get_payload_buffer()[7] = Message_send->data[1];
    get_payload_buffer()[8] = Message_send->dlc;
}

void SERIE::handle_packet(const uint8_t *payload, size_t n)
{

    switch (payload[0])
    {
    // Event 0: Message_received message.
    case '0':
    {

        Message_receive->id = payload[1];
        Message_receive->canid = (payload[2] << 24) | (payload[3] << 16) | (payload[4] << 8) | (payload[5] << 0);
        Message_receive->dlc = payload[6];
        Message_receive->data[8];

        for (size_t i = 0; i < 1; i++)
        {
            Message_receive->data[i] = payload[7 + i];
        }
    }
    break;

    default:
    {

        // error, unknown packet
        //   this->set_error(error_code::unknown_opcode);
        this->reset();
    }
    }
}