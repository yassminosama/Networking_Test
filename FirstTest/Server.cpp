#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

void receiveFile(asio::ip::tcp::socket& socket, const string& file_path) {
    ofstream file(file_path, ios::binary);  
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }

    char buffer[1024];  
    asio::error_code error;
    size_t bytesRead = 0;

    while ((bytesRead = socket.read_some(asio::buffer(buffer), error)) > 0) {
        file.write(buffer, bytesRead); 
    }

    if (error) {
        cerr << "Error during file reception: " << error.message() << endl;
    }
    file.close();  

    cout << "File received successfully" << endl;

   
    ifstream received_file(file_path);  
    if (received_file.is_open()) {
        string line;
        while (getline(received_file, line)) {
            cout << line << endl;  
        }
        received_file.close();  
    }
    else {
        cerr << "Error opening the received file" << endl;
    }
}

int main() {
    try {
        asio::io_context io_context;
        asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 8080));

        cout << "Server is running. Waiting for a client to connect..." << endl;

        asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);  

        cout << "Client connected from: "
            << socket.remote_endpoint().address().to_string()
            << ":" << socket.remote_endpoint().port() << endl;



        string file_path = "received_file.csv";  
        receiveFile(socket, file_path);  
    }
    catch (exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}