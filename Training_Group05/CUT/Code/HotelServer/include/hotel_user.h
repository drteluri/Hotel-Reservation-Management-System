#ifndef USER_H
#define USER_H
	
#include <string>
#include <common.h>

using namespace std;


class hotel_user
{
    public:
        hotel_user();
        ~hotel_user();
        void create(int id, string uname, string passwd);
        void setUserName(string);
        string getUserName();
        void setPasswd(string);
        string getPasswd();
        void display();
    private:
    	int id;
        string uname;
        string passwd;
};

class hotel_users {
    public:
        static bool isuserExists(string uname);
        static bool authenticate(string uname, string passwd);
        static void init(string inputFile);
        static map<string, hotel_user> users;
    private:
        static bool isInit;
};

#endif

