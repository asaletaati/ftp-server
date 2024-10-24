#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "ftp_utils.h"

namespace fs = std::filesystem;

void upload_file(int client_socket, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        file.write(buffer, bytes_received);
    }
    file.close();
    std::cout << "File uploaded: " << filename << std::endl;
}

void download_file(int client_socket, const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (file) {
        char buffer[1024];
        while (file.read(buffer, sizeof(buffer))) {
            send(client_socket, buffer, sizeof(buffer), 0);
        }
        file.read(buffer, sizeof(buffer));
        send(client_socket, buffer, file.gcount(), 0);
    } else {
        std::cerr << "File not found: " << filename << std::endl;
    }
}

void delete_file(const std::string &filename) {
    if (fs::remove(filename)) {
        std::cout << "File deleted: " << filename << std::endl;
    } else {
        std::cerr << "Error deleting file: " << filename << std::endl;
    }
}

void list_files(int client_socket) {
    for (const auto &entry : fs::directory_iterator(".")) {
        std::string filename = entry.path().filename().string();
        send(client_socket, filename.c_str(), filename.size(), 0);
        send(client_socket, "\n", 1, 0); // Newline for separation
    }
}

void search_file(int client_socket, const std::string &filename) {
    if (fs::exists(filename)) {
        send(client_socket, "File found: ", 12, 0);
        send(client_socket, filename.c_str(), filename.size(), 0);
        send(client_socket, "\n", 1, 0);
    } else {
        send(client_socket, "File not found\n", 16, 0);
    }
}

