#ifndef COMMON_H
#define COMMON_H

#include <cstdio>
#include <string>
#include <bits/stdc++.h>

#define MAX_PACKET_SIZE 4096

using namespace std;
namespace fd_wait {
    enum Result {
        FAILURE,
        TIMEOUT,
        SUCCESS
    };

    Result waitFor(int &fileDescriptor, uint32_t timeoutSeconds = 1);
};


typedef enum 
{
  UNAME        = 1,
  PASSWD       = 2,
  AUTH_SUCCESS = 3,
  AUTH_FAIL    = 4,
  USER_NOT_PRESENT = 5,
  ROOM_TYPE    = 6,  
  ROOM_NOTAVAILABLE = 7,
  ROOM_RESERV_SUCCESS = 8,
  ROOM_RESERV_FAIL = 9,
  ROOM_STATS = 10,
  CONFIRM_RESERVATION = 11,
  CANCEL_RESERVATION = 12,
  NEE_PASSWD = 13,
  INIT = 99,
  EXIT = 100,
  ROOMNO = 101,

} msg_types_e;

typedef struct messages
{
  msg_types_e msg_type;
  int         msg_len;
  string      msg_val;

}messages_t;

class Messaging
{
  map<msg_types_e, messages_t>  msg_map;
  void select_room_type();
  public:
    Messaging()
    {
      initialize_messages();
    }
    string create_message(msg_types_e option, string data);
    void create_message(msg_types_e option);
    void initialize_messages();
    ~Messaging(){
      msg_map.clear();
    }
};

#endif
