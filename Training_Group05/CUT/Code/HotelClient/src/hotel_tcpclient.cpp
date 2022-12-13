#include <hotel_tcpclient.h>
#include <common.h>

using namespace std;
// Receive server packets, and notify user

void* receiveTask(void *arg) 
{

	while(TcpClient::isConnected()) 
	{
		char msg[MAX_PACKET_SIZE];
		const size_t numOfBytesReceived = recv(TcpClient::getSockFd(), msg, MAX_PACKET_SIZE, 0);

		if(numOfBytesReceived < 1) 
		{
			std::string errorMsg;
			if (numOfBytesReceived == 0) 
			{ //server closed connection
				errorMsg = "Server closed connection";
			} else {
				errorMsg = strerror(errno);
			}
			TcpClient::setIsConnected(false);
			return NULL;
		} else {
			msg[numOfBytesReceived] = 0;
			cout << "recevied message::" << msg << endl;

			stringstream ss(msg);
			string word;
			vector<string> words;
			while (getline(ss, word, '-')){
				words.push_back(word);
			}
			msg_types_e option = (msg_types_e) atoi(words[0].c_str());
			if(words.size() >=3 )
			{
				string data = words[2];
				cout << "server response: "<< data << endl;
			}
			cout << "recevied option::" << option << endl;
			TcpClient::_msg.create_message(option);                    
		}
	}
	
	return NULL;
}

int TcpClient:: _sockfd;
std::atomic<bool> TcpClient::_isConnected;
std::atomic<bool> TcpClient::_isClosed;
struct sockaddr_in TcpClient::_server;
pthread_t TcpClient::_receiveThread;
Messaging TcpClient::_msg;

//connection to server

bool TcpClient::connectTo(const std::string & address, int port) {
	cout<<"connecting 1 to " << address << endl;
	try {
		initializeSocket();
		setAddress(address, port);
	} catch (const std::runtime_error& error) {
		return -1;
	}
	cout<<"connecting to " << address << endl;
	const int connectResult = connect(_sockfd, (struct sockaddr *)&_server , sizeof(_server));
	cout<<"connection result: "<<connectResult<<endl;

	const bool connectionFailed = (connectResult == -1);
	if (connectionFailed) {
		return false;
	}

	pthread_create(&_receiveThread, NULL, receiveTask, NULL); 
	_isConnected = true;
	_isClosed = false;
	return true;
}

//initialising sockets

void TcpClient::initializeSocket() {

	_sockfd = socket(AF_INET , SOCK_STREAM , 0);

	if (_sockfd == -1) 
	{
		throw std::runtime_error(strerror(errno));
	}
}

void TcpClient::setAddress(const std::string& address, int port) {
	const int inetSuccess = inet_aton(address.c_str(), &_server.sin_addr);

	if(!inetSuccess) 
	{ // inet_addr failed to parse address
		// if hostname is not in IP strings and dots format, try resolve it
		struct hostent *host;
		struct in_addr **addrList;
		if ( (host = gethostbyname( address.c_str() ) ) == nullptr){
			throw std::runtime_error("Failed to resolve hostname");
		}
		addrList = (struct in_addr **) host->h_addr_list;
		_server.sin_addr = *addrList[0];
	}
	_server.sin_family = AF_INET;
	_server.sin_port = htons(port);
}

//sending message to server

int TcpClient::sendMsg(const char * msg, size_t size) 
{
	const size_t numBytesSent = send(_sockfd, msg, size, 0);

	if (numBytesSent < 0 ) 
	{ // send failed
		return -1;
	}

	if (numBytesSent < size) 
	{ // not all bytes were sent
		return -1;
	}
	return 0;
}



void TcpClient::waitForReceiveThread() 
{
	pthread_join(_receiveThread, NULL);
}

int TcpClient::close(){
	if (_isClosed)
	{
		return -1;
	}
	pthread_exit((void*)_receiveThread);
	_isClosed = true;
	return 0;
}


