#include <iostream>
#include <common.h>
#include <hotel_tcpclient.h>

using namespace std;

Messaging msg;
	
#define CLIENT_IP    "10.128.0.4"
#define CLIENT_PORT  "5000"

int main() 
{
	string ip = "10.128.0.4";
	int port = 5000;

	bool connected = false;
	int x = 0;

	while (!connected) 
	{
		cout<<"main: connecting to " << ip << endl;
		connected = TcpClient::connectTo(ip, port);

		if (connected) 
		{
			TcpClient::_msg = msg;
			cout << "Succesful Connection with Server\n";
		}
		else 
		{
			cout << "Failed to connect Server: " << "\n"
				<< "Please check if the server is open and listening\n\n";
			sleep(2);
			cout << "Retrying to connect...\n";
			x++;
			if (x == 2)
			{
				cout << "Exceeded max retries\n" << endl;
				exit(0);
			}
		}
	};

	msg.create_message(INIT);
	TcpClient::waitForReceiveThread();
	return 0;
}
