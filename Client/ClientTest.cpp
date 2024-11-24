#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void sendFile(const string& file_path, asio::ip::tcp::socket& socket) {
    ifstream file(file_path, ios::binary); 
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return;
    }

    
    char buffer[1024];  
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));  
        size_t bytesRead = file.gcount();  
        asio::write(socket, asio::buffer(buffer, bytesRead));  
    }
    file.close();  
    cout << "File sent successfully!" << endl;
}

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::socket socket(io_context);

        asio::connect(socket, resolver.resolve("127.0.0.1", "8080"));

        cout << "Connected to the server" << endl;

        string file_path;
        cout << "Enter the file path: ";
        getline(cin, file_path);

        sendFile(file_path, socket);

    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}
