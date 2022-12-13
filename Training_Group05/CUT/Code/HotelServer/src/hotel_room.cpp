#include <iostream>
#include <common.h>
#include <hotel_room.h>

using namespace std;

hotel_room::hotel_room(){
	id = -1;
}
hotel_room::~hotel_room(){    
}

void hotel_room::create(int id, string type, string status, double price){
    this->id = id;
    this->type = type;
    this->status = status;
    this->price = price;
}
string hotel_room::getRoomType(){
    return type;
}
int hotel_room::getRoomNo(){
    return id;
}
double hotel_room::getPrice(){
    return price;
}
bool hotel_room::isAvailable(){
    return status == "1";
}
void hotel_room::reserve(){
    status = "0";
}
void hotel_room::unreserve(){
    status = "1";
}
void hotel_room::display(){
    cout<<id<<"\t\t"<<type<<"\t\t\t\t\t\t\t"<<status<<"\t\t\t\t"<<price<<endl;
}
vector<hotel_room> hotel_rooms::rooms;
bool hotel_rooms::isInit;

void hotel_rooms::display(){
    for(unsigned int i = 0; i< rooms.size(); i++){
        rooms[i].display();
    }
}

//getting room details with price
string hotel_rooms::getAvailableRoomWithPrice(string type, int noOfDays){
    cout<<"request for room type: "<<type<<endl;
    for(unsigned int i = 0; i< rooms.size(); i++){
        if(rooms[i].getRoomType() == type && rooms[i].isAvailable() )
        {
            stringstream ss;
            ss << "Room type: " << type << " Room NO: " << rooms[i].getRoomNo() << " Price: " << rooms[i].getPrice() * noOfDays;
            return ss.str();
        }
    }
        
    return "";
}

//reserved room
bool hotel_rooms::reserveRoom(int id){
    bool found = false;
    for(unsigned int i = 0; i< rooms.size(); i++){
        if(rooms[i].getRoomNo() == id )
        {
            rooms[i].reserve();
            found = true;
        }
    }
    hotel_rooms::display();
    return found;
}

//unreserved room
bool hotel_rooms::unreserveRoom(int id){
    bool found = false;
    for(unsigned int i = 0; i< rooms.size(); i++){
        if(rooms[i].getRoomNo() == id )
        {
            rooms[i].unreserve();
            found =  true;
        }
    }
    hotel_rooms::display();
    return found;
}

hotel_room hotel_rooms::getAvailableRoom(string type){
    cout<<"request for room type: "<<type<<endl;
    for(unsigned int i = 0; i< rooms.size(); i++){
        if(rooms[i].getRoomType() == type && rooms[i].isAvailable() )
        {
            return rooms[i];
        }
    }
    hotel_room h;
    return h;
}

//loading file to read data when the server starts
void hotel_rooms::init(string inputFile){
    cout<<"Reading file: "<<inputFile<<endl;
    cout<<"isInit:" <<hotel_rooms::isInit<<endl;
    if(hotel_rooms::isInit == true) {
        cout<<"Hotel rooms already loaded:"<<endl;
        return;
    }
cout<<"loading file...:"<<endl;
     hotel_rooms::isInit = true;
    string line;
	ifstream input;
    string id;
	string type;
	string status;
    double price;
    const char delimiter = ',';
    bool hasHeader = true;
    try {
		input.open(inputFile);
		if (!input.is_open())
		{
            cout<<"File not exists.\n";
			throw "UNABLE TO OPEN THE INPUT FILE GIVEN!!! " ;
			return;
	    }
        int lineCount = 0;
        while (getline(input, line)){
            lineCount++;
            if(lineCount == 1 && hasHeader == true) continue;
            line.erase( std::remove(line.begin(), line.end(), '\r'), line.end() );
            line.erase( std::remove(line.begin(), line.end(), '\n'), line.end() );
            istringstream ss(line);
            vector<string> words;
            string word;
            while (getline(ss, word, delimiter)){
                words.push_back(word);
            }
            id = words[0];
            type = words[1];
            status = words[2];
            price = stod(words[3]);
            hotel_room r;
            r.create(atoi(id.c_str()),type,status,price);
            rooms.push_back(r);
        }
    }
	catch (...) {
	}
    if (input.is_open()) input.close();
    cout<<"Rooms load completed: total total: "<<hotel_rooms::rooms.size()<<endl;
    for(unsigned int i = 0; i< rooms.size(); i++){
        rooms[i].display();
    }

}


