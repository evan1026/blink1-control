/**
 * @file NetworkManager.hpp
 * @brief Header file for blink1_control::network::NetworkManager.
 */

#pragma once

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#pragma GCC diagnostic ignored "-Wshadow"
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#pragma GCC diagnostic pop

namespace blink1_control::network {

    /**
     * This class creates and maintains network connections.
     */
    class NetworkManager {

        std::shared_ptr<boost::asio::io_context> ioc;
        boost::asio::local::stream_protocol::endpoint endpoint;
        boost::asio::local::stream_protocol::acceptor acceptor;
        std::unique_ptr<boost::thread> ioThread;

        void acceptHandler(const boost::system::error_code& error, boost::asio::local::stream_protocol::socket peer);

        void startAccept();

        public:

            /**
             * Constructor.
             *
             * @param socketPath The path to bind the socket to.
             */
            NetworkManager(std::string_view socketPath);

            /**
             * Destructor.
             */
            ~NetworkManager();

            NetworkManager(const NetworkManager& other) = delete;
            NetworkManager(NetworkManager&& other) = delete;
            NetworkManager& operator=(const NetworkManager& other) = delete;
            NetworkManager& operator=(NetworkManager&& other) = delete;

            /**
             * Starts the network I/O service.
             */
            void start();

            /**
             * Stops the network I/O service.
             */
            void stop();

    };

}
