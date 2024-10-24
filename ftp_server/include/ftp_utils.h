#ifndef FTP_UTILS_H
#define FTP_UTILS_H

#include <string>

void upload_file(int client_socket, const std::string &filename);
void download_file(int client_socket, const std::string &filename);
void delete_file(const std::string &filename);
void list_files(int client_socket);
void search_file(int client_socket, const std::string &filename);

#endif // FTP_UTILS_H
