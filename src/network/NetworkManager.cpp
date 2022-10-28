#include "network/NetworkManager.hpp"

#include <iostream>
#include <filesystem>

namespace b = boost;
namespace ba = boost::asio;
namespace bal = boost::asio::local;

static void runIo(std::shared_ptr<ba::io_context> ioContext) {
    ba::io_service::work work(*ioContext);
    ioContext->run();
}

namespace blink1_control::network {

    NetworkManager::NetworkManager(std::string_view socketPath) :
        ioc(std::make_shared<ba::io_context>()),
        endpoint(socketPath),
        acceptor(*ioc, endpoint),
        ioThread(nullptr)
    {}

    NetworkManager::~NetworkManager() {
        stop();
        std::filesystem::remove(endpoint.path());
    }

    void NetworkManager::start() {
        ioc->restart();
        acceptor.listen();
        startAccept();
        ioThread = std::make_unique<boost::thread>(std::bind_front(&runIo, this->ioc));
    }

    void NetworkManager::stop() {
        acceptor.cancel();
        ioc->stop();
    }

    void NetworkManager::startAccept() {
        acceptor.async_accept(std::bind_front(&NetworkManager::acceptHandler, this));
    }

    void NetworkManager::acceptHandler(const b::system::error_code& error, bal::stream_protocol::socket peer) {
        if (error) {

            if (error.value() != b::system::errc::operation_canceled) {
                std::cout << "Error accepting connection: " << error.what() << "\n";
            }

            return;
        }

        std::cout << "Got connection\n";
        bal::stream_protocol::iostream clientIos(std::move(peer));
        std::string data;
        std::cout << "Outbound: \"Hello, World!\"\n";
        clientIos << "Hello, World!\n";
        std::getline(clientIos, data);
        std::cout << "Inbound:  " << data << "\n";
        std::cout << "Closing connection\n";

        startAccept();
    }

}
