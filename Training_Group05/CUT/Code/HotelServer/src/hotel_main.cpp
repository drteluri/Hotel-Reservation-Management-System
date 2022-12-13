#include <iostream>
#include <common.h>
#include <hotel_tcpserver.h>
#include <hotel_room.h>


int main() 
{
  hotel_users hotelusers;
  hotel_rooms hotelrooms;
  cout<<"server main: "<<endl;
  hotelusers.init("../../data/users.csv");
  hotelrooms.init("../../data/hotelrooms.csv");
  TcpServer server;
  server.wait_for_client_termination();
  return 0;
}
