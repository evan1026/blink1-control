#include <filesystem>
#include <type_traits>
#include <sys/un.h>
#include <fcntl.h>

#include "Socket.hpp"

template <typename E>
constexpr typename std::underlying_type<E>::type to_underlying(E e) noexcept {
    return static_cast<typename std::underlying_type<E>::type>(e);
}

#define UNIX_SOCKET_ADDR_MAX_LENGTH 108

namespace blink1_control {

    Socket::Socket(SocketDomain domain_, SocketType type_, SocketProtocol protocol_) :
        domain(domain_), type(type_), protocol(protocol_)
    {
        socketfd = socket(to_underlying(domain), to_underlying(type), to_underlying(protocol));

        if (socketfd == -1) {
            error = static_cast<SocketError>(errno);
            status = SocketStatus::NotOpen;
        } else {
            error = SocketError::None;
            status = SocketStatus::Unbound;
        }
    }

    Socket::~Socket() {
        if (isBoundToFile) {
            std::filesystem::remove(path);
        }
    }

    SocketStatus Socket::getStatus() const {
        return status;
    }

    SocketError Socket::getError() const {
        return error;
    }

    int Socket::getSocketFd() const {
        return socketfd;
    }

    SocketError Socket::bind(std::string_view address) {
        switch (domain) {
            case SocketDomain::Local:
                return bindLocal(address);
            case SocketDomain::INet:
                return bindINet(address);
            case SocketDomain::INet6:
                return bindINet6(address);
        }
    }

    SocketError Socket::bindLocal(std::string_view address) {
        if (address.length() >= UNIX_SOCKET_ADDR_MAX_LENGTH) {
            return SocketError::NameTooLong;
        }

        struct sockaddr_un socketAddressInfo;
        socketAddressInfo.sun_family = AF_UNIX;
        address.copy(socketAddressInfo.sun_path, UNIX_SOCKET_ADDR_MAX_LENGTH - 1);
        socketAddressInfo.sun_path[address.length()] = '\0';

        int result = ::bind(socketfd, (struct sockaddr*)&socketAddressInfo, sizeof(socketAddressInfo));

        if (result == -1) {
            return static_cast<SocketError>(errno);
        }

        isBoundToFile = true;
        path = address;
        status = SocketStatus::Bound;
        return SocketError::None;
    }

    SocketError Socket::bindINet(std::string_view address) {
        return SocketError::Unimplemented;
    }

    SocketError Socket::bindINet6(std::string_view address) {
        return SocketError::Unimplemented;
    }

    std::string_view Socket::getPath() const {
        return path;
    }

    SocketError Socket::listen(int backlog) {
        int result = ::listen(socketfd, backlog);

        if (result == -1) {
            return static_cast<SocketError>(errno);
        }

        status = SocketStatus::Listening;
        return SocketError::None;
    }

    std::pair<SocketError, std::optional<SocketConnection>> Socket::accept() {
        switch (domain) {
            case SocketDomain::Local:
                return acceptLocal();
            case SocketDomain::INet:
                return acceptINet();
            case SocketDomain::INet6:
                return acceptINet6();
        }
    }

    std::pair<SocketError, std::optional<SocketConnection>> Socket::acceptLocal() {
        struct sockaddr_un connectionInfo;
        socklen_t infoLength = sizeof(connectionInfo);
        int newSockFd = ::accept(socketfd, (struct sockaddr*)&connectionInfo, &infoLength);

        if (newSockFd == -1) {
            if (errno == EAGAIN) {
                errno = EWOULDBLOCK; // This is just so my functions only return one of the values
            }

            return std::make_pair(static_cast<SocketError>(errno), std::nullopt);
        }

        return std::make_pair(SocketError::None, SocketConnection(newSockFd, connectionInfo.sun_path));
    }


    std::pair<SocketError, std::optional<SocketConnection>> Socket::acceptINet() {
        return std::make_pair(SocketError::Unimplemented, std::nullopt);
    }

    std::pair<SocketError, std::optional<SocketConnection>> Socket::acceptINet6() {
        return std::make_pair(SocketError::Unimplemented, std::nullopt);
    }

    SocketError Socket::setBlockingMode(bool blocking) {
        auto currentFlags = fcntl(socketfd, F_GETFL, 0);

        if (blocking) {
            currentFlags &= ~O_NONBLOCK;
        } else {
            currentFlags |= O_NONBLOCK;
        }

        int status = fcntl(socketfd, F_SETFL, currentFlags);

        if (status == -1) {
            return static_cast<SocketError>(errno);
        }

        return SocketError::None;
    }

    SocketError SocketConnection::setBlockingMode(bool blocking) {
        auto currentFlags = fcntl(socketfd, F_GETFL, 0);

        if (blocking) {
            currentFlags &= ~O_NONBLOCK;
        } else {
            currentFlags |= O_NONBLOCK;
        }

        int status = fcntl(socketfd, F_SETFL, currentFlags);

        if (status == -1) {
            return static_cast<SocketError>(errno);
        }

        return SocketError::None;
    }

