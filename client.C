/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <sstream>
#include <iostream>
#include <unistd.h> 
#include <chrono>
#include <thread>
#include <fstream>

//contains various constants

#include "SIMPLESOCKET.H"
#include "TASK1.H"

using namespace std;
string nextPassword(string oldPassword, int symbSetSize, int currentPosition);
int main(){
	srand(time(NULL));
	TCPclient c;
	string host = "localhost";
	string msg;
	string initPW;
	fstream f;
	int counter = 0;

	
	//connect to host
	c.conn(host , 2022);

	bool goOn=true;
	f.open("Daten.dat", ios::out);

	while(goOn){ // send and receive data
		string guess;
		int length;
		int size;
		int numberOfTry;
		int differenPassword = 0;

		string pwdLength;
		cin>>guess;
		c.sendData(guess);
		msg = c.receive(32);
		if(guess.compare(0, 7,"newPwd(") == 0)
		{
			std::sscanf(guess.c_str(), "newPwd(%i,%i,%i)", &length, &size, &numberOfTry);
			initPW = std::string(length, TASK1::SYMBOLS[0]);
		}
		f << "Password length: " << length << "Size of variable: " << size << endl;
		//Hier werden jetzt alle PW durchprobiert
		c.sendData(initPW);
		msg = c.receive(32);							
		while(true)
		{
			initPW = nextPassword(initPW, size, length-1);
			cout<<"clients sends:"<< initPW << endl;
			c.sendData(initPW);
			msg = c.receive(32);
			cout << "got response: " << msg << endl;
			cout << "Try number: " << counter << endl;
			counter ++;
			if(msg.compare(0,15,"ACCESS ACCEPTED") == 0)
			{
				f<< counter-1 << endl;
				ostringstream oss;
				oss << "newPwd(" << length <<"," << size;
				guess = oss.str();
				c.sendData(guess);
				msg = c.receive(32);
				initPW = std::string(length, TASK1::SYMBOLS[0]);
				c.sendData(initPW);
				msg = c.receive(32);
				differenPassword++;
				counter = 1;
				
			}
			if(differenPassword == numberOfTry)
			{
				break;
			}
		}

	}
}

string nextPassword(string oldPassword, int symbSetSize, int currentPosition)
{
	if(currentPosition < 0)
	{
		return oldPassword;				//HACK
	}
	char workChar = oldPassword[currentPosition];
	if(workChar == TASK1::SYMBOLS[symbSetSize-1]) 		//Overflow
	{
		workChar = TASK1::SYMBOLS[0];
		oldPassword[currentPosition] = workChar;
		return nextPassword(oldPassword, symbSetSize, currentPosition-1);
	}
	int index;
	for(int i = 0; i<TASK1::SYMBOLS.length(); i++)			// Add one
	{
		if(workChar == TASK1::SYMBOLS[i])
		{
			index = i+1;	
			break;	
		}			
	}
	if(index == TASK1::SYMBOLS.length())
	{
		index = index-1;						//HACK
	}
 	oldPassword[currentPosition] = TASK1::SYMBOLS[index];
	return oldPassword;
}