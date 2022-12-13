#include <hotel_tcpserver.h>
#include <common.h>
#include <map>

using namespace std;

map<int,Messaging> TcpServer::clientMessageMap;

TcpServer::TcpServer() {
    init_server();
    start_server();
}

TcpServer::~TcpServer() {

}

//initiating the server
void TcpServer::init_server()
{
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  
  memset(&server_addr, '0', sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(5000);
  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  listen(server_fd, 10);
  cout<<"Listening on Server Socket FD = "<<server_fd<<endl;
}

//starting the server
void TcpServer::start_server()
{
  int i = 0;
  while(1)
  {
    cout<<"Waiting on accept....:(:("<<endl; 
    client_fd[i] = accept(server_fd, (struct sockaddr*)NULL, NULL);
    pthread_create(&thread[i], NULL, client_handler, &client_fd[i]);
    i++;
    if (i == MAX_CLIENTS)
    {
      cout<<"Max Clients "<<i<<endl;
      break;
    }
  }
}

void TcpServer::wait_for_client_termination()
{
  for(int i = 0; i < MAX_CLIENTS; i++)
  {
    pthread_join(thread[i], NULL);
  }
}

//handling multiple clients
void* TcpServer::client_handler (void *client_fd)
{
  int *c_fd = (int *) client_fd;
  char recvBuff[64];
  int recv_bytes; 
  int cid=*c_fd;
  cout<< "cid: "<< cid<< endl;
  if (TcpServer::clientMessageMap.find(cid) == TcpServer::clientMessageMap.end()){
    Messaging m;
    TcpServer::clientMessageMap[cid] = m; 
  }
    
  memset(recvBuff, '0', sizeof(recvBuff));

  int x = 0;

  while(1)
  {
    recv_bytes = recv(cid, recvBuff, sizeof(recvBuff), 0);  
    if(recv_bytes < 0)
    {
      cout<<"\n Read error \n"<<endl;
      perror("Error: ");
      continue;
    }
    
    if(recv_bytes == 0)
    {
      cout<<"Client fd "<<*c_fd<<" is closed"<<endl;
      pthread_exit(NULL);
    }

    recvBuff[recv_bytes] = 0;

    cout<<"Client "<<*c_fd<<" Msg: "<<recvBuff<<endl;
    string sendMsg = TcpServer::clientMessageMap[cid].process_ClientMessage(recvBuff);
    cout<<"sending message to client: "<<sendMsg << endl;
    int n =send(*c_fd, sendMsg.c_str(), sendMsg.length(), 0);
    if(n<=0){
	    cout<<"Error:: message has zero bytes"<<endl;
    }
    sleep(2);
    x++;
  }
  cout<<"Exiting"<<endl;
  exit(0);
}

