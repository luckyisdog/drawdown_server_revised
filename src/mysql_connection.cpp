/*
 * mysql_connection.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: lucky
 */


#include "mysql_connection.h"

bool mysql_connection::connect(){
	this->conn = mysql_init(NULL);
		if (mysql_real_connect(conn, this->database_location, this->database_user,
					this->database_password, this->database_name, 0, NULL, 0) == NULL) {
				perror("mysql");
				return false;
			}
		return true;
}
