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
#include "mysql_player_login.h"
#define PORT 6666
#define BUFFSIZE 255


#define ROOM_SIZE 2
#define ROOM_TIMEOUT 10
#define ROOM_ROUND_TIME 90

#define POINTS_PER_GAME 50
#define DEFAULT_DIAMONDS 10
#define DEFAULT_SOS 5

using namespace std;


int master_socket;
vector<player*> online_players;
bool server_online = false;

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
				char* send_message = "SUCCESSFUL_CONNECTION;\0";
				if (send(new_socket, send_message, strlen(send_message), 0)
						!= strlen(send_message)) {
					perror("send");
				}

			}

		}
		for(int i=0; i<poll_sockets.size(); i++){
			if(i!=0){
				if(poll_sockets[i].revents & POLLIN){
					char buffer[BUFFSIZE];
					if ((read(poll_sockets[i].fd, buffer, BUFFSIZE)) != 0) {

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
	server_online = true;
	pthread_t t1;
	pthread_create(&t1, NULL, messageThread, NULL);
	pthread_join (t1, NULL);
	return 0;
}
