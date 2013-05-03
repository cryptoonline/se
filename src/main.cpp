//
//  main.cpp
//  BlindStorage
//
//  Created by Muhammad Naveed on 3/14/13.
//  Copyright (c) 2013 Muhammad Naveed. All rights reserved.
//

#include <stdint.h>
#include "DataBlock.h"
#include "AES.h"
#include "Ddisk.h"
#include "parameters.h"
#include "fileID.h"
#include <ext/hash_map>
#include "BStore.h"
#include "Blowfish.h"
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

	Blowfish cipher;
	cipher.keyGen();	
//	BStore store(comm);	
	BStore store(comm, "/home/naveed2/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/");
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
//	unsigned char plaintext[] = "8TR Mode Test";
//	unsigned char key[] = {0xF5, 0x34, 0xFC, 0x7F, 0x05, 0x65, 0xA8, 0xCF, 0x16, 0x29, 0xF0, 0x1D, 0xB3, 0x1A, 0xE3, 0xCA};
//	unsigned char iv[] = {0xA4, 0xD1, 0x6C, 0xBC, 0x01, 0x0D, 0xAC, 0xAA, 0x2E, 0x54, 0xFA, 0x67, 0x6B, 0x57, 0xA3, 0x45};
//	AES cipher;
//	unsigned char* ciphertext = cipher.ENC(plaintext, 128, key, iv);
//	for(int i = 0; i < 16; i++){
//		printf("%02X", ciphertext[i]);
//	}

	fileID fid1("/home/naveed2/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.");
	fileID fid2("/home/naveed2/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.");
	unsigned char* fid1Bytes = fid1.get();
	unsigned char* fid2Bytes = fid2.get();
	cout << "Index 1 " << fid1.getPRPofHigherID() << endl;
	cout << "Index 2 " << fid2.getPRPofHigherID() << endl;
	for(int i = 0; i < 32; i++)
		printf("%02X ", fid1Bytes[i]);
	cout << endl;

	for(int i = 0; i < 32; i++)
		printf("%02X ", fid2Bytes[i]);
	cout << endl;
	unsigned char* data = store.read("/home/naveed2/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.");
	for(int i = 0; i < 32; i++)
		cout << (int) data[i];	
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	return 0;
}





