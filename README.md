# ftp-server
homework
The File Transfer Protocol (FTP) is a standard communication protocol used for
the transfer of computer files from a server to a client on a computer network.
FTP is built on a client–server model architecture. You have to write a C++
program for a FTP Server to do the following:
• Upload a file
• Download a file
• Delete a file
• Search for file
• List the files
• Data Encryption (Optional)
Use CMAKE to build your program.
Your program should compile in a Linux environment.
Push your codes in a GitHub repository.
You can use a FTP Client app like FileZilla or WinSCP to test your server.
You must send a PDF as your program's documentation containing a link to
your GitHub repository and screenshots from a FTP Client application showing
the testing process and results.
# solution
با از ماشین مجازی و سپس نصب لینوکس در ماشین مجازی، ترمینال را باز کردم.
دستورات زیر را به ترتیب برای نصب پکیج های خواسته شده وارد کردم
sudo apt update
sudo apt install g++ cmake libboost-all-dev
بعد یک دایرکتوری ساختم 
mkdir ftp_server
cd ftp_server
بعد یک ساب دایرکتوری ساختم
mkdir src include build
ادامه دارد...

