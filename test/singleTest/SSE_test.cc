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
#include "./../../src/FileStore.h"
#include "./../../src/helper.h"


void readDocs();
void add(string filepath, docid_t docID);
void remove(docid_t docID);
void genIndex(string directoryPath, double& execTime){
	SSE sse;
	sse.indexgen(directoryPath, execTime);
}

void encryptFiles(string path){
	FileStore fstore;
	double execTime = 0;
	for(boost::filesystem::recursive_directory_iterator end, dir(path); dir!=end; ++dir){
		if(boost::filesystem::is_regular(dir->status())) {
		string docName = dir->path().string();

		size_t size = readFileSize(docName);
		byte* docBytes = new byte[size];
		readFile(docName, docBytes, size);
//		fstore.put(docName, docBytes, size, execTime);
		byte key[16] = {0};
		byte iv[16] = {0};
		AES cipher;
		clock_t startTime = clock();
		cipher.ENC_CTR(docBytes, docBytes, size, key, iv);
		execTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;
		delete[] docBytes;
		}	
	}
	cout << "Docs Encryption took " << execTime << " seconds." << endl;
}

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

		cout << "Enter directory path (Enter q to quit): ";
		string directoryName;
		cin >> directoryName;

			string docDirectoryPath = "/Users/naveed/BStore/datasets/documents/test/dir" + directoryName + "/";
			
			encryptFiles(docDirectoryPath);
			
			if(directoryName.compare("q") == 0)
			break;

		for(int i = 0; i < 1; i++){
			string directoryPath = "/Users/naveed/BStore/datasets/documents/test/text" + directoryName + "/";
			double execTime = 0;
			genIndex(directoryPath, execTime);
			cout << "indexgen took " << execTime << " seconds." << endl;
		}

//		for(int i = 0; i < 15; i++)	
			readDocs();

		
//		cout << "********************Add Test********************" << endl;
//		string filepath2;
//		cout << "Enter path of the file to add: ";
//		cin >> filepath2;
//		docid_t fileIDToRemove1;
//		cout << "Enter Doc id: ";
//		cin >> fileIDToRemove1;
//		add(filepath2, fileIDToRemove1);
//		
//		readDocs();
//		cout << "********************Remove Test********************" << endl;
//		remove(fileIDToRemove1);
	
		for(int i = 0; i < 15; i++){
			cout << "********************Lazy Remove Test********************" << endl;
			remove(i);

			cout << "********************Search with Lazy Delete Test********************" << endl;
			readDocs();
		}

		
		cout << "********************Add Test********************" << endl;
		FileStore fstore;
		for(int i = 0; i < 15; i++){
			string filepath;
//			filepath = "/Users/naveed/BStore/datasets/email/test256MB/test128MB/test64MB/beck-s/notes_inbox/143.";
//			filepath = "/Users/naveed/BStore/datasets/documents/archive/Directories/Directory7/Directory6/SITM.pdf";
//			string textpath = "/Users/naveed/BStore/datasets/documents/archive/Text/Directory7/Directory6/SITM.txt";
//			filepath = "/Users/naveed/BStore/datasets/documents/archive/Directories/Directory7/pricing_your_food_product.ppt";
//			string textpath = "/Users/naveed/BStore/datasets/documents/archive/Text/Directory7/pricing_your_food_product.txt";

//			filepath = "/Users/naveed/BStore/datasets/documents/archive/Directories/Directory7/SP800-145.pdf";
//			string textpath = "/Users/naveed/BStore/datasets/documents/archive/Text/Directory7/SP800-145.txt";

//			filepath = "/Users/naveed/BStore/datasets/documents/archive/Directories/Directory7/Where_is_the_Universe.ppt";
//				string textpath = "/Users/naveed/BStore/datasets/documents/archive/Text/Directory7/WhereistheUniverse.txt";
			double execTime;
			size_t size = readFileSize(filepath);
			byte* bytes = new byte[size];
			readFile(filepath, bytes, size);
	//		fstore.put(filepath, bytes, size, execTime);

		byte key[16] = {0};
		byte iv[16] = {0};
		AES cipher;
		clock_t startTime = clock();
		cipher.ENC_CTR(bytes, bytes, size, key, iv);
		execTime += (double)(clock()-startTime)/(double)CLOCKS_PER_SEC;

//		cout << "Encryption time for adding doc is " << execTime << endl;
//			add(textpath, 200000+i);
//			remove(200000+i);

			filepath = "/Users/naveed/BStore/datasets/email/test256MB/test128MB/test64MB/beck-s/notes_inbox/143.";
			add(filepath, 200000+i+4);
			remove(200000+i+4);

//			filepath = "/Users/naveed/BStore/datasets/email/test256MB/hernandez-j/all_documents/283.";
//			add(filepath, 200000+i+1);
//			remove(200000+i+1);
//			
//			filepath = "/Users/naveed/BStore/datasets/email/test256MB/kitchen-l/_americas/sec_media/67.";
//			add(filepath, 200000+i+2);
//			remove(200000+i+2);
//			
	//		filepath = "/Users/naveed/BStore/datasets/email/test256MB/test128MB/test64MB/test32MB/forney-j/sent_items/158.";
//			add(filepath, 200000+i+4);
//			remove(200000+i+4);
		}


	}
 
}

void add(string filepath, docid_t docID){
	SSE sse;
	double execTime = 0;

	cout << "Adding file " << filepath << endl;
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
//	while(1){
		string keyword = "the";
//		cout << "Enter keyword to search for (Enter q to quit): ";
//		cin >> keyword;
//		if(keyword.compare("q") == 0)
//			break;
		
		SSE sse;
		vector<docid_t> docIDs;
//		docIDs.re(10000);
		double execTime = 0;
		if(sse.search(keyword, docIDs, execTime)){
			cout << "Number of documents with keyword " << keyword << " are " << docIDs.size() << endl;

			cout << "Search took " << execTime*1000 << " milliseconds." << endl;


//			cout << "*************************************************docIDs***************************************************" << endl; 
//			for(int i = 0; i < docIDs.size(); i++)
//				cout << docIDs[i] << "\t";
//			cout << endl;
//			cout << "*************************************************docIDs***************************************************" << endl; 
		}
		else
			cout << "No files with keyword " << keyword << " found." << endl;

}
