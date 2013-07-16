//
// OnlineSession_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/OnlineSession.h"
#include "./../src/BStore.h"
#include "./../src/DiskCommunicator.h"
#include "./../src/helper.h"

TEST(OnlineSessionRead, Test1){
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";
//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	BStore store(directoryPath);
//	DiskCommunicator dcomm;
	
	while(true){
		cout << "Enter filenumber (Enter q to quit): ";
		string filenumber;
		cin >> filenumber;
		if(filenumber.compare("q") == 0){
			cout << "Thank you for using Blind Storage System." << endl;
			break;
		}
		string filename = directoryPath + filenumber + ".";
		cout << "Reading file: " << filename << endl;
		OnlineSession session;
		byte* file;
		size_t filesize = session.read(filename, file);
		printchars(file, filesize, "FILE");
		delete[] file;
		}
}

TEST(OnlineSessionWrite, Test1){
	cout <<"********************************************************************************************************************************************************************************************************" << endl; 
///	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/" ;
//	BStore store(directoryPath);
	OnlineSession session;
	string filename1 = directoryPath + "1.";
	string filename2 = directoryPath + "267.";
	size_t size = readFileSize(filename1);
	byte updateFileBytes[size];
	readFile(filename1, updateFileBytes, size);
	
	session.update(filename2, updateFileBytes, size);

	OnlineSession session1;
	byte* file;
	size_t filesize = session1.read(filename2, file);
	printchars(file, filesize, "FILE READ");
	delete[] file;
}
