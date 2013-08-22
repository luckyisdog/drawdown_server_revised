/*
 * player.cpp
 *
 *  Created on: Aug 16, 2013
 *      Author: lucky
 */

#include "player.h"

void player::increaseDiamonds(int amount){
	this->value_diamonds += amount;
}
void player::removeDiamonds(int amount){
	this->value_diamonds -= amount;
}
void player::increasePoints(int amount){
	this->value_points += amount;
}
void player::increaseSOS(int amount){
	this->value_sos += amount;
}
void player::removeSOS(int amount){
	this->value_sos -= amount;
}
void player::increaseWin(){
	this->value_wins++;
}
int player::getWins(){
	return this->value_wins;
}
int player::getSOS(){
	return this->value_sos;
}
int player::getDiamonds(){
	return this->value_diamonds;
}
int player::getPoints(){
	return this->value_points;

}
int player::getDatabaseId(){
	return this->database_id;
}
std::string player::getDisplayName(){
	return this->display_name;
}
std::string player::getProfilePicture(){
	return this->profile_picture;
}
int player::getSocket(){
	return this->socket;

}

void player::setStatus(std::string status){
	this->status = status;
}
