/*
 * TASK1.C
 *
 *  Created on: 10.09.2019
 *      Author: aml
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h> 

#include "TASK1.H"
#include "SHA256.H"

namespace TASK1{

string BlackBoxUnsafe::getSymbols(){
	return string(charSymbArray_);
};

BlackBoxUnsafe::BlackBoxUnsafe(int pwdLength, int symbSetSize){
	if((symbSetSize < 1) || (symbSetSize > SYMBOLS.length()) ){
		lengthSymbArray_ = SYMBOLS.length();
	}else{
		lengthSymbArray_ = symbSetSize;
	}
	// extract and save the subset of valid symbols
	charSymbArray_ = new char [lengthSymbArray_ + 1];
	strncpy(charSymbArray_, SYMBOLS.c_str(), lengthSymbArray_);
	// create and save random password
	pwd_ = this->randomPwd(pwdLength);
}

string BlackBoxUnsafe::input(string strPwd){
	if(strPwd.compare(pwd_) == 0){
		return string("ACCESS ACCEPTED");
	}
	return string("ACCESS DENIED");
}

/// @brief Sets the password member variable pwd_ to the password hash 
BlackBoxSafe::BlackBoxSafe(int pwdLength, int symbSetSize) : BlackBoxUnsafe(pwdLength,symbSetSize){
	pwd_ = sha256(pwd_);
	return;
}

/// @brief compares the input to the saved password 
/// @param strPwd 
/// @return returns either "ACCESS ACCEPTED" if the PW hashes are identical or "ACCESS DENIED" if they are not
std::string BlackBoxSafe::input(std::string strPwd)
{
	strPwd= sha256(strPwd.substr(0,strPwd.length()));
	cout<<strPwd<<endl<<pwd_<<endl;
	if(strPwd.compare(pwd_) == 0){
		return string("ACCESS ACCEPTED");
	}
	return string("ACCESS DENIED");
}

string BlackBoxUnsafe::randomPwd(int l){
	int symbolIdx;
	if(l < MINIMAL_PWD_LENGTH){ l = MINIMAL_PWD_LENGTH;};
	pwd_ = string("");
	for(int i=0; i < l; i++){
		symbolIdx = rand() % lengthSymbArray_;
		pwd_ += charSymbArray_[symbolIdx];
	}
	return pwd_;
}

/// @brief redundand function 
string BlackBoxSafe::nextPassword(string oldPassword, int symbSetSize, int currentPosition)
{
	if(currentPosition == oldPassword.length()-1)
	{
		return oldPassword;
	}
	char workChar = oldPassword[currentPosition];
	if(workChar == SYMBOLS[symbSetSize-1]) 		//Overflow
	{
		workChar = SYMBOLS[0];
		oldPassword[currentPosition] = workChar;
		return nextPassword(oldPassword, symbSetSize, currentPosition+1);
	}
	int index;
	for(int i = 0; i<SYMBOLS.length(); i++)			// Add one
	{
		if(workChar == SYMBOLS[i])
			index = i+1;					
	}
 	oldPassword[currentPosition] = SYMBOLS[index];
	return oldPassword;
}

void demoTASK1_00(){
	string pwd("meinpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	pwd = ("einpassword");
	cout << "pwd   : " << pwd << endl;
	cout << "SHA256: " << sha256(pwd) << endl;
	return;
}

void demoTASK1_01(){
	TASK1::BlackBoxUnsafe bs(4,4);
	cout << bs.pwd_ << endl;

	cout << bs.input(bs.pwd_) << endl;

	return;
}

}


