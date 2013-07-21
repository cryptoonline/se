//
// SSE_test.cc
// Searchable Encryption Test
//

#include "gtest/gtest.h"

#include <stdio.h>

#include "./../src/parameters.h"
#include "./../src/SSE.h"
#include "./../src/sse_parameters.h"

void readDocs(SSE &sse);
void deletefile(string file);

TEST(SSEReadTest, Test1){
	/* Delete D and T created by BlindStorage Tests */
//	deletefile(D_FILE);
//	deletefile(T_FILE);

	SSE sse;
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";

//	string directoryPath = "/Users/naveed/BStore/datasets/testdir/";
	sse.indexgen(directoryPath);

	readDocs(sse);
}

TEST(SSERemoveTest, Test1){
	SSE sse;

	while(1){
		string filename;
		cout << "Enter filename to delete (Enter q to quit): ";
		cin >> filename;

		if(filename.compare("q") == 0)
			break;

		sse.remove(filename);
	}
	
	readDocs(sse);
}

TEST(SSEAddTest, Test1){
	SSE sse;
	string directoryPath = "/Users/naveed/BStore/datasets/email/enron_mail_20110402/maildir/mann-k/inbox/";

	while(1){
		string filename;
		cout << "Enter fiename of the file to be added: (Enter q to quit): ";
		cin >> filename;

		if(filename.compare("q") == 0)
			break;

		sse.add(directoryPath + filename);
	}

	readDocs(sse);
}

void deletefile(string file)
{
  if( remove(file.c_str()) != 0 )
    perror( "Error deleting file" );
  else
    puts( "File successfully deleted" );
}

void readDocs(SSE &sse){
	while(1){
		string keyword;
		cout << "Enter keyword to search for (Enter q to quit): ";
		cin >> keyword;
		if(keyword.compare("q") == 0)
			break;

		vector<docid_t> docIDs;
		if(sse.search(keyword, docIDs)){
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
