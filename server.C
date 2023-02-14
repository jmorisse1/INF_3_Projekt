/*
 * server.C
 *
 *  Created on: 11.09.2019
 *      Author: aml
 */

/// @file
/// @brief contains the implementation of the server

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
///
/// \class myServer
/// \brief establishes connection between Server and Client, implements the communication protocoll
class myServer : public TCPserver
{
public:
	myServer(int port, int size) : TCPserver(port, size){ 
		
		Password = new BlackBoxSafe(4,4);
	};
	/// @brief 
	/// @param input string send by client
	/// @return 
	string myResponse(string input)
	{	
		/*if(input == "<Neues PW - Befehl>"){
			Password = new BlackBoxSafe(var1, var2); //var1 & var2 mit sstream und sscanf aus input auslesen
		}else{...}*/
		if(input.length() < 4){//if input string is shorter than the required minimum length of the password
			return("Input too short!");
		}
		///
		/// @param length sets the length of the password
		/// @param size dictates how many different characters from the const string SYMBOLS are being used 
		///
		int length, size;

		//std::string substring = input.substr(0,6); //Takes the first 8 characters from the string and stores them as a string in a substring
		
		if(input.compare(0, 7,"newPwd(") == 0){ // input is compared
			std::sscanf(input.c_str(), "newPwd(%i,%i)", &length, &size); //takes two integer variables from predefined string
			delete Password; //Password is deleted in case there is still one in heap
			Password = new BlackBoxSafe(length, size);
			return string("new pwd created");
			
		}else{
			return Password->input(input); 
		}

		return string("unknown command"); //default case 

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
