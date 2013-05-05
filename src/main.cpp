//
//  main.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//
#define READ
#define BUILD

#include <stdint.h>
#include "DataBlock.h"
#include "AES.h"
#include "Ddisk.h"
#include "parameters.h"
#include "fileID.h"
#include <ext/hash_map>
#include "BStore.h"
#include "Blowfish.h"
#include "Debug.h"
#include <string>
using std::string;

#include <cstdlib>
using std::exit;

void print(string tag, unsigned char* value){
    cout << "<" << tag << ">" << value << "</" << tag << ">" << endl;
}

void print(string tag, char* value, uint32_t size){
    cout << endl << "*********************" << tag << " (start)*********************" << endl;
    for( int i = 0; i < size; i++)
        cout << value[i];
    cout << endl << "**********************" << tag << " (end)**********************" << endl;
}

int main(int argc, const char * argv[]){
	Communicator comm;

//	BStore store(comm);	
	BStore store(comm, "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/");
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
//	unsigned char plaintext[] = "8TR Mode Test";
//	unsigned char key[] = {0xF5, 0x34, 0xFC, 0x7F, 0x05, 0x65, 0xA8, 0xCF, 0x16, 0x29, 0xF0, 0x1D, 0xB3, 0x1A, 0xE3, 0xCA};
//	unsigned char iv[] = {0xA4, 0xD1, 0x6C, 0xBC, 0x01, 0x0D, 0xAC, 0xAA, 0x2E, 0x54, 0xFA, 0x67, 0x6B, 0x57, 0xA3, 0x45};
//	AES cipher;
//	unsigned char* ciphertext = cipher.ENC(plaintext, 128, key, iv);
//	for(int i = 0; i < 16; i++){
//		printf("%02X", ciphertext[i]);
//	}

	unsigned char* data = store.read("/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.");
	for(int i = 0; i < 1412; i++)
		cout << (char) data[i];	
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	return 0;
}





