#include <Application.h>
#include <Common.h>
#include <Config.h>
#include <Drivers/MotorDriver.h>
#include <Servers/MainServer.h>
#include <String.h>

#include <spdlog/spdlog.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

namespace Server {

// FIXME: This is very hackish. Find a better way to do this.
Driver::MotorDriver* motor_left = nullptr;
Driver::MotorDriver* motor_right = nullptr;

void Main::Run()
{
    if (!motor_left || !motor_right) {
        motor_left = new Driver::MotorDriver(
            Configuration::the().motors()->back_left[0],
            Configuration::the().motors()->back_left[1],
            Configuration::the().motors()->back_left[2],
            Configuration::the().motors()->back_left[3]);

        motor_right = new Driver::MotorDriver(
            Configuration::the().motors()->back_right[0],
            Configuration::the().motors()->back_right[1],
            Configuration::the().motors()->back_right[2],
            Configuration::the().motors()->back_right[3]);
    }

    int opt = true;
    int master_socket, addrlen, new_socket, client_socket[30],
        max_clients = 30, activity, i, valread, sd;
    int max_sd;
    struct sockaddr_in address;

    char buffer[1025];
    fd_set readfds;

    const char* message = "YROS Daemon\r\n";

    for (i = 0; i < max_clients; i++)
        client_socket[i] = 0;

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return;
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        return;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(Configuration::the().server()->port);

    if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return;
    }

    if (listen(master_socket, 3) < 0) {
        perror("listen");
        return;
    }

    spdlog::info("MainServer: Awaiting connections on port {}", Configuration::the().server()->port);

    addrlen = sizeof(address);

    m_running = true;
    for (;;) {
        FD_ZERO(&readfds);

        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (sd > 0)
                FD_SET(sd, &readfds);

            if (sd > max_sd)
                max_sd = sd;
        }

        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
            spdlog::error("MainServer: select error");

        if (FD_ISSET(master_socket, &readfds)) {
            if ((new_socket = accept(master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
                perror("accept");
                return;
            }

            spdlog::info("MainServer: New connection: {}:{}", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            if (send(new_socket, message, strlen(message), 0) != strlen(message)) {
                spdlog::warn("MainServer: Failed sending welcome message:");
                perror("send");
            }

            for (i = 0; i < max_clients; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }

        for (i = 0; i < max_clients; i++) {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds)) {
                if ((valread = read(sd, buffer, 1024)) == 0) {
                sock_disconnect:
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    spdlog::info("MainServer: Host disconnected: {}:{}", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                } else {
                    buffer[valread] = '\0';
                    string data(buffer);
                    auto response = ParseInstruction(data);
                    send(sd, response.response.data(), response.response.size(), 0);
                    if (response.action == ResponseAction::Close)
                        goto sock_disconnect;
                }
            }
        }
    }
}

void Main::Stop()
{
    m_running = false;
}

const string STATUS_OK = "OK\r\n";
const string STATUS_ERR = "ERR\r\n";

Main::Response Main::ParseInstruction(string instruction)
{
    auto lines = split(instruction, "\r\n");

    for (auto line : lines) {

        for (auto& c : line)
            c = toupper(c);

        if (line.empty())
            continue;

        auto tokens = split(line, " ");

        if (tokens.size() < 1)
            continue;

        if (tokens[0] == "PING") {
            return { ResponseAction::None, STATUS_OK };
        } else if (tokens[0] == "CLOSE" || tokens[0] == "EXIT") {
            return { ResponseAction::Close, STATUS_OK };
        } else if (tokens[0] == "MOTOR") {
            if (tokens.size() < 3)
                return { ResponseAction::None, STATUS_ERR };

            if (tokens[1] == "LEFT") {
                try {
                    motor_left->SetSpeed(stoi(tokens[2]) * (Configuration::the().motors()->left_inverted ? -1 : 1));
                } catch (...) {
                    return { ResponseAction::None, STATUS_ERR };
                }
            } else if (tokens[1] == "RIGHT") {
                try {
                    motor_right->SetSpeed(stoi(tokens[2]) * (Configuration::the().motors()->right_inverted ? -1 : 1));
                } catch (...) {
                    return { ResponseAction::None, STATUS_ERR };
                }
            } else {
                return { ResponseAction::None, STATUS_ERR };
            }

            return { ResponseAction::None, STATUS_OK };
        } else {
            return { ResponseAction::None, STATUS_ERR };
        }
    }

    return {
        ResponseAction::None,
        STATUS_OK
    };
}

}
