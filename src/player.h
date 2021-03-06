/*
 * player.h
 *
 *  Created on: Aug 16, 2013
 *      Author: lucky
 */
#include <cstring>
#include <string>

#ifndef PLAYER_H_
#define PLAYER_H_

class player{
public:
	int socket;
	int database_id;
	int facebook_id;
	//online, offline, in_game, in_queue
	std::string status;
	std::string display_name;
	std::string profile_picture;
	int value_diamonds;
	int value_points;
	int value_sos;
	int value_wins;
	std::string already_seen_newbies;
	std::string already_seen_regulars;
	std::string already_seen_highrollers;
//	player(int socket, int database_id, char* display_name, char* profile_picture, int value_diamonds, int value_points, int value_sos, int value_wins){
//		this->database_id = database_id;
//		this->display_name = display_name;
//		this->profile_picture = profile_picture;
//		this->value_diamonds = value_diamonds;
//		this->value_points = value_points;
//		this->value_sos = value_sos;
//		this->value_wins = value_wins;
//	}
	void removeDiamonds(int amount);
	void increaseDiamonds(int amount);
	void increaseWin();
	void increasePoints(int amount);
	void removeSOS(int amount);
	void increaseSOS(int amount);
	void setStatus(std::string status);
	int getDiamonds();
	int getPoints();
	int getSOS();
	int getWins();
	int getDatabaseId();
	std::string getDisplayName();
	std::string getProfilePicture();
	int getSocket();
};


#endif /* PLAYER_H_ */
