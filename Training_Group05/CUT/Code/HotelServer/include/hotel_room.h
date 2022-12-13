#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <common.h>

using namespace std;

class hotel_room
{
    public:
        hotel_room();
        ~hotel_room();
        void create(int id, string type, string status, double price);
        void display();
        string getRoomType();
        int getRoomNo();
        bool isAvailable();
        double getPrice();
        void reserve();
        void unreserve();
    private:
    	int id;
        string type;
        string status;
        double price;
};

class hotel_rooms {
    public:
        static string getAvailableRoomWithPrice(string type, int noOfDays);
        static hotel_room getAvailableRoom(string type);
        static void init(string inputFile);
        static vector<hotel_room> rooms;
        static bool reserveRoom(int id);
        static bool unreserveRoom(int id);
        static void display();
    private:
        static bool isInit;
};

#endif

