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

TEST(OnlineSessionReadwithFileBStore, Test1){
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";
	BStore store;

	int numFiles = 12;
	string filenumbers[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9" , "10", "267", "207"};
	for(int i = 0; i < numFiles; i++){
		string filename = directoryPath + filenumbers[i] + ".";
		size_t size = readFileSize(filename);
		byte fileBytes[size];
		readFile(filename, fileBytes, size);
		store.add(filename, fileBytes, size);
	}
	store.finalize();

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

		cout << "Enter filenumber you want to write (Enter q to quit): ";
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
		cout << "New file size is " << size << endl;
		byte updateFileBytes[size];
		memset(updateFileBytes, 0, size);
		readFile(newfilename, updateFileBytes, size);
		
		writeSession->write(filename, updateFileBytes, size);
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

TEST(OnlineSessionUpdate, Test1){
//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	cout << "This is updated test." << endl;
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
		cout << "New file size is " << size << endl;
		byte updateFileBytes[size];
		memset(updateFileBytes, 0, size);
		readFile(newfilename, updateFileBytes, size);
		
		byte* file;
		size_t filesize = writeSession->updateRead(filename, file, size);
	
		cout << "File size retrieved is " << filesize << endl;
		cout << "Updated file size is " << size + filesize << endl;
		byte* updatedContents = new byte[size+filesize];
	
		memcpy(updatedContents, updateFileBytes, size);
		memcpy(&updatedContents[size], file, filesize);

		printchars(updatedContents, size + filesize, "EXPECTED FILE");
		
		writeSession->updateWrite(filename, updatedContents, size+filesize);
		
		if(file)
			delete[] file;		
		
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

TEST(OnlineSessionUpdate, Test2){
//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	cout << "This is updated test." << endl;
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

		string filename = directoryPath + filenumber + ".";
		
		byte* file;
		size_t filesize = writeSession->updateRead(filename, file, 0);
	
		cout << "File size retrieved is " << filesize << endl;
	
//		memcpy(updatedContents, updateFileBytes, size);
//		memcpy(&updatedContents[size], file, filesize);

		memset(file+filesize/2, 0, filesize/2);
		printchars(file, filesize/2, "EXPECTED FILE");
		
		writeSession->updateWrite(filename, file, filesize/2);
		
		if(file)
			delete[] file;		
		
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

TEST(OnlineSessionRemove, Test1){
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";
	OnlineSession sessionDelete;
	string filename1 = directoryPath + "2" + ".";
	string filename2 = directoryPath + "207" + ".";
	sessionDelete.remove(filename1);

	OnlineSession session0;
	byte* file0;
	size_t filesize0 = session0.read(filename1, file0);
	byte expectedFile[filesize0];
	memset(expectedFile, 0, filesize0);
	EXPECT_TRUE(0 == memcmp(file0, expectedFile, filesize0));
	printchars(file0, filesize0, "DELETED FILE");
	delete[] file0;

	OnlineSession session1;
	size_t filesize1 = readFileSize(filename2);
	byte updateFileBytes[filesize1];
	readFile(filename2, updateFileBytes, filesize1);
	session1.write(filename1, updateFileBytes, filesize1);

	OnlineSession session2;
	byte* file2;
	size_t filesize2 = session2.read(filename1, file2);
	printchars(file2, filesize2, "UPDATED DELTED FILE");
	EXPECT_TRUE(0 == memcmp(file2, updateFileBytes, filesize2));
	delete[] file2;

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
