#include "thread.h"
#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace Sync;
bool flag = true;

//thread for the client so multiple clients can be active at once
class ClientThread : public Thread
{
private:
	//create socket object
    Socket& socket;
    	//create Byte Array object for message
    ByteArray data;
    	//create string object to store data as string
    std::string dString;
    
   
public:
    ClientThread(Socket& socket)
    : socket(socket)
    {}

    ~ClientThread()
    {
    
    }

    virtual long ThreadMain()
    {
    		//boolean variable for closing socket
    		bool init = false;
    		
    		try{//creating interger value for socket to keep track if the socket is valid or not(non-negative)
    			int result = socket.Open();
    		}
    		catch(...) {//check if server is running, set these values if server not runnning causing socket to close in main()
    			init = true;
    			flag = false;
    		}
    		
    		while(1){//break out of  loop if the server isnt runnign
        		if(init){
				break;
			}
			
		//getting characters of input and placing them as a string in dString
		std::getline(std::cin, dString);
		
		//setting the data ByteArray object with the inputted string as the parameter
		data = ByteArray(dString);
	
		//send data to the servers socket
		socket.Write(data);
		
		//if the string is the termination word make flag = false to stop the loop in the main method and break out of the current while loop
		if(dString=="done"){
			flag = false;
			break;
		}
		
		//recieve data back from the server
		socket.Read(data);
		
		//take the data from the servers response and place it in a string
		dString = data.ToString();
		
		//print out the response from the server
		std::cout<< "reponse: " << dString << std::endl;
		}
        
	return 1;
    }
};

int main(void)
{	
	// Welcome the user 
	std::cout << "SE3313 Lab 3 Client" << std::endl;
	
	// Create our socket
	Socket socket("127.0.0.1", 3000);
	
	//create client thread
	ClientThread clientThread(socket);
	
	//while flag is true continue looping
	while(flag)
	{
		sleep(1);//run this loop once a second
	}
	try {
		socket.Close();//check is flag is true and f it is close the socket
		std::cout<<"socket closed" << std::endl;
	}
	catch (...) {}
	return 0;
}
	

	

