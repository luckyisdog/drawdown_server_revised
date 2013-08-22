/*
 * mysql_connection.h
 *
 *  Created on: Aug 16, 2013
 *      Author: lucky
 */
#include "player.h"
#include <iostream>
#include <sstream>
#include <mysql.h>
#include <my_global.h>
#include <my_sys.h>

#ifndef MYSQL_CONNECTION_H_
#define MYSQL_CONNECTION_H_

#define DEFAULT_DIAMONDS 5
#define DEFAULT_POINTS 0
#define DEFAULT_WINS 0
#define DEFAULT_SOS 3



class mysql_connection{

public:
	MYSQL *conn;
	char* database_name;
	char* database_user;
	char* database_password;
	char* database_location;
	mysql_connection(char* database_name, char* database_user, char* database_password, char* database_location){
		this->database_name = database_name;
		this->database_user = database_user;
		this->database_password = database_password;
		this->database_location = database_location;
	}
	bool connect();
	//WITHOUT FACEBOOK COMMANDS
	bool checkRegistered(std::string email);
	player newPlayer(std::string email, std::string password, std::string display_name);
	bool savePlayer(player requested_player, int database_id);
	player loginPlayer(std::string email, std::string password);
private:

};


#endif /* MYSQL_CONNECTION_H_ */
