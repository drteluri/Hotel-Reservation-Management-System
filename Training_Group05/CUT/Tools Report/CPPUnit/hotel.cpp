#include<iostream>
#include<set>
#include<fstream>
#include<string>
#include<stdio.h>
#include<algorithm>
#include<stdlib.h>
#include<time.h>

#include "hotel.h"

#define MAX_ROOM 10

using namespace std;
hotel::hotel(){}

hotel::hotel(string room_type,int room_cost,int room_num)
{
	this->mroom_type=room_type;
	this->mroom_cost=room_cost;
	this->mroom_num=room_num;
}
int hotel::setroom(string room_type,int room_cost,int r){
	this->mroom_type=room_type;
	this->mroom_cost=room_cost;
	this->mroom_num=r;	
	if(r>MAX_ROOM)
	{
		return 0;
	}
	else
	{
	fstream fout;
	fout.open("Record.txt",ios::out|ios::app|ios::in|ios::binary);
		fout.write((char*)this,sizeof(hotel));
		fout.close();
		return 1;
	}
} 
int hotel::rooms()
{
	fstream fin("Record.txt",ios::in);
	
	hotel h;
	
	if(!fin.is_open())
	{
		return 0;
	}
	else
	{

	while(!fin.eof())
	{
                 
		fin.read((char*)this,sizeof(hotel));
		return 1;
	}

	fin.close();
}
return 0;
}
int  hotel::cancel(int room)
{
	int r,flag=0;
        int ch;
	hotel h;
	ifstream fin("Record.txt",ios::in);
	
	ofstream fout("temp.txt",ios::out|ios::app);
	if(!fin)
	{
		cout<<"record not found"<<endl;
	}
	if(1)
	{

		while(fin.read((char*)this,sizeof(hotel)))
		{
			if(mroom_num==room)
			{
			flag=1;


		        }
		}
	}
	if(flag)
	{

		while(fin.read((char*)this,sizeof(hotel)))
		{
			if(mroom_num!=room)
			{

				fout.write((char*) this,sizeof(hotel));
			}
		}
	fin.close();
	fout.close();
	remove("Record.txt");
	rename("temp.txt","Record.txt");
		ch = 1;
	}
	else 
	{
		ch = 0;
	}
	return ch;

}

