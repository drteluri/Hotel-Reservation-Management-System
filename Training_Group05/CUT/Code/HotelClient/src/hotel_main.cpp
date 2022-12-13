#include <iostream>
#include <common.h>
#include <hotel_tcpclient.h>

Messaging msg;

#define LOGIN  1
#define GET_ROOM_DETAILS 2
#define RESERVE_ROOM  3


//Display menu list when client accesses server
void printMenu() 
{
	std::cout << "*******MENU******* \n" <<
		"1. Login \n" <<
		"2. Get Room Details   \n"  <<
		"3. Reserve Room    \n"  <<
		"4. Exit    \n";
}

int getMenuSelection()
{
	int selection = 0;
	std::cin >> selection;

	if (!std::cin) 
	{
		throw std::runtime_error("Invalid choice!!!");
	}

	std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n');
	return selection;
}

bool handleMenuSelection(int selection)
{
	static const int minSelection = LOGIN;
	static const int maxSelection = EXIT;

	if (selection < minSelection || selection > maxSelection)
	{
		std::cout << " invalid selection: " << selection <<
			". selection must be in between" << minSelection << " and " << maxSelection << "\n";
		return false;
	}

	string sendBuffer;

	switch (selection) 
	{
		case LOGIN :
			{ // send message to server
				msg.create_message(USER_NOT_PRESENT);
				break;
			}

		case GET_ROOM_DETAILS :
			{ 
				msg.create_message(ROOM_TYPE);
				break;
			}

		case RESERVE_ROOM : 
			{ 
				int choice = 0;
				while(1){
					std::cout<< "Do you want to proceed for reservation?\n";
					std::cout<< "1.Confirm\n";
					std::cout<< "2.Cancel\n";
					std::cin>>choice;
					if(choice == 1)
					{msg.create_message(CONFIRM_RESERVATION); break;}
					else if(choice == 2)
					{msg.create_message(CANCEL_RESERVATION);break;}
					else
						cout<< "OOPS SORRY!!! Invalid selection :(\n";
				}
				 
				break;
			}

		case EXIT : 
			{
				return true;
				break;
			}

		default   :  
			{
				std::cout << " OOPS SORRY!!! Invalid selection :(" << selection <<
					"Selection must be between " << minSelection << " and " << maxSelection << "\n";
			}
	}
	return false;
}

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
			std::cout << "Succesful Connection with Server\n";
		}
		else 
		{
			std::cout << "Failed to connect Server: " << "\n"
				<< "Please check if the server is open and listening\n\n";
			sleep(2);
			std::cout << "Retrying to connect...\n";
			x++;
			if (x == 2)
			{
				std::cout << "Exceeded max retries\n" << endl;
				exit(0);
			}
		}
	};

	msg.create_message(INIT);
	TcpClient::waitForReceiveThread();
	return 0;
}
