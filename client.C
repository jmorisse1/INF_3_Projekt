/*
 * client.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

#include <string>
#include <iostream>
#include <unistd.h> 
#include <chrono>
#include <thread>
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
	int counter = 0;

	
	//connect to host
	c.conn(host , 2022);

	bool goOn=true;

	while(goOn){ // send and receive data
		string guess;
		int length;
		int size;

		if(initPW.length() != 0)
		{
		}
		string pwdLength;
		cin>>guess;
		c.sendData(guess);
		if(guess.compare(0, 7,"newPwd(") == 0)
		{
			std::sscanf(guess.c_str(), "newPwd(%i,%i)", &length, &size);
			initPW = std::string(length, TASK1::SYMBOLS[0]);
		}
		//Hier werden jetzt alle PW durchprobiert
		c.sendData(initPW);								
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
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
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