#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "ftp_utils.h"

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0'; // Null-terminate the string
        std::string command(buffer);
        
        if (command.substr(0, 6) == "UPLOAD") {
            upload_file(client_socket, command.substr(7));
        } else if (command.substr(0, 8) == "DOWNLOAD") {
            download_file(client_socket, command.substr(9));
        } else if (command.substr(0, 6) == "DELETE") {
            delete_file(command.substr(7));
        } else if (command == "LIST") {
            list_files(client_socket);
        } else if (command.substr(0, 6) == "SEARCH") {
            search_file(client_socket, command.substr(7));
        }
    }
    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Binding error." << std::endl;
        return 1;
    }

    listen(server_socket, 5);
    std::cout << "FTP Server running on port " << PORT << std::endl;

    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len)) >= 0) {
        handle_client(client_socket);
    }

    close(server_socket);
    return 0;
}
