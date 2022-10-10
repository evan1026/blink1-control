#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <utility>
#include <optional>

namespace blink1_control {
    enum class SocketDomain {
        Local = AF_LOCAL,
        INet = AF_INET,
        INet6 = AF_INET6
    };

    enum class SocketType {
        Stream = SOCK_STREAM,
        Datagram = SOCK_DGRAM,
        SeqPacket = SOCK_SEQPACKET,
        Raw = SOCK_RAW,
        ReliableDatagram = SOCK_RDM
    };

    enum class SocketProtocol {
        IP = 0,
        ICMP = 1,
        TCP = 6,
        UDP = 17,
        RDP = 27,
        IPV6 = 41,
        IPV6_ICMP = 58
    };

    enum class SocketError {
        None,
        AccessDenied = EACCES,
        UnsupportedAddress = EAFNOSUPPORT,
        Invalid = EINVAL,
        ProcessFileDescriptorLimitReached = EMFILE,
        SystemFileDescriptorLimitReached = ENFILE,
        NoBuffersAvailable = ENOBUFS,
        NoMemoryAvailable = ENOMEM,
        UnsupportedProtocol = EPROTONOSUPPORT,
        Unimplemented = ENOSYS,
        NameTooLong = ENAMETOOLONG,
        AddressInUse = EADDRINUSE,
        BadFileDescriptor = EBADF,
        NotASocket = ENOTSOCK,
        AddressNotAvailable = EADDRNOTAVAIL,
        OutOfAddressSpace = EFAULT,
        SymbolicLinkLoop = ELOOP,
        DirectoryNotFound = ENOENT,
        NotADirectory = ENOTDIR,
        AddressInReadOnlyFilesystem = EROFS,
        UnsupportedOperation = EOPNOTSUPP,
        WouldBlockTryAgain = EWOULDBLOCK,
        ConnectionAborted = ECONNABORTED,
        Interrupted = EINTR,
        ProtocolError = EPROTO,
        SocketClosed
    };

    enum class SocketStatus {
        NotOpen,
        Unbound,
        Bound,
        Listening
    };

    class SocketConnection {

        int socketfd;
        std::string address;

        char* receiveBuffer;

        public:
            SocketConnection(int fd, std::string_view addr) :
                socketfd(fd), address(addr), receiveBuffer(new char[1024]) {}

            SocketConnection(const SocketConnection& other) = delete;
            SocketConnection& operator=(const SocketConnection& other) = delete;

            SocketConnection(SocketConnection&& other) {
                receiveBuffer = new char[1024];

                socketfd = other.socketfd;
                other.socketfd = -1;

                address = other.address;
                other.address = "";
            }

            SocketConnection& operator=(SocketConnection&& other) {
                socketfd = other.socketfd;
                other.socketfd = -1;

                address = other.address;
                other.address = "";

                return *this;
            }

            ~SocketConnection() {
                delete[] receiveBuffer;

                if (socketfd != -1) {
                    ::shutdown(socketfd, 2 /* Disallow further communication */);
                }
            }

            [[nodiscard]] std::string_view getAddress() { return address; }

            // TODO this should not be the real interface
            std::pair<SocketError, std::string> receive();

            SocketError setBlockingMode(bool blocking);
            SocketError setBlocking() { return setBlockingMode(true); }
            SocketError setNonBlocking() { return setBlockingMode(false); }
    };

    class Socket {
        int socketfd;
        std::string path;
        bool isBoundToFile;

        SocketStatus status;
        SocketError error;

        SocketDomain domain;
        SocketType type;
        SocketProtocol protocol;

        SocketError bindLocal(std::string_view address);
        SocketError bindINet(std::string_view address);
        SocketError bindINet6(std::string_view address);

        std::pair<SocketError, std::optional<SocketConnection>> acceptLocal();
        std::pair<SocketError, std::optional<SocketConnection>> acceptINet();
        std::pair<SocketError, std::optional<SocketConnection>> acceptINet6();

        public:
            Socket(SocketDomain domain, SocketType type, SocketProtocol protocol);

            Socket(const Socket& other) = delete;
            Socket(Socket&& other) = delete;
            Socket& operator=(const Socket& other) = delete;
            Socket& operator=(Socket&& other) = delete;

            ~Socket();

            [[nodiscard]] SocketStatus getStatus() const;

            [[nodiscard]] SocketError getError() const;

            [[nodiscard]] int getSocketFd() const;

            [[nodiscard]] std::string_view getPath() const;

            SocketError bind(std::string_view address);

            SocketError listen(int backlog = 50);

            std::pair<SocketError, std::optional<SocketConnection>> accept();

            SocketError setBlockingMode(bool blocking);
            SocketError setBlocking() { return setBlockingMode(true); }
            SocketError setNonBlocking() { return setBlockingMode(false); }
    };
}

std::ostream& operator<<(std::ostream& os, const blink1_control::SocketError& error);
std::ostream& operator<<(std::ostream& os, const blink1_control::SocketProtocol& protocol);
std::ostream& operator<<(std::ostream& os, const blink1_control::SocketType& type);
std::ostream& operator<<(std::ostream& os, const blink1_control::SocketDomain& domain);
std::ostream& operator<<(std::ostream& os, const blink1_control::SocketStatus& status);
