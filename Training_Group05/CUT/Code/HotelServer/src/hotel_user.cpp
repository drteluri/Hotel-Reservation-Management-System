#include <iostream>
#include <common.h>
#include <hotel_user.h>

using namespace std;

hotel_user::hotel_user(){    
}
hotel_user::~hotel_user(){    
}

string hotel_user::getPasswd(){
    return passwd;
}

void hotel_user::create(int id, string uname, string passwd){
    this->id = id;
    this->uname = uname;
    this->passwd = passwd;
}

void hotel_user::display(){
    cout<<id<<"\t\t"<<uname<<"\t\t"<<passwd<<endl;
}

map<string, hotel_user> hotel_users::users;
bool hotel_users::isInit;
bool hotel_users:: isuserExists(string uname){
    if (hotel_users::users.find(uname) != hotel_users::users.end())
        return true;
    return false;
}

//authenticating username and password
bool hotel_users:: authenticate(string uname, string password){
    cout<<"authenticate: "<<uname<<":"<<uname.length()<<""<<password<<":"<<password.length()<<endl;

    if (hotel_users::users.find(uname) != hotel_users::users.end()){
        string str = users[uname].getPasswd();

        cout<<"original pwd: "<< str<< ":" <<str.length()<<endl; 
        
        if(str == password){
            return true;
        }
    }
        
    return false;
}

//loading file at the server
void hotel_users::init(string inputFile){
    cout<<"Reading file: "<<inputFile<<endl;
    cout<<"isInit:" <<hotel_users::isInit<<endl;
    if(hotel_users::isInit == true) {
        cout<<"users already loaded:"<<endl;
        return;
    }
cout<<"loading file...:"<<endl;
     hotel_users::isInit = true;
    string line;
	ifstream input;
    string id;
	string name;
	string password;
    const char delimiter = ',';
    bool hasHeader = true;
    //using exception handling for handling the mentioned file
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
            name = words[1];
            password = words[2];
            hotel_user u;
            u.create(atoi(id.c_str()),name,password);
            hotel_users::users[name]= u;
        }
    }
	catch (...) {
	}
    if (input.is_open()) input.close();
    cout<<"users load completed: total users: "<<hotel_users::users.size()<<endl;
    
    for (auto const& x : hotel_users::users)
    {
        hotel_user tu = x.second;
        tu.display();
    }
}


