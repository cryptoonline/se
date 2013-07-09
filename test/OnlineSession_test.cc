//
// OnlineSession_test.cc
// BlindStorage Test
//

#include "gtest/gtest.h"

#include "./../src/OnlineSession.h"
#include "./../src/BStore.h"
#include "./../src/DiskCommunicator.h"

TEST(OnlineSessionGet, Test1){
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
//		std::string filename = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/260.";
		string filename = directoryPath + filenumber + ".";
		cout << "Reading file: " << filename << endl;
//		string filename = "/Users/naveed/BStore/datasets/testdir/test1";
		OnlineSession session;
		byte* file;
		size_t filesize = session.read(filename, file);
		printchars(file, filesize, "FILE");
		delete[] file;
		}
//	printchars(contents, size, "File contents");

}
