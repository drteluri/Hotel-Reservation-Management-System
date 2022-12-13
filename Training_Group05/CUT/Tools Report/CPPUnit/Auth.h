#ifndef AUTH_H
#define AUTH_H

#include <iostream>
#include <cstring>

using namespace std;

class Auth{
	public:
		Auth();
		Auth(string, string);
		void setUser_Name(string);
		void setPasswd(string);
		string getUser_Name();
		string getPasswd();
		void login(string, string);
		void login_Verify();
                int validuser(string str);
		int checkpasswd(string);
		void display() const;

	private:
		string mUser_Name;
		string mPasswd;
		int n;

};

#endif
