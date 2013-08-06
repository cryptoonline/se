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

void readDocs();
void add(string filepath, docid_t docID);
void remove(docid_t docID);

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
		double execTime = 0;
		sse.indexgen(directoryPath, execTime);
		cout << "indexgen took " << execTime << " seconds." << endl;
		
		readDocs();

		
		cout << "********************Add Test********************" << endl;
		string filepath2;
		cout << "Enter path of the file to add: ";
		cin >> filepath2;
		docid_t fileIDToRemove1;
		cout << "Enter Doc id: ";
		cin >> fileIDToRemove1;
		add(filepath2, fileIDToRemove1);
		
		readDocs();
		cout << "********************Remove Test********************" << endl;
		remove(fileIDToRemove1);
		
		cout << "********************Lazy Remove Test********************" << endl;
		docid_t fileIDToRemove;
		cout << "Enter id of the file to be removed: ";
		cin >> fileIDToRemove;
		remove(fileIDToRemove);

		cout << "********************Search with Lazy Delete Test********************" << endl;
		readDocs();

		cout << "********************Add Test after lazy delete********************" << endl;
		string filepath;
		cout << "Enter path of the file just deleted to add it back again: ";
		cin >> filepath;
		add(filepath, fileIDToRemove);
		readDocs();

	}

 
}

void add(string filepath, docid_t docID){
	SSE sse;
	double execTime = 0;

	sse.add(docID, filepath, execTime);
	cout << "Add took " << execTime*1000 << " millseconds." << endl;
	
}

void remove(docid_t docID){
	SSE sse;
	double execTime = 0;

	sse.remove(docID, execTime);
	cout << "Remove took " << execTime*1000 << " milliseconds." << endl;
}

TEST(SSERemoveTest, Test1){
//	for(int i = 0; i < 10; i++){
//	string op;
//	cout << "Enter i for generated and a for add: ";
//	cin >> op;
//	if(op.compare("i") == 0){
//	SSE ssegen;
//	ssegen.indexgen("/Users/naveed/BStore/datasets/test256MB/");
//	}
//	else{
//	SSE sse;

//	string directoryPath = "datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";

//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
//	while(1){
	//	string filename = "datasets/email/enron_mail_20110402_withoutheaders/maildir/dorland-c/deleted_items/20.";
//			string filename = directoryPath + "207.";
//		cout << "Enter fiename of the file to be added: (Enter q to quit): ";
//		cin >> filename;

//		if(filename.compare("q") == 0)
//			break;
//		clock_t starttime = clock();	
//		docid_t maxFileNum = 2^30;
//		sse.add(maxFileNum, filename);
//		cout << "Add took " << ((double)(clock() - starttime)/(double)CLOCKS_PER_SEC) << "." << endl;
//	}
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

void readDocs(){
	while(1){
		string keyword;
		cout << "Enter keyword to search for (Enter q to quit): ";
		cin >> keyword;
		if(keyword.compare("q") == 0)
			break;
		
		SSE sse;
		vector<docid_t> docIDs;
		docIDs.reserve(10000);
		double execTime = 0;
		if(sse.search(keyword, docIDs, execTime)){
			cout << "Number of documents with keyword " << keyword << " are " << docIDs.size() << endl;

			cout << "Search took " << execTime*1000 << " milliseconds." << endl;


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
