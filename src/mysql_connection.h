/*
 * mysql_connection.h
 *
 *  Created on: Aug 16, 2013
 *      Author: lucky
 */
#include "player.h"
#include <mysql.h>
#include <my_global.h>
#include <my_sys.h>

#ifndef MYSQL_CONNECTION_H_
#define MYSQL_CONNECTION_H_



class mysql_connection{
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
	bool something();
	bool savePlayer(player requested_player, int database_id);
	bool loginPlayer(char* username, char* password);
};


#endif /* MYSQL_CONNECTION_H_ */
