/**
 * @file SERIE.h
 * @author Ezequiel Acerbo (acerboezequiel@live.com)
 * @brief Clase para el control y monitoreo del puerto Serie.
 * @version 0.1
 * @date 2024-04-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#pragma once

#include <iostream>
#include <atomic>
#include <thread>
#include <fstream>
#include <array>

#include <unistd.h>

#include <boost/ref.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>

#include "protocol.h"
/**
 * @brief Clase para controlar la interfaz CAN implementada en STM32 a través de puerto serie.
 */
class SERIE : private protocol::packet_decoder,
              private protocol::packet_encoder
{
public:
    /**
     * @brief Estructura para recepcion y envio de mensajes.
     */
    struct Message
    {
        uint32_t originator_id; /**< Identificador del dispositivo: 0,1,... */
        uint32_t id;            /**< Por ahora un ùnico tipo de evento: recepción, pero se pueden agregar otros: status, error, etc.  */
        uint32_t canid;         /**< CANID (32bits para soportar standard y extended). */
        uint32_t dlc;           /**< Cantidad de bytes del mensaje (máximo 8 bytes). */
        uint8_t data[8];        /**< Datos del mensaje. */
    };
    
    /**
     * @brief Estructura para el envio de mensajes al puerto serie
     * 
     */
    Message *Message_send = new Message;
    /**
     * @brief Estructura para la recpcion de mensajes al puerto serie
     * 
     */
    Message *Message_receive = new Message;
    
    /**
     * @brief Buffer para los bytes de envio de mensajes
     * 
     */
    static constexpr size_t BUFSIZE = 64;

    /**
     * @brief Constructor por defecto del puerto SERIE.
     * 
     */
    SERIE();

    /**
     * @brief Iniciar envío y recepcion de mensajes.
     */
    void start();

    /**
     * @brief Para detener el puerto serie.
     */
    void stop();
    /**
     * @brief Para enviar datos al puerto serie
     * 
     */
    void write_some();
    /**
     * @brief Para recibir datos por el puerto Serie
     * 
     */
    void read_some();

    std::array<char, BUFSIZE> rx_buffer;
    std::array<char, BUFSIZE> tx_buffer;

private:
    boost::asio::io_service io;
    boost::asio::serial_port port;
    std::thread comm_thread;
    std::atomic<bool> keep_running;
 
    void read_handler(const boost::system::error_code &error, size_t bytes_transferred);
    void write_handler(const boost::system::error_code &error, size_t bytes_transferred);

    std::string response_get(std::size_t length);
    /**
     * @brief Para iniciar los servicio del puerto serie.
     * 
     */
    void run();
    
    
    /**
     * @brief Para la recepcion del paquete de mensajes del protocolo.
     * 
     * @param payload Puntero constante que almacena los datos recibidos.
     * @param n Cantidad de datos recibidos.
     */
    void handle_packet(const uint8_t *payload, size_t n);

        /**
     * @brief Para el envios del paquete de mensajes del protocolo.
     * 
     * @param buf Puntero constante que almacena los datos enviados
     * @param n Cantidad de datos enviados.
     */
    void send_impl(const uint8_t *buf, uint8_t n) override;
};