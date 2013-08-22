//============================================================================
// Name        : drawdown_server_v3.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define _GNU_SOURCE
#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <poll.h>
#include "player.h"
#include "mysql_connection.h"
#define PORT 6666
#define BUFFSIZE 255


#define ROOM_SIZE 2
#define ROOM_TIMEOUT 10
#define ROOM_ROUND_TIME 90

#define POINTS_PER_GAME 50
#define DEFAULT_DIAMONDS 10
#define DEFAULT_SOS 5

using namespace std;

mysql_connection my_mysql = mysql_connection("drawdown_db", "root", "wierdo", "127.0.0.1");
int master_socket;
vector<player> online_players;
bool server_online = false;
sem_t request_sem;

struct request{
	int socket;
	string action;
	vector<string> attributes;
};
vector<request> requests;
void sendMessage(int socket, char* message){
				if (send(socket, message, strlen(message), 0)
						!= strlen(message)) {
					perror("send");
				}
}
void* requestThread(void*){

	while(server_online){
		sem_wait(&request_sem);
		if(requests.size() > 0){
			if(requests[0].action == "LOGIN"){
				player new_player = my_mysql.loginPlayer(requests[0].attributes[0], requests[0].attributes[1]);
			}
			if (requests[0].action == "REGISTER"){
				player new_player = my_mysql.newPlayer(requests[0].attributes[0], requests[0].attributes[1], requests[0].attributes[1]);
				if(new_player.display_name == "ALREADY_REGISTERED"){
					sendMessage(requests[0].socket,"ALREADY_REGISTERED;\0");
				}else{
					new_player.socket = requests[0].socket;
					sendMessage(requests[0].socket,"SUCCESSFUL_LOGIN;\0");
					online_players.push_back(new_player);
				}
			}
			requests.erase(requests.begin());
		}
	}
}
void* messageThread(void*){
	vector<pollfd> poll_sockets;
	pollfd* poll_sockets_ptr;
	int poll_activity;
	pollfd add_socket;
	add_socket.fd = master_socket;
	add_socket.events = POLLIN;
	poll_sockets.push_back(add_socket);
	while(server_online){
		poll_sockets_ptr = &poll_sockets[0];
		poll_activity = poll(poll_sockets_ptr, poll_sockets.size(), -1);
		if(poll_activity < 0){
			perror("poll");
		}
		if(poll_sockets[0].revents & POLLIN){
			int new_socket;
			struct sockaddr_in client_address;
			int client_addrlen = sizeof(client_address);
			if ((new_socket = accept(master_socket,
					(struct sockaddr*) &client_address,
					(socklen_t*) &client_addrlen)) < 0) {
				perror("accept");
			} else {
				pollfd add_socket;
				add_socket.fd = new_socket;
				add_socket.events = POLLIN;
				poll_sockets.push_back(add_socket);
				cout << "Client " << new_socket << " has connected! ("
						<< inet_ntoa(client_address.sin_addr) << ":"
						<< ntohs(client_address.sin_port) << ")" << endl;
				cout << "Connected count (including server): " << poll_sockets.size() << endl;
			sendMessage(new_socket,"SUCCESSFUL_CONNECTION;\0");

			}

		}
		for(int i=0; i<poll_sockets.size(); i++){
			if(i!=0){
				if(poll_sockets[i].revents & POLLIN){
					char buffer[BUFFSIZE];
					if ((read(poll_sockets[i].fd, buffer, BUFFSIZE)) != 0) {
						vector<string> client_messages;
						char *msg_tok = strtok(buffer, ";");
						while(msg_tok != NULL){
							client_messages.push_back(msg_tok);
							msg_tok = strtok(NULL, ";");
						}

						if (client_messages[0] == "LOGIN"){
							if(client_messages.size() > 3){
							request new_request;
							new_request.socket = poll_sockets[i].fd;
							new_request.action = client_messages[0];
							new_request.attributes.push_back(client_messages[1]);
							new_request.attributes.push_back(client_messages[2]);
							requests.push_back(new_request);
							sem_post(&request_sem);
							}
						}
						if (client_messages[0] == "REGISTER"){
							if (client_messages.size() > 4){
								request new_request;
								new_request.socket = poll_sockets[i].fd;
								new_request.action = client_messages[0];
								new_request.attributes.push_back(client_messages[1]);
								new_request.attributes.push_back(client_messages[2]);
								new_request.attributes.push_back(client_messages[3]);
								requests.push_back(new_request);
									sem_post(&request_sem);
							}
						}
						delete msg_tok;
					}else{
						cout << "Client " << i << " has disconnected!" << endl;
						poll_sockets.erase(poll_sockets.begin()+i);
						cout << "Connected count (including server): " << poll_sockets.size() << endl;
						//disconnected
					}
				}
			}
		}

	}
	return 0;
}
int main(int argc, char* argv[]){
	struct sockaddr_in address;
	address.sin_port = htons(PORT);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	master_socket = socket(AF_INET,SOCK_STREAM,0);
	if((bind(master_socket, (struct sockaddr*)&address, sizeof(address)))<0){
		perror("bind");
		return 1;
	}
	if(listen(master_socket,0)){
		perror("listen");
		return 1;
	}
	if(!my_mysql.connect()){
		perror("mysql");
		return 1;
	}
	if((sem_init(&request_sem, 0,0)) != 0){
		perror("semaphore");
		return 1;
	}
	server_online = true;
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, messageThread, NULL);
	pthread_create(&t2, NULL, requestThread, NULL);
	pthread_join (t1, NULL);
	pthread_join (t2, NULL);
	return 0;
}
