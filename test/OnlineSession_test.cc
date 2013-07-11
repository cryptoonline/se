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
	OnlineSession session;
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/" ;
	string filename1 = directoryPath + "1.";
	string filename2 = directoryPath + "2.";
	string filename3 = "/Users/naveed/BStore/datasets/testdir/1.";
	size_t size = readFileSize(filename3);
	byte updateFileBytes[size];
	readFile(filename3, updateFileBytes, size);
	session.write(filename1, updateFileBytes, size);

	byte* file;
	size_t filesize = session.read(filename1, file);
	printchars(file, filesize, "FILE READ");
	delete[] file;
}
