#include <cstdint>
#include <common.h>
#include <bits/stdc++.h>
#include <sys/select.h>
#include <vector>
#include <string>
#include <algorithm>
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

string Messaging::process_ClientMessage(string message){
	string responseString;
	char sendMsg[64];
	
	message.erase( std::remove(message.begin(), message.end(), '\r'), message.end() );
  	message.erase( std::remove(message.begin(), message.end(), '\n'), message.end() );

	vector<string> words;
	string word;
	stringstream ss(message);
	cout<<"process_ClientMessage entered "<< endl;
	memset(sendMsg, '0', sizeof(sendMsg));
	while (getline(ss, word, '-')){
		words.push_back(word);
	}
  	msg_types_e option = (msg_types_e) atoi(words[0].c_str());
  	string data = words[2];
	cout<<option<<"\t\t"<<words[1]<<"\t\t"<<data<<endl;

	if (msg_map.find(option) == msg_map.end())
	{
		cout<< "Unable to process client message!!!"<<endl;
		return "";
	
	}
	msg_map[option].msg_type = option;
	msg_map[option].msg_val = data;
	msg_map[option].msg_len = data.length();
	
	if(option == UNAME){
		//check user exists
		
		if(hotel_users::isuserExists(data)){
			snprintf(sendMsg, sizeof(sendMsg), "%d\r\n", (int)NEE_PASSWD);
		}else{
			responseString = "User not found.";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)USER_NOT_PRESENT, responseString.length(), responseString.c_str());
		}
	}else if(option == PASSWD){
		//check user exists
		if(hotel_users::authenticate(msg_map[UNAME].msg_val, data)){
			snprintf(sendMsg, sizeof(sendMsg), "%d\r\n", (int)AUTH_SUCCESS);
		}else{
			responseString = "Login failed...";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)AUTH_FAIL, responseString.length(), responseString.c_str());
		}
	} else if(option == ROOM_TYPE){
		//check user exists
		cout<<"processing room type\n";
		string word1;
		stringstream ss1(data);
		vector<string> words1;
		while (getline(ss1, word1, ':')){
			words1.push_back(word1);
			cout<<"segments: "<<word1<<endl;
		}
		string roomType = words1[0];
		int noOfDays = atoi(words1[1].c_str());
		cout<<"room type: " << roomType<< endl;
		cout<< "no of days: "<< noOfDays << endl;
		hotel_room r = hotel_rooms::getAvailableRoom(roomType);

		if(r.getRoomNo() < 0){
			responseString = "Room not available";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)ROOM_NOTAVAILABLE, responseString.length(), responseString.c_str());
		}else{
			stringstream ss;
			string sRoomNo;
			ss << r.getRoomNo();
			sRoomNo = ss.str();
			msg_map[ROOMNO].msg_val = sRoomNo;
            ss << "Room type: " << r.getRoomType() << " Room NO: " << r.getRoomNo() << " Price: " << r.getPrice() * noOfDays;
            responseString = ss.str();
			snprintf(sendMsg, sizeof(sendMsg), "%d-%ld-%s\r\n", (int)ROOM_STATS,responseString.length(),responseString.c_str());
		}
	} else if(option == CONFIRM_RESERVATION){
		cout<< "reservation processing...\n";
		int roomid = atoi(msg_map[ROOMNO].msg_val.c_str());
		if(hotel_rooms::reserveRoom(roomid)){
			responseString = "Resevation succeeded..";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)ROOM_RESERV_SUCCESS, responseString.length(), responseString.c_str());
		}else{
			responseString = "Resevation failed..";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)ROOM_RESERV_FAIL, responseString.length(), responseString.c_str());
		}
	} else if(option ==CANCEL_RESERVATION){
		int roomid = atoi(data.c_str());
		if(hotel_rooms::unreserveRoom(roomid)){
			responseString = "Resevation cancelled..";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)ROOM_RESERV_SUCCESS, responseString.length(), responseString.c_str());
		}else{
			responseString = "Resevation cancelled failed..";
			snprintf(sendMsg, sizeof(sendMsg),"%d-%ld-%s\r\n", (int)ROOM_RESERV_FAIL, responseString.length(), responseString.c_str());
		}
	}
	else {
		cout<<"invalid type: "<<msg_map[option].msg_type<<endl;
	}
	cout<<"message prepared: "<<sendMsg<<endl;
	cout<<"process_ClientMessage end: "<< endl;
	return sendMsg;
}
void Messaging::create_message(msg_types_e option, string data)
{
	char sendBuffer[64];
	memset(sendBuffer, '0', sizeof(sendBuffer));
	if (msg_map.find(option) != msg_map.end())
	{
		if ((option == UNAME) || (option == PASSWD))
		{
			msg_map[option].msg_val = data;
			msg_map[option].msg_len = data.length();
			snprintf(sendBuffer, sizeof(sendBuffer), "%d-%d-%s\r\n", msg_map[option].msg_type,
				msg_map[option].msg_len, msg_map[option].msg_val.c_str());
		}
	}
	cout << "message created is " << sendBuffer << endl;
}

void Messaging::create_message(msg_types_e option)
{
	char sendBuffer[64];
	int choice, n;
	string room_type;

	memset(sendBuffer, '0', sizeof(sendBuffer));

	if (msg_map.find(option) != msg_map.end())
	{
		if (option == ROOM_TYPE)
		{
			cout << "PLEASE SELECT ROOM TYPE FROM BELOW\n";
			cout << "\t\t Type   \t\t   price\n";
			cout << "\t\t1.Ac Room   \t\t   3000\n";
			cout << "\t\t2.Non-Ac Room \t\t 1000\n";
			cout << "\t\t3.Deluxe Suite \t\t   5000\n";
			cout << "\t\t4.Executive Suite \t\t6000\n";
			cin >> choice;
			cout << "Please enter no.of days you want stay\n";
			cin >> n;
			switch (choice)
			{
			case AC_ROOM: room_type = "AC";
				break;
			case NON_AC_ROOM: room_type = "Non-AC";
				break;
			case DELUXE_SUITE: room_type = "Deluxe Suite";
				break;
			case EXECUTIVE_SUITE: room_type = "Executive Suite";
				break;
			default: cout << "Enter valid Input!!! \n";
				break;
			}
			string s = "";
			s.append(room_type);
			msg_map[option].msg_val = s;
			msg_map[option].msg_len = s.length();

			snprintf(sendBuffer, sizeof(sendBuffer), "%d-%d-%s\r\n", msg_map[option].msg_type,
				msg_map[option].msg_len, msg_map[option].msg_val.c_str());
		}
		else if (option == CONFIRM_RESERVATION)
		{
			
			snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type);
		}
		else if (option == CANCEL_RESERVATION)
		{
			
			snprintf(sendBuffer, sizeof(sendBuffer), "%d\r\n", msg_map[option].msg_type);
		}
		
	}
	cout << "message created is " << sendBuffer << endl;
}


