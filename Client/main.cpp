#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
using namespace std;
#pragma warning(disable: 4996)

void recv(SOCKET* sock) {
	char file_size_str[16];
	char file_name[32];

	recv(*sock, file_size_str, 16, 0);
	int file_size = atoi(file_size_str);
	char* bytes = new char[file_size];

	recv(*sock, file_name, 32, 0);

	fstream file;
	file.open(file_name, ios_base::out | ios_base::binary);

	cout << "size: " << file_size << endl;
	cout << "name: " << file_name << endl;

	if (file.is_open()) {
		recv(*sock, bytes, file_size, 0);

		file.write(bytes, file_size);
		cout << "ok save\n";
	}
	else 
		cout << "Error file open" << endl;

	delete[] bytes;
	file.close();
}


int main(int argc, char* argv[]) {
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error" << std::endl;
		exit(1);
	}

	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
		cout << "Error: failed connect to server.\n";
		return 1;
	}
	cout << "Connected!\n";
	recv(&Connection);

	system("pause");
	return 0;
}