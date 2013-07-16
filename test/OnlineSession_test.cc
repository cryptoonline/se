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
//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/" ;
//	BStore store(directoryPath);
	while(true){
		OnlineSession* writeSession = new OnlineSession();;
		string filenumber;
		string newfilenumber;

		cout << "Enter filenumber you want to update (Enter q to quit): ";
		cin >> filenumber;
		if(filenumber.compare("q") == 0){
			cout << "Thank you for using Blind Storage System." << endl;
			break;
		}

		cout << "Enter new filenumber (Enter q to quit): ";
		cin >> newfilenumber;
		if(newfilenumber.compare("q") == 0){
			cout << "Thank you for using Blind Storage System." << endl;
			break;
		}
			
		string filename = directoryPath + filenumber + ".";
		string newfilename = directoryPath + newfilenumber + ".";
		
		size_t size = readFileSize(newfilename);
		byte updateFileBytes[size];
		readFile(newfilename, updateFileBytes, size);
		
		writeSession->update(filename, updateFileBytes, size);
		delete writeSession;
	}

	while(true){
		string filenumber;
		cout << "Enter file to read (Enter q to quit): ";
		cin >> filenumber;

		if(filenumber.compare("q") == 0){
			cout << "Thank you for using Blind Storage System." << endl;
			break;
		}

		string filename = directoryPath + filenumber + ".";
		OnlineSession* readSession = new OnlineSession();
		byte* file;
		size_t filesize = readSession->read(filename, file);
		printchars(file, filesize, "UPDATED FILE");
		delete[] file;
		delete readSession;
	}
}
