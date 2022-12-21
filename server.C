/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */
 #include <iostream>
#include <cstdio> // standard input and output library
#include <cstdlib> // this includes functions regarding memory allocation
#include <cstring> // contains string functions
#include <cerrno> //It defines macros for reporting and retrieving error conditions through error codes
#include <ctime> //contains various functions for manipulating date and time


#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses

#include "SIMPLESOCKET.H"
#include "TASK1.H"
#include "SHA256.H"

using namespace TASK1;

class myServer : public TCPserver
{
public:
	myServer(int port, int size) : TCPserver(port, size){ 
		
		Password = new BlackBoxSafe(4,4);
	};
	string myResponse(string input)
	{	
		/*if(input == "<Neues PW - Befehl>"){
			Password = new BlackBoxSafe(var1, var2); //var1 & var2 mit sstream und sscanf aus input auslesen
		}else{...}*/
		cout << input << endl;
		
		input = sha256(input);

		cout << input << endl;
		if(input.length() < 4){//if input string is shorter than the required minimum length of the password
			return("Input too short!");
		}
		
		int var1, var2;

		std::string substr = input.substr(0,8); //Takes the first 8 characters from the string and stores them as a string in a substring
		
		if(substr == "{newPwd}"){
			std::sscanf(input.c_str(), "{newPwd}(%i,%i)", &var1, &var2);
			Password = new BlackBoxSafe(var1, var2); //var1 & var2 mit sstream und sscanf aus input auslesen
		}else{
			return Password->input(input);
			}

		return 0;

	}
	void Start(string input)
	{
		

	}
private:
	BlackBoxSafe *Password;
};


int main(){
	srand(time(nullptr));
	myServer srv(2022,25);
	srv.run();
}
