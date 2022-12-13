#include <cstdint>
#include <common.h>
#include <bits/stdc++.h>
#include <sys/select.h>
#include <hotel_tcpclient.h>

using namespace std;

#define SELECT_FAILED -1
#define SELECT_TIMEOUT 0

#define AC_ROOM 1
#define NON_AC_ROOM 2
#define DELUXE_SUITE 3
#define EXECUTIVE_SUITE 4

namespace fd_wait {
	// monitor file descriptor and wait for I/O operation
	Result waitFor(int& fileDescriptor, uint32_t timeoutSeconds) {
		struct timeval tv;
		tv.tv_sec = timeoutSeconds;
		tv.tv_usec = 0;
		fd_set fds;

		FD_ZERO(&fds);
		FD_SET(fileDescriptor, &fds);
		const int selectRet = select(fileDescriptor + 1, &fds, nullptr, nullptr, &tv);

		if (selectRet == SELECT_FAILED) {
			return Result::FAILURE;
		}
		else if (selectRet == SELECT_TIMEOUT) {
			return Result::TIMEOUT;
		}
		return Result::SUCCESS;

	}
}

//initialising the map elements to 0

void Messaging::initialize_messages()
{
	messages_t m;

	msg_map.insert(pair<msg_types_e, messages_t>(UNAME, { UNAME,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(PASSWD, { PASSWD,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(AUTH_SUCCESS, { AUTH_SUCCESS,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(AUTH_FAIL, { AUTH_FAIL,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(USER_NOT_PRESENT, { USER_NOT_PRESENT,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOM_TYPE, { ROOM_TYPE,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOM_NOTAVAILABLE, { ROOM_NOTAVAILABLE,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOM_RESERV_SUCCESS, { ROOM_RESERV_SUCCESS,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOM_RESERV_FAIL, { ROOM_RESERV_FAIL,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOM_STATS, { ROOM_STATS,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(CONFIRM_RESERVATION, { CONFIRM_RESERVATION,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(CANCEL_RESERVATION, { CANCEL_RESERVATION,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(NEE_PASSWD, { NEE_PASSWD,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(INIT, { INIT,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(EXIT, { EXIT,0,"" }));
	msg_map.insert(pair<msg_types_e, messages_t>(ROOMNO, { ROOMNO,0,"" }));

	map<msg_types_e, messages_t>::iterator itr;

	cout << "\nThe map msgs is : \n";
	cout << '\t' << "KEY" << '\t' << "TYPE" << '\t' << "LENGTH" << '\t' << "VALUE" << endl;
	for (itr = msg_map.begin(); itr != msg_map.end(); ++itr)
	{
		cout << '\t' << itr->first << '\t' << itr->second.msg_type << '\t' << itr->second.msg_len << '\t' << itr->second.msg_val
			<< '\n';
	}
	cout << endl;
}

//displaying room menu 
void Messaging::select_room_type(){
	cout << "*******MENU******** \n" <<
		"1. Get Room Details   \n"  <<
		"0. Exit    \n";
}

//creating message to send to server
string Messaging::create_message(msg_types_e option, string data)
{
	char sendBuffer[64];
	memset(sendBuffer, '0', sizeof(sendBuffer));
	if (msg_map.find(option) != msg_map.end())
	{
		
		{
			msg_map[option].msg_val = data;
			msg_map[option].msg_len = data.length();

			snprintf(sendBuffer, sizeof(sendBuffer), "%d-%ld-%s\r\n", (int)msg_map[option].msg_type, data.length(), data.c_str());
			
		}
	}
	cout << "message created is " << sendBuffer << endl;
	return sendBuffer;
}

void Messaging::create_message(msg_types_e option)
{
	char sendBuffer[64];
	int choice, n;
	string room_type;


	memset(sendBuffer, '0', sizeof(sendBuffer));
	while(1){
		if (msg_map.find(option) != msg_map.end())
		{
			if(option == INIT)
			{
				cout << "======== WELCOME TO HOTEL BOOKING ========\n";
				option = UNAME;
			}
			else if(option == EXIT)
			{
				cout<<"BYE...\n";
				TcpClient::close();
				return;
			}
			else if(option == USER_NOT_PRESENT || option == UNAME)
			{

				string uname, passwd; 
				cout << "Enter user name: ";
				cin >> uname;
				string sendMsg = create_message(UNAME, uname);
				TcpClient::sendMsg(sendMsg.c_str(),sizeof(sendMsg.c_str()));
				return;
			}
			else if(option == NEE_PASSWD)
			{
				string passwd; 
				cout << "Enter passwd: ";
				cin >> passwd;
				string sendMsg = create_message(PASSWD, passwd);
				TcpClient::sendMsg(sendMsg.c_str(),sizeof(sendMsg.c_str()));
				return;
			}
			else if(option == AUTH_FAIL)
			{
				option = USER_NOT_PRESENT;
			}
			else if(option == AUTH_SUCCESS)
			{
				msg_map[option].msg_val = "1";
				msg_map[option].msg_len = 1;
				cout << "Welcome "<< msg_map[UNAME].msg_val<<endl;
				cout << "*******MENU******* \n" <<
					"1. Book a Room   \n"  <<
					"2. Cancel a Room   \n"  <<
					"0. Exit    \n";
				cin >> choice;
				if(choice == 1)
				{
					cout << "PLEASE SELECT ROOM TYPE FROM BELOW\n";
					cout << "  Type   \t\t\t   price\n";
					cout << "1.Ac Room   \t\t\t   3000\n";
					cout << "2.Non-Ac Room \t\t\t  1000\n";
					cout << "3.Deluxe Suite \t\t\t   5000\n";
					cout << "4.Executive Suite \t\t6000\n";
					cin >> choice;
					cout << "Please enter no.of days you want stay\n";
					cin >> n;
					switch (choice)
					{
						case AC_ROOM: room_type = "AC";
							      break;
						case NON_AC_ROOM: room_type = "Non AC";
								  break;
						case DELUXE_SUITE: room_type = "Deluxe Suite";
								   break;
						case EXECUTIVE_SUITE: room_type = "Executive Suite";
								      break;
						default: cout << "Enter valid Input!!! \n";
							 break;
					}
					stringstream ss;
					ss << room_type <<":" << n;

					string s = ss.str();
					string sendMsg = create_message(ROOM_TYPE, s);
					TcpClient::sendMsg(sendMsg.c_str(),sendMsg.length());
					return;
				}
				else if(choice == 2)
				{
					string roomid;
					cout << "Please enter a room to cancel: ";
					cin >> roomid;
					string sendMsg = create_message(CANCEL_RESERVATION, roomid);
					TcpClient::sendMsg(sendMsg.c_str(),sizeof(sendMsg.c_str()));
					return;
				}
				else if(choice == 0)
				{
					option = EXIT;
				}
			}
			else if (option == ROOM_STATS)
			{
				cout<< "Do you want to proceed for reservation?\n";
				cout<< "1.yes\n";
				cout<< "2.no\n";
				cout<< "0.Exit\n";
				cin >> choice;
				if(choice == 1)
				{
					string sendMsg = create_message(CONFIRM_RESERVATION, "1");
					TcpClient::sendMsg(sendMsg.c_str(),sizeof(sendMsg.c_str()));
					return;
				}else if(choice == 2)
				{
					option = AUTH_SUCCESS;
				}else if(choice == 0)
				{
					option = EXIT;
				}
			}
			else if (option == ROOM_RESERV_SUCCESS)
			{
				option = AUTH_SUCCESS;
			}
			else if (option == ROOM_RESERV_FAIL || option == ROOM_NOTAVAILABLE)
			{
				cout << "Sorry! room not available" << endl;
				option = AUTH_SUCCESS;
			}
			else{
				cout << "Invalid option: "<< option << endl;
				if(msg_map[AUTH_SUCCESS].msg_val == "1")
				{
					option = AUTH_SUCCESS;
				}else
				{
					option = USER_NOT_PRESENT;
				}
			}
		}
		else
		{
			cout<<"somthing wrong!!!..."<<endl;
			cout<< "Do you want proceed?\n";
			cout<< "1.yes\n";
			cout<< "0.Exit\n";
			cin >> choice;
			if(choice == 1)
			{
				option = USER_NOT_PRESENT;
			}
			else
			{
				return;
			}
		}
	}

	cout << "message created is " << sendBuffer << endl;
}





