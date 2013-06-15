//
// helper.cpp
// BlindStorage
// Code taken from http://forums.codeguru.com/showthread.php?316299-C-General-What-are-different-number-representations

#include "helper.h"

void hexStringToByteArray(string hexstring, unsigned char bytes[]){
	int length = hexstring.length();

	if( length%2 == 1){
		cerr << "Length of hex string is invalid: not divisible by 2." << endl;	
	}

	stringstream ss(hexstring);
	for(int i = 0; i < length/2; i++){
		char ch1, ch2;
		ss >> ch1 >> ch2;
		int dig1, dig2;
		
		if( isdigit(ch1) )	dig1 = ch1 - '0';
		else if ( ch1>='A' && ch1<='F' ) dig1 = ch1 - 'A' + 10;
		else if ( ch1>='a' && ch1<='f' ) dig1 = ch1 - 'a' + 10;

		if( isdigit(ch2) ) dig2 = ch2 - '0';
		else if ( ch2>='A' && ch2<='F' ) dig2 = ch2 - 'A' + 10;
		else if ( ch2>='a' && ch2<='f' ) dig2 = ch2 - 'a' + 10;

		bytes[i] = dig1 * 16 + dig2;
	}
}

void gen_random(byte* s, const int len){
	
	static const byte alphanumsym[] = 
		"`~!@#$%^&*()-_=+[{]}\\|;:\'\",<.>/\?"
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

//	srand(clock());
	srand(100);
	for(int i = 0; i < len; ++i) {
		s[i] = alphanumsym[rand() % (sizeof(alphanumsym) - 1)];
	}

//	s[len] = 0;
}
