#ifndef HOTEL_H
#define HOTEL_H

#include<iostream>
#include<set>
#include<fstream>
#include<string>
#include<stdio.h>
#include<algorithm>
#include<stdlib.h>
#include<time.h>

using namespace std;

class hotel
{
	private:
		int mroom_num;
		string mroom_type;
		int mroom_cost;
		int r;
	public:
		hotel();
		hotel(string room_type ,int room_cost,int room_num);
		int setroom(string room_type ,int room_cost,int r);
		int rooms();
		int cancel(int room);
};
#endif
