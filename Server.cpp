#include "thread.h"
#include "socketserver.h"
#include "socket.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <algorithm>
#include <bits/stdc++.h>

using namespace Sync;


// This thread handles each client connection
class SocketThread : public Thread
{
private:
    // Reference to our connected socket
    Socket& socket;
    // The data we are receiving
    ByteArray data;
public:
    SocketThread(Socket& socket)
    : socket(socket)
    {}

    ~SocketThread()
    {}

    Socket& GetSocket()
    {
        return socket;
    }

    virtual long ThreadMain()
    {
        while(1)
        {
            try
            {
                // Wait for data from client
                socket.Read(data);
                
                //make string object to hold data from array
                std::string str = data.ToString();
                
                if (str == "done"){
                    break;//break out of loop if input is done
                }
                
               int n = str.length();//reverse string algorithm
		for(int i = 0; i < n/2; i++){
		std::swap(str[i], str[n - i - 1]);
		}
		
		
                data = ByteArray(str);//make the data the reversed string
               
                // Send it back to client
                socket.Write(data);
                
            }
            catch (...)
            {}
        }
        return 0;
    }
};


// This thread handles the server operations
class ServerThread : public Thread
{
private:
	//creating variables for ue in the class
    SocketServer& server;
    
    std::vector<SocketThread*> socketThreadVector;
    //making vector for string operations
    
    //creating byte array
    ByteArray data;
    
   
public:
    ServerThread(SocketServer& server)
    : server(server)
    {}

    ~ServerThread()
    {
    	//cleanup algorithm
	for (int i = 0; i < socketThreadVector.size(); i++){
            Socket& socket = socketThreadVector[i]->GetSocket();
            socket.Close();
            }
    }

    virtual long ThreadMain()
    {
    	while(true){
    		
        	// Wait for a client socket connection
        	Socket* newConnection = new Socket(server.Accept());

        	// A reference to this pointer 
        	Socket& socketReference = *newConnection;
        	
        	//vector for multiple threads
        	socketThreadVector.push_back(new SocketThread(socketReference));
        		
        }
    }
};

int main(void)
{
    std::cout << "I am a server." << std::endl;
	
    // Create our server
    SocketServer server(3000);    

    // Need a thread to perform server operations
    ServerThread serverThread(server);
	
    // This will wait for input to shutdown the server
    FlexWait cinWaiter(1, stdin);
    cinWaiter.Wait();

    // Shut down and clean up the server
    server.Shutdown();

}


