#include <iostream>
#include <bits/stdc++.h>
#include <cstring>
#include <vector>
#include <map>
#include <fstream>
#include "Auth.h"

using namespace std;

Auth::Auth(){};

Auth::Auth(string user_name, string passwd){
	this->mUser_Name = user_name;
	this->mPasswd = passwd;
}

void Auth::setUser_Name(string user_name){
	this->mUser_Name = user_name;
}

string Auth::getUser_Name(){
	return mUser_Name;
}

void Auth::setPasswd(string passwd){
	this->mPasswd = passwd;
}

string Auth::getPasswd(){
	return mPasswd;
}


int Auth::validuser(string str){
	n=str.length();
	bool hasDigit = false;
	for (int i=0; i<n; i++){
		if (isdigit(str[i])){
			hasDigit = true;
		}
	}
    if(!str.empty() && hasDigit && (n>=3&&n<=6)){
        return 1;
     }else{
        return 0;
     }
}

int Auth::checkpasswd(string mPasswd){
	int n = mPasswd.length();
	bool hasLower = false;
	bool hasUpper = false;
	bool hasDigit = false;
	
	for (int i=0; i<n; i++){
		if (islower(mPasswd[i])){
			hasLower = true;
		}
		if (isupper(mPasswd[i])){
			hasUpper = true;
		}
		if (isdigit(mPasswd[i])){
			hasDigit = true;
		}
	}
	if(hasUpper && hasDigit && hasLower && (n>=6 && n<=12)){
		return 1;
	}else if((hasUpper || hasLower) && hasLower && (n>=6 && n<=12)){
		        return 0;
	}else{
		
		return 0;
	}
	if(n>12||n<6)
	{
		return 0;
	}
}
 


