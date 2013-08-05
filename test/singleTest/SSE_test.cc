//
// SSE_test.cc
// Searchable Encryption Test
//

#include "gtest/gtest.h"

#include <stdio.h>
#include <ctime>

#include "./../../src/parameters.h"
#include "./../../src/SSE.h"
#include "./../../src/sse_parameters.h"
#include <fstream>

void readDocs(SSE &sse);
void deletefile(string file);

TEST(SSEReadTest, Test1){
	/* Delete D and T created by BlindStorage Tests */
//	deletefile(D_FILE);
 //	deletefile(T_FILE);
///
	cout << "RAND_MAX is " << RAND_MAX << endl;
	cout << "Please wait! cleaning filestore." << endl;
	system("exec rm -r data/sse/filestore");
	system("exec mkdir data/sse/filestore");
	cout << "Fresh filestore created." << endl;

	while(1){
		SSE sse;

		cout << "Enter directory path (Enter q to quit): ";
		string directoryName;
		cin >> directoryName;

		if(directoryName.compare("q") == 0)
			break;

		string directoryPath = "/Users/naveed/BStore/datasets/" + directoryName + "/";
		sse.indexgen(directoryPath);
		
		readDocs(sse);
	}
 
}

TEST(SSERemoveTest, Test1){
//	for(int i = 0; i < 10; i++){
	string op;
	cout << "Enter i for generated and a for add: ";
	cin >> op;
	if(op.compare("i") == 0){
	SSE ssegen;
	ssegen.indexgen("/Users/naveed/BStore/datasets/test256MB/");
	}
	else{
	SSE sse;

	string directoryPath = "datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";

//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
//	while(1){
	//	string filename = "datasets/email/enron_mail_20110402_withoutheaders/maildir/dorland-c/deleted_items/20.";
			string filename = directoryPath + "207.";
//		cout << "Enter fiename of the file to be added: (Enter q to quit): ";
//		cin >> filename;

//		if(filename.compare("q") == 0)
//			break;
		clock_t starttime = clock();	
		docid_t maxFileNum = 10000;
		sse.add(maxFileNum, filename);
		cout << "Add took " << ((double)(clock() - starttime)/(double)CLOCKS_PER_SEC) << "." << endl;
	}
		//	}

//	readDocs(sse);
//	while(1){
//		string filename;
//		cout << "Enter filename to delete (Enter q to quit): ";
//		cin >> filename;
//
//		if(filename.compare("q") == 0)
//			break;
//		clock_t starttime = clock();
//		sse.remove(directoryPath + filename + ".");
//		cout << "Remove took " << ((double)(clock() - starttime)/CLOCKS_PER_SEC) << "." << endl;
//	}
//	
//	readDocs(sse);
//	}
}

TEST(SSEAddTest, Test1){
//	SSE sse;
//	string directoryPath = "datasets/email/enron_mail_20110402/maildir/mann-k/";
////	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
//
//	while(1){
//		string filename;
//		cout << "Enter fiename of the file to be added: (Enter q to quit): ";
//		cin >> filename;
//
//		if(filename.compare("q") == 0)
//			break;
//
//		sse.add(directoryPath + filename + ".");
//	}
//
//	readDocs(sse);
}

void deletefile(string file)
{
  if( remove(file.c_str()) != 0 )
    perror( "Error deleting file" );
  else
    puts( "File successfully deleted" );
}

void readDocs(SSE &sse2){
	while(1){
		string keyword;
		cout << "Enter keyword to search for (Enter q to quit): ";
		cin >> keyword;
		if(keyword.compare("q") == 0)
			break;
		
		SSE sse;
		vector<docid_t> docIDs;
		docIDs.reserve(10000);
		clock_t starttime = clock();
		if(sse.search(keyword, docIDs)){
			cout << "Number of documents with keyword " << keyword << " are " << docIDs.size() << endl;

			cout << "Search took " << ((double)(clock() - starttime)/CLOCKS_PER_SEC) << "." << endl;


			cout << "*************************************************docIDs***************************************************" << endl; 
			for(int i = 0; i < docIDs.size(); i++)
				cout << docIDs[i] << "\t";
			cout << endl;
			cout << "*************************************************docIDs***************************************************" << endl; 
		}
		else
			cout << "No files with keyword " << keyword << " found." << endl;
	}
}
