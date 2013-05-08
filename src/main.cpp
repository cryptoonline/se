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
#include "SSE.h"

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

int64_t readFileSize(string filename){
	ifstream file(filename.c_str(), std::ios_base::binary | std::ios_base::ate);
	return file.tellg();
}

unsigned char* readFile(string filename){
	ifstream file(filename.c_str());
	int64_t filesize = readFileSize(filename);
	file.seekg(0, std::ios::beg);
	char* bytes = new char[filesize];
	file.read(bytes, filesize);
	return reinterpret_cast<unsigned char*>(bytes);
}
	
int main(int argc, const char * argv[]){
	Communicator comm;
	SSE sse;
	sse.indexGen("/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/");
	getchar();
	BStore store(comm);	
//	BStore store(comm, "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/");
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
//	unsigned char plaintext[] = "8TR Mode Test";
//	unsigned char key[] = {0xF5, 0x34, 0xFC, 0x7F, 0x05, 0x65, 0xA8, 0xCF, 0x16, 0x29, 0xF0, 0x1D, 0xB3, 0x1A, 0xE3, 0xCA};
//	unsigned char iv[] = {0xA4, 0xD1, 0x6C, 0xBC, 0x01, 0x0D, 0xAC, 0xAA, 0x2E, 0x54, 0xFA, 0x67, 0x6B, 0x57, 0xA3, 0x45};
//	AES cipher;
//	unsigned char* ciphertext = cipher.ENC(plaintext, 128, key, iv);
//	for(int i = 0; i < 16; i++){
//		printf("%02X", ciphertext[i]);
//	}

	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	vector<unsigned char> data;
//	store.read("/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/calendar/1.", data);
	store.read("/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/bass-e/inbox/2.", data);
	cout << "File is here " << endl;
	for(int i = 0; i < data.size(); i++)
		cout << (char) data[i];
	getchar();	
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	string filename = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/holst-k/inbox/8.";
	store.write(filename, readFile(filename), readFileSize(filename));
	vector<unsigned char> data3;
	store.read(filename, data3);
	for(int i = 0; i < data3.size(); i++){
		cout << (char) data3[i];
	}
	cout << readFileSize(filename) << endl;
	getchar();
	cout << readFile(filename) << endl;
	cout << "File size is " << readFileSize(filename) << endl;
	getchar();
//	unsigned char* data1 = store.read("/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/holst-k/inbox/8.");
//   for(int i = 0; i < 2045; i++)
//   	cout <<(char) data1[i];
//   return 0;
}





