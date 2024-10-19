#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>
#include <vector>

#define PORT 8080           // پورت سرور
#define BUFFER_SIZE 1024    // اندازه بافر برای دریافت و ارسال داده‌ها

// تابع برای آپلود فایل
void uploadFile(int newSocket) {
    char buffer[BUFFER_SIZE];
    std::ofstream outFile("uploaded_file.txt", std::ios::binary);  // ایجاد فایل برای نوشتن

    if (!outFile) {
        std::cerr << "Error opening file for writing." << std::endl;
        return;
    }

    int bytesReceived;
    // دریافت داده‌ها از کلاینت و نوشتن در فایل
    while ((bytesReceived = recv(newSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        outFile.write(buffer, bytesReceived);
    }

    outFile.close();
    std::cout << "File uploaded successfully!" << std::endl;
}

// تابع برای دانلود فایل
void downloadFile(int newSocket) {
    char buffer[BUFFER_SIZE];
    std::ifstream inFile("uploaded_file.txt", std::ios::binary);  // باز کردن فایل برای خواندن

    if (!inFile) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
    }

    // ارسال محتوای فایل به کلاینت
    while (inFile.read(buffer, BUFFER_SIZE)) {
        send(newSocket, buffer, inFile.gcount(), 0);
    }
    
    inFile.clear();
    inFile.seekg(0, std::ios::end);
    std::streamsize size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    send(newSocket, buffer, size, 0);
    
    inFile.close();
    std::cout << "File downloaded successfully!" << std::endl;
}

// تابع برای حذف فایل
void deleteFile(int newSocket) {
    const char* filename = "uploaded_file.txt";  // نام فایلی که باید حذف شود
    if (std::remove(filename) == 0) {
        std::cout << "File deleted successfully!" << std::endl;
    } else {
        std::cerr << "Error deleting file!" << std::endl;
    }
}

// تابع برای جستجوی فایل
void searchFile(int newSocket) {
    const char* filename = "uploaded_file.txt";
    if (std::filesystem::exists(filename)) {  // بررسی وجود فایل
        const char* message = "File found!";
        send(newSocket, message, strlen(message), 0);
        std::cout << "File found!" << std::endl;
    } else {
        const char* message = "File not found.";
        send(newSocket, message, strlen(message), 0);
        std::cout << "File not found!" << std::endl;
    }
}

// تابع برای لیست کردن فایل‌ها
void listFiles(int newSocket) {
    std::string fileList;
    // لیست کردن فایل‌های موجود در دایرکتوری
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        fileList += entry.path().filename().string() + "\n";  // اضافه کردن نام فایل به لیست
    }
    
    send(newSocket, fileList.c_str(), fileList.size(), 0);  // ارسال لیست فایل‌ها به کلاینت
    std::cout << "File list sent!" << std::endl;
}

int main() {
    int server_fd, newSocket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    // ایجاد سوکت
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // تنظیم گزینه‌های سوکت
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // تنظیم آدرس و پورت
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // بایند کردن سوکت
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // شروع به گوش دادن
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // حلقه اصلی برای قبول اتصالات ورودی
    while ((newSocket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) >= 0) {
        std::cout << "Connection accepted!" << std::endl;

        char request[BUFFER_SIZE];
        recv(newSocket, request, BUFFER_SIZE, 0);  // دریافت درخواست از کلاینت

        // مدیریت درخواست‌های مختلف
        if (strcmp(request, "upload") == 0) {
            uploadFile(newSocket);
        } else if (strcmp(request, "download") == 0) {
            downloadFile(newSocket);
        } else if (strcmp(request, "delete") == 0) {
            deleteFile(newSocket);
        } else if (strcmp(request, "search") == 0) {
            searchFile(newSocket);
        } else if (strcmp(request, "list") == 0) {
            listFiles(newSocket);
        } else {
            std::cerr << "Invalid request!" << std::endl;
        }

        close(newSocket);  // بستن سوکت کلاینت
    }

    if (newSocket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    close(server_fd);  // بستن سوکت سرور
    return 0;
}
