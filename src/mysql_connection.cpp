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
bool mysql_connection::checkRegistered(std::string email){
	MYSQL_RES *res_set;
	MYSQL_ROW row;

	std::stringstream query;
	std::string query_import;
	query << "SELECT * FROM players WHERE email='" << email << "'";
	query_import = query.str();
	mysql_query(this->conn, query_import.c_str());
	res_set = mysql_store_result(conn);

	unsigned int numrows = mysql_num_rows(res_set);
	while ((row = mysql_fetch_row(res_set)) != NULL) {
		mysql_free_result(res_set);

		return true;
	}

	mysql_free_result(res_set);
	return false;

}
player mysql_connection::newPlayer(std::string email, std::string password, std::string display_name){
	if(!checkRegistered(email)){
		player new_player;
		new_player.display_name = display_name;
		new_player.profile_picture = "NA";
			new_player.value_diamonds = DEFAULT_DIAMONDS;
		new_player.value_points = DEFAULT_POINTS;
		new_player.value_sos = DEFAULT_SOS;
		new_player.value_wins = DEFAULT_WINS;
		new_player.already_seen_newbies = "";
		new_player.already_seen_regulars = "";
		new_player.already_seen_highrollers = "";
		std::stringstream query;
		std::string query_import;
				query << "INSERT into players (facebook_id,email,display_name,password,value_diamonds,value_wins,value_sos,value_points,already_seen_newbies,already_seen_regulars,already_seen_highrollers, profile_picture) VALUES('"
				<< "NA" << "','" << email
				<< "','" << display_name << "','"
				<< password << "','"
				<< new_player.value_diamonds << "','"
				<< new_player.value_wins << "','" << new_player.value_sos
				<< "','" << new_player.value_points << "','"
				<< new_player.already_seen_newbies << "','"
				<< new_player.already_seen_regulars << "','"
				<< new_player.already_seen_highrollers << "','"
				<< new_player.profile_picture << "')";
		query_import = query.str();
		mysql_query(this->conn, query_import.c_str());
		return new_player;
	}

		player new_player;
		new_player.display_name = "ALREADY_REGISTERED";
		return new_player;

}
int mysql_connection::checkLogin(std::string email, std::string password){
	MYSQL_RES *res_set;
	MYSQL_ROW row;
	std::stringstream query;
	std::string query_import;
	query << "SELECT database_id FROM players WHERE email='" << email << "' AND password='" << password << "'";
	query_import = query.str();
	mysql_query(this->conn, query_import.c_str());
	res_set = mysql_store_result(this->conn);
	unsigned int i = 0;
	unsigned int numrows = mysql_num_rows(res_set);
	while ((row = mysql_fetch_row(res_set)) != NULL) {
		mysql_free_result(res_set);
		return atoi(row[i]);

	}
	return -1;
}
player mysql_connection::loginPlayer(std::string email, std::string password){
	int id = checkLogin(email,password);
	if(id != -1){
		player login_player;
		MYSQL_RES *res_set; /* Create a pointer to recieve the return value.*/
				MYSQL_ROW row; /* Assign variable for rows. */
				//mysql_query(conn,"UPDATE players SET display_name='works' WHERE database_id=3");
				std::stringstream query;
				std::string query_import;
				query
						<< "SELECT facebook_id, email, display_name, password, value_diamonds, value_wins, value_points, value_sos, already_seen_newbies, already_seen_regulars, already_seen_highrollers, profile_picture, database_id FROM players WHERE database_id='"
						<< id << "'";
				query_import = query.str();
				mysql_query(this->conn, query_import.c_str());

				res_set = mysql_store_result(this->conn);

				unsigned int numrows = mysql_num_fields(res_set);

				while ((row = mysql_fetch_row(res_set))) {
					for (int i = 0; i < numrows; i++) {
						if (i == 0) { //FACEBOOK ID
							login_player.facebook_id = atoi(row[i]);

						}
						if (i == 2) { //DISPLAY_NAME
							login_player.display_name = row[i];

						}
						if (i == 4) { //VALUE_DIAMONDS
							login_player.value_diamonds = atoi(row[i]);

						}
						if (i == 5) { //VALUE_WINS
							login_player.value_wins = atoi(row[i]);

						}
						if (i == 6) { //VALUE_POINTS
							login_player.value_points = atoi(row[i]);

						}
						if (i == 7) { //VALUE_SOS
							login_player.value_sos = atoi(row[i]);

						}
						if (i == 8) { //ALREADY_SEEN_NEWBIES
							login_player.already_seen_newbies = row[i];

						}
						if (i == 9) { //ALREADY_SEEN_REGULARS
							login_player.already_seen_regulars = row[i];

						}
						if (i == 10) { //ALREADY_SEEN_HIGHROLLERS
							login_player.already_seen_highrollers = row[i];

						}
						if (i == 11) {
							login_player.profile_picture = row[i];

						}
						if (i == 12) {
									login_player.database_id = atoi(row[i]);

								}

					}

				}
				return login_player;
				mysql_free_result(res_set);
	}

		player new_player;
		new_player.display_name = "INCORRECT_LOGIN";
		return new_player;


}