    std::pair<SocketError, std::string> SocketConnection::receive() {
        ssize_t receivedBytes = recv(socketfd, receiveBuffer, 1023, 0);

        if (receivedBytes > 0) {
            receiveBuffer[receivedBytes] = '\0';
            return std::make_pair(SocketError::None, receiveBuffer);
        } else if (receivedBytes == 0) {
            return std::make_pair(SocketError::SocketClosed, "");
        } else {
            return std::make_pair(static_cast<SocketError>(errno), "");
        }
    }
}

using blink1_control::SocketDomain;
using blink1_control::SocketType;
using blink1_control::SocketProtocol;
using blink1_control::SocketError;
using blink1_control::SocketStatus;

std::ostream& operator<<(std::ostream& os, const SocketStatus& status) {
    switch(status) {
        case SocketStatus::NotOpen:
            os << "NotOpen";
            break;
        case SocketStatus::Unbound:
            os << "Unbound";
            break;
        case SocketStatus::Bound:
            os << "Bound";
            break;
        case SocketStatus::Listening:
            os << "Listening";
            break;
        default:
            os << "Unknown Value (" << to_underlying(status) << ")";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SocketDomain& domain) {
    switch(domain) {
        case SocketDomain::Local:
            os << "Local";
            break;
        case SocketDomain::INet:
            os << "INet";
            break;
        case SocketDomain::INet6:
            os << "INet6";
            break;
        default:
            os << "Unknown Value (" << to_underlying(domain) << ")";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SocketType& type) {
    switch(type) {
        case SocketType::Stream:
            os << "Stream";
            break;
        case SocketType::Datagram:
            os << "Datagram";
            break;
        case SocketType::SeqPacket:
            os << "SeqPacket";
            break;
        case SocketType::Raw:
            os << "Raw";
            break;
        case SocketType::ReliableDatagram:
            os << "ReliableDatagram";
            break;
        default:
            os << "Unknown Value (" << to_underlying(type) << ")";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SocketProtocol& protocol) {
    switch(protocol) {
        case SocketProtocol::IP:
            os << "IP";
            break;
        case SocketProtocol::ICMP:
            os << "ICMP";
            break;
        case SocketProtocol::TCP:
            os << "TCP";
            break;
        case SocketProtocol::UDP:
            os << "UDP";
            break;
        case SocketProtocol::RDP:
            os << "RDP";
            break;
        case SocketProtocol::IPV6:
            os << "IPV6";
            break;
        case SocketProtocol::IPV6_ICMP:
            os << "IPV6_ICMP";
            break;
        default:
            os << "Unknown Value (" << to_underlying(protocol) << ")";
            break;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const SocketError& error) {
    switch(error) {
        case SocketError::None:
            os << "None";
            break;
        case SocketError::AccessDenied:
            os << "AccessDenied";
            break;
        case SocketError::UnsupportedAddress:
            os << "UnsupportedAddress";
            break;
        case SocketError::Invalid:
            os << "Invalid";
            break;
        case SocketError::ProcessFileDescriptorLimitReached:
            os << "ProcessFileDescriptorLimitReached";
            break;
        case SocketError::SystemFileDescriptorLimitReached:
            os << "SystemFileDescriptorLimitReached";
            break;
        case SocketError::NoBuffersAvailable:
            os << "NoBuffersAvailable";
            break;
        case SocketError::NoMemoryAvailable:
            os << "NoMemoryAvailable";
            break;
        case SocketError::UnsupportedProtocol:
            os << "UnsupportedProtocol";
            break;
        case SocketError::Unimplemented:
            os << "Unimplemented";
            break;
        case SocketError::NameTooLong:
            os << "NameTooLong";
            break;
        case SocketError::AddressInUse:
            os << "AddressInUse";
            break;
        case SocketError::BadFileDescriptor:
            os << "BadFileDescriptor";
            break;
        case SocketError::NotASocket:
            os << "NotASocket";
            break;
        case SocketError::AddressNotAvailable:
            os << "AddressNotAvailable";
            break;
        case SocketError::OutOfAddressSpace:
            os << "OutOfAddressSpace";
            break;
        case SocketError::SymbolicLinkLoop:
            os << "SymbolicLinkLoop";
            break;
        case SocketError::DirectoryNotFound:
            os << "DirectoryNotFound";
            break;
        case SocketError::NotADirectory:
            os << "NotADirectory";
            break;
        case SocketError::AddressInReadOnlyFilesystem:
            os << "AddressInReadOnlyFilesystem";
            break;
        case SocketError::UnsupportedOperation:
            os << "UnsupportedOperation";
            break;
        case SocketError::WouldBlockTryAgain:
            os << "WouldBlockTryAgain";
            break;
        case SocketError::ConnectionAborted:
            os << "ConnectionAborted";
            break;
        case SocketError::Interrupted:
            os << "Interrupted";
            break;
        case SocketError::ProtocolError:
            os << "ProtocolError";
            break;
        case SocketError::SocketClosed:
            os << "SocketClosed";
            break;
        default:
            os << "Unknown Value (" << to_underlying(error) << ")";
            break;
    }

    return os;
}
