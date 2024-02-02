// C++ program to show the example of server application in 
// socket programming 
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include<queue>

using namespace std; 

int main() 
{ 
    // creating socket 
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    int buffer_size;
    std::queue<char> queue;

    // specifying the address 
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // binding socket. 
    bind(serverSocket, (struct sockaddr*)&serverAddress, 
        sizeof(serverAddress)); 

    // listening to the assigned socket 
    listen(serverSocket, 5); 

    // accepting connection request 
    while (true){
        int clientSocket 
            = accept(serverSocket, nullptr, nullptr); 

        cout << "Message from client: ";
        char buffer[1024] = { 0 }; 
        while (true){
            buffer_size = recv(clientSocket, buffer, sizeof(buffer), 0); 
            for (int i = 0; i < buffer_size; i++){
                queue.push(buffer[i]);
            }
            if (buffer_size <= 0) break;
        }
        while (!queue.empty()){
            cout << queue.front();
            queue.pop();
        }
        cout << endl;
        close(clientSocket);
    }
    // closing the socket. 
    close(serverSocket); 

    return 0; 
}