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

#include <string>
using std::string;

#include <cstdlib>
using std::exit;

#include "cryptopp562/cryptlib.h"
using CryptoPP::Exception;

#include "cryptopp562/hmac.h"
using CryptoPP::HMAC;

#include "cryptopp562/sha.h"
using CryptoPP::SHA256;

#include "cryptopp562/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include "cryptopp562/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;
using CryptoPP::HashVerificationFilter;

#include "cryptopp562/secblock.h"
using CryptoPP::SecByteBlock;

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
	BStore store(comm, "/Users/naveed/Desktop/Projects/Code/enron_mail_20110402_withoutheaders/maildir");
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	unsigned char* data = store.read("/Users/naveed/Desktop/Projects/Code/enron_mail_20110402_withoutheaders/maildir/zufferli-j/sent_items/8.");
//	for(int i = 0; i < 32; i++)
//		cout << (char)data[i];	
	cout << endl << "*******************************************************************************************************************************************************************" << endl;
	return 0;
}





